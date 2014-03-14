/*  Este programa usa el timer 1 del arduino para hacer 2 notas simultáneas.
  Modifica variables del timer para poder funcionar.
Setup: 4 buzzers conectados a pins 2, 3, 4 y 5. No olvidar resistencias de pulldown (controlan corriente)
*/
#include "notas.h"
/* Tener las notas en un arreglo ayuda a automatizar cálculos para otros valores importantes */

/* Buzzers activos */

char index=0;
/* Arreglo con 4 acordes*/
char* chords[][41]={{"S","S","S","S"},{"C4","E4","G4","C5"},{"A3","C4","E4","A4"},{"F3","A3","C4","F4"}};//,{"G3","B3","D4","G4"}};
word tempo=60;

#define N_BUZZERS 4

int NOTAS[]={NOTE_B0,NOTE_C1,NOTE_CS1,NOTE_D1,NOTE_DS1,NOTE_E1,NOTE_F1,NOTE_FS1,NOTE_G1,NOTE_GS1,NOTE_A1,NOTE_AS1, //12
             NOTE_B1,NOTE_C2,NOTE_CS2,NOTE_D2,NOTE_DS2,NOTE_E2,NOTE_F2,NOTE_FS2,NOTE_G2,NOTE_GS2,NOTE_A2,NOTE_AS2, //24
             NOTE_B2,NOTE_C3,NOTE_CS3,NOTE_D3,NOTE_DS3,NOTE_E3,NOTE_F3,NOTE_FS3,NOTE_G3,NOTE_GS3,NOTE_A3,NOTE_AS3, //36
             NOTE_B3,NOTE_C4,NOTE_CS4,NOTE_D4,NOTE_DS4,NOTE_E4,NOTE_F4,NOTE_FS4,NOTE_G4,NOTE_GS4,NOTE_A4,NOTE_AS4, //48
             NOTE_B4,NOTE_C5,NOTE_CS5,NOTE_D5,NOTE_DS5,NOTE_E5,NOTE_F5,NOTE_FS5,NOTE_G5,NOTE_GS5,NOTE_A5,NOTE_AS5, //60
             NOTE_B5,NOTE_C6,NOTE_CS6,NOTE_D6,NOTE_DS6,NOTE_E6,NOTE_F6,NOTE_FS6,NOTE_G6,NOTE_GS6,NOTE_A6,NOTE_AS6, //72
             NOTE_B6,NOTE_C7,NOTE_CS7,NOTE_D7,NOTE_DS7,NOTE_E7,NOTE_F7,NOTE_FS7,NOTE_G7,NOTE_GS7,NOTE_A7,NOTE_AS7, //84
             NOTE_B7,NOTE_C8,NOTE_CS8,NOTE_D8,NOTE_DS8,SILENCIO}; //90

/* Nota. Tiene la nota que tocará cada buzzer. En realidad, posee el subíndice a la nota
asociada del arreglo de notas. 8 bits bastan, por lo que cabe en un char.*/
//char* nota[]={"C4", "E4", "G4", "B4"};
char parseNota(char s[]){
  if (s[0]=='S') return 89;
  /* recibe un string con el nombre de la nota. Se asumirá que los inputs son correctos*/
  
  /* Para parsear el string, hay que observar las siguientes cosas:
  El string tiene dos formas posibles: 
                      -XY
                      -XSY
    donde X en {A, B, C, D, E, F}
          Y en {0,1,2,3,4,5,6,7,8}
  
  Notar que: el número fija particiones de tamaño 12. El 1 envía a la primera, y
  así sucesivamente. La letra (y su sostenido siesque tiene) ubica la nota dentro de
  la partición.*/
  /* Recordando que A=65, ..., G=71: */
  char pos;
  if (s[1]!='S'){//era natural, sin sostenido
    Serial.println("natural");
    switch (s[0]){
      case 65: //A
        pos=9;
        break;
      case 66: //B
        pos=11;
        break;
      case 67: //C
        pos=0;
        break;
      case 68: //D
        pos=2;
        break;
      case 69: //E
        pos=4;
        break;
      case 70: //F
        pos=5;
        break;
      case 71: //G
        pos=7;
        break;
    }
  }
  else{
    Serial.println("sostenido");
    switch (s[0]){
      case 65: //AS
        pos=10;
        break;
      //no existe BS
      case 67: //CS
        pos=1;
        break;
      case 68: //DS
        pos=3;
        break;
      //no existe ES
      case 70: //FS
        pos=6;
        break;
      case 71: //GS
        pos=8;
        break;
    }
  }
  /* Ojo que la partición la define s[largo-1]*/
  return (((s[(s[1]=='S') ? 2 : 1]-1) - '0')*12)+1+pos;
}
/* Arreglo con los períodos de cada nota en el buzzer, en microsegundos */
int NOTAS_P[N_BUZZERS];
const int mcd=30; //Propuesto por Jeremy Blum: 64. Se obligará a los períodos a ser un múltiplo de esto
//ha funcionado bien para 71. Probando con 30, que es más preciso.
/* Inicializador de los períodos */
void initNotas_p(){
  for (int i=0;i<N_BUZZERS;i++){
    int periodo=(int)(500000.0/NOTAS[parseNota(chords[index][i])]);//nota[i])]); //periodo real de cada nota
    //NOTAS_P[i]=periodo;
    NOTAS_P[i]=periodo + ((periodo%mcd)<=(mcd/2) ? -(periodo%mcd) : (mcd-(periodo%mcd)));     //nuevo período es múltiplo de 64 más cercano al período original
  }
}

