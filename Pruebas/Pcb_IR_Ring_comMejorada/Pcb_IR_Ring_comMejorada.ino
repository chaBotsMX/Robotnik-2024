#include "TSSP.h"
long readLimit;
int angle;  
int intensidad;
int enviar;
int lastInten;
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

void setup() {
  Serial.begin(115200);   //puerto serial para debugear
  Serial1.begin(57600);  //puerto serial para enviar informacion, pines GPIO 16 RX y GPIO 17 TX, al actualizar las librerias pueden cambiarse los pines
  delay(1000); // se da tiempo para iniciar el uart de forma correcta o puede que los relojes no se sincronicen
  lastInten = 1; 
  pinMode(25, OUTPUT);  //led integrado en la rasp para debugear
  readLimit=millis()+50;
}

void loop() {
  if(millis()>readLimit){
    sensor_IR.sensorInfo();
    angle = sensor_IR.angleIR(); // Obtiene el ángulo
    int newIntensity = sensor_IR.intIR();  // Obtiene la nueva intensidad

    // Solo actualiza intensidad si la nueva lectura no es 0
    if (newIntensity != 0) {
      intensidad = newIntensity;
      lastInten = newIntensity;  // Actualiza la última intensidad válida
    } else {
      intensidad = lastInten;  // Utiliza la última intensidad válida si la nueva es 0
    }
  Serial.print("Ángulo: ");
  Serial.print(angle);
  Serial.print(", Intensidad: ");
  Serial.println(intensidad);
  // Ajustar si es necesario
  if(angle < 0 || angle > 360) angle = 200;
  if(intensidad < 0) intensidad = 0; // Asegura que la intensidad sea al menos 0
  if(intensidad > 1000) intensidad = 1000; // Asegura que la intensidad no exceda 1000

  // Estructura del paquete
  byte startMarker = 255;
  byte angleHigh = highByte(angle);
  byte angleLow = lowByte(angle);
  byte intensityHigh = highByte(intensidad);
  byte intensityLow = lowByte(intensidad);
  byte endMarker = 254;
  byte checksum = angleHigh + angleLow + intensityHigh + intensityLow;
  // Envío del paquete
  Serial1.write(startMarker);
  Serial1.write(angleHigh);
  Serial1.write(angleLow);
  Serial1.write(intensityHigh);
  Serial1.write(intensityLow);
  Serial1.write(checksum);
  Serial1.write(endMarker);



  // Debugging
  }
}
