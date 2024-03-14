#include "TSSP.h"
const size_t dataLength = 2; //Tamaño del array a enviar
int data[2] = {0,0}; //Iniciamos el array

long readLimit;
int angle;
int intensidad;
int enviar;
int lastInten;
#define intensidad_constante 140
#define intensidad_cuadrado 19600
TSSP sensor_IR;

int angulo(int x){
  if(x >= 180 && x <= 360){
    // Serial.println("debug");
    intensidad = 1000 - intensidad;
    return  x-90*intensidad/1200;
    

  }
  else if(x <180 && x >= 0){
//    y = 1762.2e-0.002x


    intensidad = 1000 - intensidad;
    return  x+90*intensidad/1200;
  }
  else{return 200;}
}

int ajusteAngulo(int x){
  int anguloModificado = x;
  if(anguloModificado<=350 && anguloModificado >=30){
    anguloModificado>=180 ? x=1 : x=0; 
    anguloModificado>=180 ? anguloModificado=360.00-anguloModificado : anguloModificado=anguloModificado;
    double t=sqrt(((intensidad*intensidad)+(intensidad_cuadrado))-2*(intensidad*intensidad_constante)*cos(anguloModificado*M_PI/180.00));
    double d=asin(((intensidad*sin(anguloModificado*M_PI/180.00))/t))*180.00/M_PI;
    if(!x)
      anguloModificado=180.00-(180.00-anguloModificado-d);
    else
      anguloModificado=180.00+(180.00-anguloModificado-d);

    return anguloModificado;
  } else {
    return 0;
  }
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
      
      enviar = ajusteAngulo(angle);
    }
    
    Serial.print(intensidad);
    Serial.print(" ");
    Serial.print(angle);
Serial.print(" ");
    
Serial.print(" ");
    Serial.println(enviar);
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
   // Serial.println(intensidad);
    
  }
  //el delay es para evitar que la raspb funcione al 100% y a si evitar problemas en la sincronizacion del uart
 
}  
