#include "TSSP.h"
long readLimit;
double angle;

TSSP sensor_IR;

void setup() {
  Serial.begin(115200);
  Serial1.begin(57600);
  delay(1000);
  pinMode(25, OUTPUT);
  readLimit=millis()+20;
}

void loop() {
  if(millis()>readLimit){
    sensor_IR.sensorInfo();
    angle = sensor_IR.angleIR();
<<<<<<< HEAD
    Serial.print(angle);
    Serial.print(" ");
    Serial.println();
 
=======

    Serial.println(angle);

  
    if(angle >= 0 ){
    angle = angle/2;
    Serial1.write(angle);
    digitalWrite(25, HIGH);
    }
    else {
      Serial1.write(255);
      digitalWrite(25, LOW);
    }
>>>>>>> 6cef51da26c14bddd40194683c833d03ab1d6ca8
  }
}
