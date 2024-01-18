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
  }
}
