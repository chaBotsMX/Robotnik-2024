/*************************************************************************/
/*                                                                       */
/*                                                                       */
/****************************CHABOTS ROBOTNIK 2021************************/
/*                                                                       */
/*TSSP V 1.0													         */
/*S.L.P MEXICO                           TIME:14:28:48 PM DATE:06/02/2020*/
/*************************************************************************/

#include "Arduino.h"
#include "TSSP.h"

TSSP::TSSP(){
  for(int i = 0; i<NUM_TSSP; i++)
    pinMode(tssp[i],INPUT);  
}

void TSSP::sensorInfo(){
  const unsigned long startTime_us = micros();
  for(int i=0; i<NUM_TSSP; i++)
    lectura[i]=0;
  do {
    for(int i = 0; i<NUM_TSSP; i++){
      if(!digitalRead(tssp[i])) {
        lectura[i]+=delta;
      }
    }
  } while((micros() - startTime_us) < timeLimit);
}

int TSSP::angleIR(){
  double x1=0;
  double y1=0;
  int counter=0;
  double angle; 
  //Se hace la lectura del arreglo donde estan registradas las lecturas de los TSSPs
  for (int i=0; i<NUM_TSSP;  i++){
    //Si algun tssp marca lectura se hace la suma del vector registrado
    if (lectura[i]>0){
      x1=x1+(lectura[i]*vecx[i]);
      y1=y1+(lectura[i]*vecy[i]);
      counter++;
    }
  }
  counter !=0 ? angle=atan2(x1,y1)/M_PI*180.00 : angle= -1; //Se saca la arcotangente de la suma de los vectores
  return angle;  
}

int TSSP::hipIR(){
  int hip;
  double x1=0;
  double y1=0;
  int counter=0;
  double angle; 
  //Se hace la lectura del arreglo donde estan registradas las lecturas de los TSSPs
  for (int i=0; i<NUM_TSSP;  i++){
  //Si algun tssp marca lectura se hace la suma del vector registrado
    if (lectura[i]>0){
      x1=x1+(lectura[i]*vecx[i]);
      y1=y1+(lectura[i]*vecy[i]);
      
    }
  }
  hip=sqrt(pow(x1,2.0) + pow(y1,2.0));
  return hip;
}

int TSSP::intIR(){
  int intensidad=0;
  for(int i=0; i<NUM_TSSP; i++){
    if(lectura[i]>0)
      count++;  
    intensidad=max(intensidad, lectura[i]);
  }
  return intensidad;
}