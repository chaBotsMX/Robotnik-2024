#include "TSSP.h"
const size_t dataLength = 2; //Tamaño del array a enviar
int data[2] = {0,0}; //Iniciamos el array

long readLimit;
int angle;
int intensidad;
int enviar;
int lastInten;
TSSP sensor_IR;

int angulo(int x){
  if(x >= 180 && x <= 360){
    // Serial.println("debug");
    intensidad = 877 - intensidad;
    return  x-90*intensidad/877;
    

  }
  else if(x <180 && x >= 0){
//    y = 1762.2e-0.002x


    intensidad = 1000 - intensidad;
    return  x+90*intensidad/877;
  }
  else{return 200;}
}

void setup() {
  Serial.begin(115200);   //puerto serial para debugear
  Serial1.begin(115200);  //puerto serial para enviar informacion, pines GPIO 16 RX y GPIO 17 TX, al actualizar las librerias pueden cambiarse los pines
  delay(1000); // se da tiempo para iniciar el uart de forma correcta o puede que los relojes no se sincronicen
  pinMode(25, OUTPUT);  //led integrado en la rasp para debugear
  readLimit=millis()+20;
}

void loop() {

  if(millis()>readLimit){
    sensor_IR.sensorInfo();
    angle = sensor_IR.angleIR();  //se obtiene el angulo con la funcion de la libreria
    
    // si hay comunicacion entre la raspb y el receptor entonces se envia la informacion

    intensidad = sensor_IR.intIR();
    if(angle >= 0 && angle <= 360 ){
      int newInten =  sensor_IR.intIR();
      if(newInten != 0){
        intensidad = newInten;
        lastInten = newInten;
      }  
      else{
        intensidad = lastInten;
      }
      enviar = angulo(angle);
    }

    
    if (angle <= 20 && angle >=0 || angle >= 330 ){
    
        enviar = angle/2;
        digitalWrite(25,HIGH); 
      
    }
    else if(angle < 0 ){
    
      enviar = 200;
      digitalWrite(25,HIGH);
        
    }
    else{
      enviar = enviar/2;
    }

    
    Serial1.write(enviar);
   
    Serial.print(enviar);
    Serial.print(" ");
    Serial.println(angle);
  }
  //el delay es para evitar que la raspb funcione al 100% y a si evitar problemas en la sincronizacion del uart
  
}  
