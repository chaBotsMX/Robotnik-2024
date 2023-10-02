#include "TSSP.h"
long readLimit;
double angle;

TSSP sensor_IR;

void setup() {
  Serial.begin(115200);
  readLimit=millis()+20;
}

void loop() {
  if(millis()>readLimit){
    sensor_IR.sensorInfo();
    angle = sensor_IR.angleIR();
    angle != -1?  angle = angle+180 : angle = -1;
    angle = floor(angle/5);
    angle = angle*5;
    Serial.print(angle);
    Serial.print(" ");
    Serial.println(sensor_IR.intIR());
  }
}
