#include "TSSP.h"
long readLimit;
double angle;

TSSP sensor_IR;

void setup() {
  Serial.begin(115200);
  Serial1.begin(74880);
  pinMode(25, OUTPUT);
  readLimit=millis()+20;
}

void loop() {
  if(millis()>readLimit){
    sensor_IR.sensorInfo();
    angle = sensor_IR.angleIR();
    Serial.print(angle);
    Serial.print(" ");
    Serial.println();
 
  }
}
