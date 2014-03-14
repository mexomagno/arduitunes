var octava=4;
var duracion=4;
var silencio=false;
var voz_activa=1;
var cursores=[0,0,0,0,0];
var nombres=["Voz 1", "Voz 2", "Voz 3", "Voz 4", "Voz 5"];
//var repeticion=[false,false,false,false,false]; //true si se está poniendo una repetición
/* Arreglos de cada voz*/
var voz1=[];
var voz2=[];
var voz3=[];
var voz4=[];
var voz5=[];
var arrvoces=[voz1, voz2, voz3, voz4, voz5];
function addNota(nota,oct){
	if ((octava<7)||((octava==7)&&(oct==0))||((octava==7)&&(oct==1)&&((nota=="C")||(nota=="CS")||(nota=="D")||(nota=="DS")))){
		/* agregar en el arreglo de la voz*/
		//arrvoces[voz_activa-1].push(""+duracion+nota+""+(octava+oct));
		arrvoces[voz_activa-1].splice(cursores[voz_activa-1],0,""+duracion+nota+""+(octava+oct));
		/* mostrar melodía actual */
		cursores[voz_activa-1]++;
		
		refreshDisplay(); //es lento!
		
		var wrap=document.getElementById("display_notas_wrap");
		wrap.setAttribute("style","background-color: #DDDDDD;");
		window.setTimeout(function(){wrap.setAttribute("style","");},100);
		//document.getElementById("mensajes").innerHTML=voz1[voz1.length-1];
		/* reproducir sonido */
		play(nota,oct);
	}
}
/*function play(nota,oct){
	/* toca la nota en el texto nota *
	var suma=octava+oct;
	document.getElementById("dummy").innerHTML="<embed src='sounds/"+nota+""+suma+".mp3' hidden='true' autostart='true' loop='false' />";
}*/
function play(nota,oct){
	var suma=octava+oct;
	var texto="<audio autoplay><source src='sounds/wav/"+nota+suma+".wav' type='audio/wav'><source src='sounds/mp3/"+nota+suma+".mp3' type='audio/mpeg'>Audio no soportado :(</audio>";
	document.getElementById("dummy").innerHTML=texto.toLowerCase();
	//window.alert(suma+nota+".wav");
}
function playString(s){
	if ((s!=":||") && (s!="||:") && (s.substring(s.length-1) != "S")){
		var oct=parseInt(s.substring(s.length-1));
		var nota=s.substring((s.substring(s.length-2,s.length-1)=="S") ? s.length-3 : s.length-2,s.length-1);
		play(nota,oct-octava);
	}
}
function bajar(){
	octava= (octava>2) ? octava-1 : 2;
	update();
}
function subir(){
	octava= (octava<7) ? octava+1 : 7;
	update();
}
function update(){
	document.getElementById("octava1").innerHTML=octava;
	document.getElementById("octava2").innerHTML=octava+1;
	document.getElementById("boton_bajar").setAttribute("style", (octava>2) ? "" : "background-color: #EEEEEE; border-color: #BBBBBB; box-shadow: inset 0px 0px 0px 0px #CCCCCC;");
	document.getElementById("boton_subir").setAttribute("style", (octava<7) ? "" : "background-color: #EEEEEE; border-color: #BBBBBB; box-shadow: inset 0px 0px 0px 0px #CCCCCC;");
	var prohibidas=document.getElementsByClassName("prohibida")
	if (octava==7){//entonces en la segunda octava hay notas no admitidas
		for (i=0;i<prohibidas.length;i++ in prohibidas){
			prohibidas[i].setAttribute("style", "background-color: #FF0000;");
			if (prohibidas[i].className=="tecla_negra prohibida"){
				prohibidas[i].setAttribute("style", "background-color: #CC0000;");
			}
		}
	}
	else{
		for (i=0;i<prohibidas.length;i++){
			prohibidas[i].setAttribute("style","");
		}
	}
}
function refreshDisplay(){
	/* obtener display*/
	var display=document.getElementById("display_notas");
	/* vaciar display*/
	display.innerHTML="";
	/* agregar notas*/
	for (j=0;j<arrvoces[voz_activa-1].length;j++){
		var div=document.createElement("div");
		div.setAttribute("class","preview_notas");
		//div.setAttribute("id","nota_"+(arrvoces[voz_activa-1].length-1));
		div.setAttribute("onClick","cursores["+voz_activa+"-1]="+(j+1)+"; playString('"+arrvoces[voz_activa-1][j]+"'); refreshDisplay();");
		div.setAttribute("id","voz_"+voz_activa+"_nota_"+j);
		alert("se seteo id="+"voz_"+voz_activa+"_nota_"+j);
		div.innerHTML=arrvoces[voz_activa-1][j];
		display.appendChild(div);
		/* poner el cursor en su posición*/
		if (j==(cursores[voz_activa-1]-1)){ 
			var cursor=document.createElement("div");
			cursor.setAttribute("id","cursor");
			display.appendChild(cursor);
		}
		//display.innerHTML+=" "+arrvoces[voz_activa-1][j];
	}
}
function setBotones(){
	var tabla=document.getElementById("tabla_botones_duracion");
	var fila=document.createElement("tr");
	for (i=1; i<=32;i*=2){
		var col=document.createElement("td");
		
		var div2=document.createElement("div");
		div2.setAttribute("class", "botones_duracion");
		div2.setAttribute("id","boton_duracion_"+i);
		div2.setAttribute("onClick","setDuracion("+i+");");
		var img=document.createElement("img");
		img.setAttribute("id","imagen_boton_duracion_"+i);
		img.setAttribute("src","images/"+i+".png");
		img.setAttribute("alt",""+i);
		img.setAttribute("class","img_botones_duracion");
		div2.appendChild(img);
		col.appendChild(div2);
		fila.appendChild(col);
		//div.appendChild(div2);
	}
	/* agregar botones de repetición */
	var rep1=document.createElement("div");
	rep1.setAttribute("class","boton_repeticion");
	rep1.setAttribute("id","boton_repeticion_1");
	rep1.setAttribute("onClick","addRep(false);");
	var imgrep1=document.createElement("img");
	imgrep1.setAttribute("src","images/rep1.png");
	imgrep1.setAttribute("alt","rep1.png");
	imgrep1.setAttribute("class","img_botones_duracion");
	var rep2=document.createElement("div");
	rep2.setAttribute("class","boton_repeticion");
	rep2.setAttribute("id","boton_repeticion_2");
	rep2.setAttribute("onClick","addRep(true);");
	var imgrep2=document.createElement("img");
	imgrep2.setAttribute("src","images/rep2.png");
	imgrep2.setAttribute("alt","rep2.png");
	imgrep2.setAttribute("class","img_botones_duracion");
	rep1.appendChild(imgrep1);
	rep2.appendChild(imgrep2);
	var col1=document.createElement("td");
	col1.setAttribute("style","border-left: 2px solid #DDDDEE");
	col1.appendChild(rep1);
	fila.appendChild(col1);
	var col2=document.createElement("td");
	col2.appendChild(rep2);
	
	/* Agregar selector de silencio*/
	var tdsil=document.createElement("th");
	tdsil.setAttribute("colspan","6");
	tdsil.setAttribute("style","border-right: 2px solid #DDDDEE;");
	var silencio=document.createElement("div");
	silencio.innerHTML="Silencios";
	silencio.setAttribute("id","silencio");
	silencio.setAttribute("onClick","toggleSilencio();");
	tdsil.appendChild(silencio);
	var threp=document.createElement("th");
	threp.setAttribute("colspan","2");
	var rephd=document.createElement("div");
	rephd.innerHTML="Repetir";
	threp.appendChild(rephd);
	var filasil=document.createElement("tr");
	filasil.appendChild(tdsil);
	filasil.appendChild(threp);
	fila.appendChild(col2);
	tabla.appendChild(filasil);
	tabla.appendChild(fila);
}
function toggleSilencio(){
	/* cambiar variable*/
	silencio=!silencio;
	/* mostrar el cambio en el botón*/
	var botonSilencio=document.getElementById("silencio");
	botonSilencio.setAttribute("style", silencio ? "background: #AAECAA;" : "");
	botonSilencio.innerHTML=silencio ? "Notas" : "Silencios";
	/* cambiar imágenes de botones*/
	for (i=1;i<=32;i*=2){
		var boton=document.getElementById("imagen_boton_duracion_"+i);
		boton.setAttribute("src",silencio? "images/"+i+"s.png" : "images/"+i+".png");
		boton.setAttribute("alt",silencio? "images/"+i+"s.png" : "images/"+i+".png");
	}
	document.getElementById("boton_duracion_"+duracion).setAttribute("style", silencio ? "" : "background-color: #AAAACC;");
}
function setDuracion(dur){
	if (!silencio){
		document.getElementById("boton_duracion_"+duracion).setAttribute("style","");
		duracion=dur;
		/* mostrar duración seleccionada coloreando el botón correspondiente*/
		document.getElementById("boton_duracion_"+duracion).setAttribute("style","background-color: #AAAACC;");
	}
	else{
		/* guardar el silencio en las notas*/
		arrvoces[voz_activa-1].splice(cursores[voz_activa-1],0,""+dur+"S");
		/* mostrar melodía actual */
		cursores[voz_activa-1]++;
		refreshDisplay();
		var wrap=document.getElementById("display_notas_wrap");
		wrap.setAttribute("style","background-color: #DDDDDD;");
		window.setTimeout(function(){wrap.setAttribute("style","");},100);
	}
}
function undo(){
	if (cursores[voz_activa-1]>0){
		/* borrar última nota escrita*/
		//var ultima = arrvoces[voz_activa-1].pop();
		//var ultima = arrvoces[voz_activa-1].splice(cursores[voz_activa-1]-1,1);
		/* ver que hacer si era una repetición*/
		/*if (ultima==":||"){
			repeticion[voz_activa-1]=true;
			enableRep();
		}
		if (ultima=="||:"){
			repeticion[voz_activa-1]=false;
			enableRep();
		}*/
		/* Escribir nuevamente todas las notas*/
		if (cursores[voz_activa-1]>0) cursores[voz_activa-1]--;
		refreshDisplay();
		/* mostrar flash rojizo*/
		var wrap=document.getElementById("display_notas_wrap");
		wrap.setAttribute("style","background-color: #FFDDDD;");
		window.setTimeout(function(){wrap.setAttribute("style","");},100);
	}
	else{
		/* mostrar flash rojizo*/
		var wrap=document.getElementById("display_notas_wrap");
		wrap.setAttribute("style","background-color: #FFDDDD;");
		window.setTimeout(function(){wrap.setAttribute("style","");},100);
	}
}
function setVoces(){
	var tabla=document.getElementById("tabla_voces");
	var fila=document.createElement("tr");
	for (i=1;i<=5;i++){
		var col=document.createElement("td");
		var voz=document.createElement("div");
		voz.setAttribute("id","voz"+i);
		voz.setAttribute("class","voces");
		voz.setAttribute("onClick","setVoz("+i+")");
		voz.innerHTML="Voz "+i;
		var tf=document.createElement("input");
		tf.setAttribute("style","display: none;");
		tf.setAttribute("id","tf_voz"+i);
		tf.setAttribute("onKeyDown","keyDown(event,"+i+");");
		voz.appendChild(tf);
		col.appendChild(voz);
		fila.appendChild(col);
	}
	tabla.appendChild(fila);
}
function setVoz(i){
	//if (voz_activa!=i){//si no estaba parado en esa voz
		/* desseleccionar última voz*/
		var tab=document.getElementById("voz"+voz_activa);
		
		tab.setAttribute("style","");
		/* cambiar variable*/
		voz_activa=i;
		/* mostrar cambio en pestañas*/
		tab=document.getElementById("voz"+voz_activa);
		tab.setAttribute("style","background-color: #FFBB22; padding-top: 30px;");
		refreshDisplay();
		enableRep();
	//}
	//else{
		/*var tf=document.getElementById("tf_voz"+i);
		var name=document.getElementById("voz"+i);
		tf.setAttribute("style","");
		name.setAttribute("style","display:none;");*/
	//}
}
/*function keyDown(elEvento,voz) {
  var evento = elEvento || window.event;
  if (evento.keyCode==13){//apretó enter
	  var tf = document.getElementById("tf_voz"+voz);
	  var name=document.getElementById("voz"+voz);
	  name.setAttribute("style","");
	  tf.setAttribute("style","display:none;");
	  nombres[voz-1]=tf.value;
	  name.innerHTML=tf.value;
	  tf.value="";
  }
}*/
function addRep(i){
	/* i puede valer false o true, significando ||: o :|| respectivamente*/
	//if (i==repeticion[voz_activa-1]){
		//var rep= repeticion[voz_activa-1] ? ":||" : "||:";
		var rep= i ? ":||" : "||:";
		//arrvoces[voz_activa-1].push(""+rep);
		/* añadir silencio a arreglo de notas correspondiente*/
		arrvoces[voz_activa-1].splice(cursores[voz_activa-1],0,""+rep);
		/* moviendo el cursor*/
		cursores[voz_activa-1]++;
		/* mostrar silencio en display*/
		refreshDisplay(); //Lento!!!
		/* para control de repeticiones. Puede eliminarse, pues de esto se preocupa la validación.*/
		//repeticion[voz_activa-1]=!repeticion[voz_activa-1];
		//enableRep();
		/* blink del display*/
		var display=document.getElementById("display_notas_wrap");
		display.setAttribute("style","background-color: #DDDDDD;");
		window.setTimeout(function(){display.setAttribute("style","");},100);
	//}
}
function enableRep(){
	/* una suerte de update para los botones de repetición*/
	var rep1=document.getElementById("boton_repeticion_1");
	var rep2=document.getElementById("boton_repeticion_2");
	//rep1.setAttribute("style", repeticion[voz_activa-1] ? "background-color: #998888;" : "");
	//rep2.setAttribute("style", repeticion[voz_activa-1] ? "" : "background-color: #998888;");
}
function generar(){
	if (validar()){
		var textareawrap=document.getElementById("textarea_wrap");
		textareawrap.setAttribute("style","display: inline;");
		var texto=document.getElementById("texto");
		/* crear el texto compatible con instrucciones para arduino*/
		var respuesta="";
		for (i=0;i<5;i++){
			respuesta+='PROGMEM prog_char voz'+(i+1)+'[]={<br>"';
			for (j=0;j<arrvoces[i].length;j++){
				respuesta+=arrvoces[i][j]+' ';
				if ((j+1)%40==0){
					respuesta+='"<br>"';
				}
			}
			respuesta+='X"};<br>';
		}
		texto.innerHTML=respuesta;
		window.location.hash="#texto";
		/* dejar el texto seleccionado*/
		if (document.selection) {
			var range = document.body.createTextRange();
			range.moveToElementText(texto);
            range.select();
        } else if (window.getSelection) {
            var range = document.createRange();
            range.selectNode(texto);
            window.getSelection().addRange(range);
        }
	}
}
function validar(){
	/* validar que haya cerrado todas las repeticiones*/
	var vacias=0;
	for (i=0;i<5;i++){
		//revisar que no haya repeticiones anidadas y que estén balanceadas
		var rep=0; //si vale x <=-1 o x>=2, hay error en las repeticiones
		for (j=0;j<arrvoces[i].length;j++){
			if (arrvoces[i][j]=="||:"){
				rep++;
			}
			if (arrvoces[i][j]==":||"){
				rep--;
			}
			if ((rep>=2)||(rep<=-1)){
				window.alert("Voz "+(i+1)+": ¡No se admiten repeticiones anidadas!");
				setVoz(i+1);
				/* llevar a donde está el error*/
				/* Obtener nota donde se equivocó*/
				var div=document.getElementById("voz_"+(i+1)+"_nota_"+j);
				alert("se pide al elemento voz_"+(i+1)+"_nota_"+j);
				div.setAttribute("style","background-color: #FF5555;");
				window.hash.location="#display_notas_wrap";
				return false;
			}
		}
		if (rep!=0){
			window.alert("Debe cerrar la repetición de la voz "+(i+1));
			setVoz(i+1);
			return false;
		}
		/*if (repeticion[i]){
			window.alert("Debe cerrar la repetición de la voz "+(i+1));
			setVoz(i+1);
			return false;
		}*/
		if (arrvoces[i].length==0){
			vacias++;
		}
	}
	 /* validar que haya alguna melodía*/
	if (vacias==5){
		window.alert("¡No ha ingresado ninguna melodía! :(");
		return false;
	}
	return true;
}
function init(){
	setBotones();
	setDuracion(4);
	setVoces();
	setVoz(1);
	enableRep();
	update();
	refreshDisplay();
}
