#include <M5Stack.h>

int estadoA=0;
int estadoB=0;
int estadoC=0;

void setup() {
    M5.begin();        
    M5.Power.begin();   
    M5.Lcd.setTextColor(GREEN);  
    M5.Lcd.setTextSize(4);     
    Serial.begin(9600);
}

void loop() {
  M5.update(); 
  BotonA();
  BotonB();
  BotonC();
}


void BotonA(){
  if(M5.BtnA.wasPressed()){
    Serial.println(estadoA);
    if(estadoA==1) {
      estadoA=0;
    }
    else if(estadoA==0) {
      estadoA=1;
    }
  }
  M5.Lcd.setCursor(70,90);
  if (estadoA==0) { 
    M5.Lcd.clear(BLACK);  
    M5.Lcd.println("Jugando");
    delay(100);
  }
  else if(estadoA==1){
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(50,90);
    M5.Lcd.println("Jugandon't");
    delay(100);
  }
  delay(20);
}


void BotonB(){
  if(M5.BtnB.wasPressed()){
    Serial.println(estadoB);
    if(estadoB==1) {
      estadoB=0;
    }
    else if(estadoB==0) {
      estadoB=1;
    }
  }
  M5.Lcd.setCursor(70,90);
  if (estadoB==0) {   
    M5.Lcd.clear(BLACK);  
    M5.Lcd.println("Calibrando dirección");
    delay(100);
  }
  else if(estadoB==1){
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(50,90);
    M5.Lcd.println(":)");
    delay(100);
  }
  delay(20);
}


void BotonC(){
  if(M5.BtnC.wasPressed()){
    Serial.println(estadoC);
    if(estadoC==1) {
      estadoC=0;
    }
    else if(estadoC==0) {
      estadoC=1;
    }
  }
  M5.Lcd.setCursor(70,90);
  if (estadoC==0) {  
    M5.Lcd.clear(BLACK);  
    M5.Lcd.println("Calibrando pista");
    delay(100);
  }
  else if(estadoC==1){
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(50,90);
    M5.Lcd.println(":)");
    delay(100);
  }
  delay(20);
}