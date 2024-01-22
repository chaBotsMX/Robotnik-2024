#include "TSSP.h"
const size_t dataLength = 2;
int data[2] = {0,0};

long readLimit;
double angle;

TSSP sensor_IR;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(1000);
  pinMode(25, OUTPUT);
  readLimit=millis()+20;
}

void loop() {
  if(millis()>readLimit){
    sensor_IR.sensorInfo();
    angle = sensor_IR.angleIR();
<<<<<<< HEAD
  
    if(angle >= 255 )
    {
    data[0] = 255;
    data[1]= angle - 255;
    }  
    else
    {
    data[0] = angle;
    data[1] = 0;
    }

    if(Serial1.available()) 
    {
    Serial1.write((byte*)data, dataLength * sizeof(data[0]));
    digitalWrite(25, HIGH);
    }
    else 
    {
      
    digitalWrite(25, LOW);
    Serial.print(angle);
    Serial.print(" ");
    Serial.println();
    }
=======
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
>>>>>>> 0666b8d433cf1ee2026bac28569598e386895a24
  }
}  