/* Count. Lleva la cuenta de ticks para cada buzzer. Números pequeños, 
  caben en un char*/
unsigned int count[N_BUZZERS];

/* Cantidad de cuentas por cada nota. El máximo es 252 asi que cabe en 8 bits.
  Las cuentas son el período total dividido por el tiempo que se demora en reiniciar el contador
  del timer. Como se quiere que el timer se demore "mcd" microsegundos (y se forzará que esto
  se cumpla), cuentas es período/mcd.
OJO: PRÓXIMAMENTE VER SI SE PUEDE CAMBIAR POR TIMER 2 (8 bits) EN VEZ DE TIMER 1 (16 bits)
*/
unsigned char counts[N_BUZZERS];
void initCounts(){
  for (int i=0;i<N_BUZZERS;i++){
    counts[i]=NOTAS_P[i]/mcd;
    count[i]=0;
  }
}

/* Número de pin para cada buzzer. Caben en 8 bits */
char buzz[N_BUZZERS];

/* Toggle. Indica cuando enviar un Low o un High para cada buzzer.
  Toma los valores LOW o HIGH, que equivalen a false o true*/
boolean toggle[N_BUZZERS];

void initTimer2(){
  /* Desactivar interrupciones por overflow del contador*/
  TIMSK2  &= ~(1<<TOIE2);
  /* Activar interrupciones por Compare Match A*/
  TIMSK2 |= (1<<OCIE2A);
  /* Limpiar registros TCCR2A y TCCR2B*/
  TCCR2A = 0;
  TCCR2B = 0;
  /* Setear modo CTC (Clear Timer on Compare match): se resetea
  automáticamente el timer al igualarse al valor OCR2A por fijar*/
  TCCR2A |= (1<<WGM21);
  /* Configurar el prescaler a 8*/
  TCCR2B |= (1<<CS21);
  /* Setear el valor para el cual se generará una interrupción.
  El contador del timer 2 llega a 255. Se llegará a mcd*2 -1.
  Usando un mcd=71 (valor empírico) se tiene que este valor es
  igual a 141.*/
  OCR2A=mcd*2 -1;
}

void initBuzzers(){
  for (int i=0; i<N_BUZZERS; i++){
    buzz[i]=i+2;
    pinMode(i+2,OUTPUT);
  }
}
void setup(){
  initBuzzers();
  initNotas_p();
  initCounts();
  initTimer2();
  Serial.begin(9600);
  
}

/* Definición del handler de interrupciones del timer: ISR (Interrupt Service Routine)*/
ISR(TIMER2_COMPA_vect){
  //TCNT2 = 0;
  /* Recorrer cada buzzer. Para cada uno, aumentar su cuenta y ver si hay que cambiar toggle*/
  for (int i=0; i<N_BUZZERS; i++){
    count[i]++;
    if (count[i] == counts[i]){
      toggle[i]=~toggle[i];
      digitalWrite(buzz[i], toggle[i]);
      count[i]=0;
    }
  }
}



void update(){
  initNotas_p();
  initCounts();
}
void loop(){
  delay(600);
  Serial.println("hola");
  index=(index+1)%4;
  update();
}


/* Review: 
-Día 1: Al parecer el asunto del mcd no es trivial: si alguna combinación de notas hace
que las interrupciones ocurran muy seguido, sencillamente no funciona. Deducción sacada a partir de
la experiencia de que para algunas tuplas de notas funciona y ambas suenan al unísono, para otras
hay sonido pero con notas extrañas y para otras sencillamente no hay sonido. Habría que de alguna
manera modificar los valores de las frecuencias para los casos donde las interrupciones ocurren
muy seguido. Habría que modificar la frecuencia de manera de que exista un mcd relativamente 
significativo. Por ejemplo, mmodificarlas todas para que siempre sea 5.
-Día 2: Se generan períodos aproximados de manera de cumplir con un mcd global. Con esto se 
elimina el problema anterior y se tienen notas bastante buenas. Se cambiará a timer 2 para no
desperdiciar la capacidad del timer 1 en términos de bits del counter.
Ahora se usa timer 2, y el modo de introducir notas es en texto. Se parsean con una función especial.
El reproductor funciona y puede tocar diversos acordes. La meta ahora es determinar un método de introducción
de la música que sea cómodo y considere duraciones de cada nota para todas las voces. Los silencios deberían
poder implementarse como frecuencias nulas. Silencios implementados
  Idea:
    Melodías como un string, para cada voz (cada Buzzer)
    Protocolo: AB(S)C o AS si es silencio.
          A=Duración (1=redonda, 2= blanca, 4=negra, 8=corchea, 16=fusa)
          B=Nota (A, B, C, D, E, F, G)
          C=Octava de la nota. de 0 a 8, el La 440 es A4
    Separador: Espacio.
    Ejemplo: "8C5 8S 8G4 8G4 8A4 8S 8G4 4S 8S 8B4 8S 8C4"
    Eso para una sola voz.
    
    Cada voz carga su nota y debe durar lo que dice la información. 
    Cargar la nota significa: 
      -Actualizar las notas del vector de notas. Notar que el vector de notas tiene strings.
      -Establecer duración de nota. Para esto, crear un contador para cada nota, que 
        parte en el valor que le corresponde y decrece hasta llegar a 0, en cuyo caso
        se procede a leer la siguiente nota para esa voz, desde el string destinado a eso.
        Como idea para la lectura, ir "consumiendo" de alguna manera el string.
    Tras cargar la nota, actualizar los "counts" y los períodos "NOTAS_P" para cada nota. 
      Para eso existe función update();
    El delay de cada ciclo depende del tempo que se especifique.
    
    .*/
