#include <Comunicacion.h>
#include <stddef.h>
#include <Arduino.h>
#include <HardwareSerial.h>


Comunicacion::Comunicacion() {
  
}

void Comunicacion::recive1()
{    
  if(Serial1.available() >= dataLength1 * sizeof(data1[0]))
  { 
    Serial1.readBytes((byte*)data1, dataLength1 * sizeof(data1[0]));
  } 
}

void Comunicacion::send1(uint8_t info1 = 0, uint8_t info2 = 0, uint8_t info3 = 0, uint8_t info4 = 0, uint8_t info5 = 0, uint8_t info6 = 0, uint8_t info7 = 0, uint8_t info8 = 0)
{
  data1[0] = info1;
  data1[1] = info2;
  data1[2] = info3;
  data1[3] = info4;
  data1[4] = info5;
  data1[5] = info6;
  data1[6] = info7;
  data1[7] = info8; 
  
  Serial1.write((byte*)data1, dataLength1 * sizeof(data1[0]));
}
 
void Comunicacion::request()
{
  const unsigned long startTime_us = micros();
  Serial1.write(1);

  do {
    if(Serial1.available())
    {
      recive1();
      status =1;
    }
    else
    {
      status = 0;
    }
  } while((micros() - startTime_us) < timeLimit);

}