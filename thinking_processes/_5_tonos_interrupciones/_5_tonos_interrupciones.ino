//** ReacXion Source Code **//
//** www.jeremyblum.com **//

/* Timer reload value, globally available */
unsigned int tcnt2;

/* Toggle HIGH or LOW digital write */
int toggle1 = 0;
int toggle2 = 0;
int toggle3 = 0;
int toggle4 = 0;
int toggle5 = 0;

/* Keep track of when each note needs to be switched */
int count1 = 0;
int count2 = 0;
int count3 = 0;
int count4 = 0;
int count5 = 0;

/* Frequency Output Pins */
#define FREQ1 9
#define FREQ2 10
#define FREQ3 11
#define FREQ4 12
#define FREQ5 13

//Setup Function will run once at initialization
void setup()
{

/* First disable the timer overflow interrupt*/
TIMSK2 &= ~(1<<TOIE2);

/* Configure timer2 in normal mode (no PWM) */
TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
TCCR2B &= ~(1<<WGM22);

/* Select clock source: internal I/O clock */
ASSR &= ~(1<<AS2);

/* Disable Compare Match A interrupt (only overflow) */
TIMSK2 &= ~(1<<OCIE2A);

/* Configure the prescaler to CPU clock divided by 128 */
TCCR2B |= (1<<CS22)  | (1<<CS20); // Set bits
TCCR2B &= ~(1<<CS21);             // Clear bit

/* We need to calculate a proper value to load the counter.
* The following loads the value 248 into the Timer 2 counter
* The math behind this is:
* (Desired period) = 64us.
* (CPU frequency) / (prescaler value) = 125000 Hz -> 8us.
* (desired period) / 8us = 8.
* MAX(uint8) - 8 = 248;
*/
/* Save value globally for later reload in ISR */
tcnt2 = 248;

/* Finally load end enable the timer */
TCNT2 = tcnt2;
TIMSK2 |= (1<<TOIE2);

//Configure I/O Pin Directions
pinMode(FREQ1,    OUTPUT);
pinMode(FREQ2,    OUTPUT);
pinMode(FREQ3,    OUTPUT);
pinMode(FREQ4,    OUTPUT);
pinMode(FREQ5,    OUTPUT);

}

/* Install the Interrupt Service Routine (ISR) for Timer2.  */
ISR(TIMER2_OVF_vect) {
/* Reload the timer */
TCNT2 = tcnt2;

count1++; count2++; count3++; count4++; count5++;

if (count1 == 60)
{
digitalWrite(FREQ1, toggle1 == 0 ? HIGH : LOW);
toggle1 = ~toggle1;
count1 = 0;
}
if (count2 == 53)
{
digitalWrite(FREQ2, toggle2 == 0 ? HIGH : LOW);
toggle2 = ~toggle2;
count2 = 0;
}
if (count3 == 47)
{
digitalWrite(FREQ3, toggle3 == 0 ? HIGH : LOW);
toggle3 = ~toggle3;
count3 = 0;
}
if (count4 == 40)
{
digitalWrite(FREQ4, toggle4 == 0 ? HIGH : LOW);
toggle4 = ~toggle4;
count4 = 0;
}
if (count5 == 35)
{
digitalWrite(FREQ5, toggle5 == 0 ? HIGH : LOW);
toggle5 = ~toggle5;
count5 = 0;
}
}

void loop()
{
//Do whatever else you want to do with your arduino!
}
