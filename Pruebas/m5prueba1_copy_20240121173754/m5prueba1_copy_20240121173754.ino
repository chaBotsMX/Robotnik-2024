#include <M5Stack.h>

int estado=0;

void setup() {
    M5.begin();        
    M5.Power.begin();   
    M5.Lcd.setTextColor(GREEN);  
    M5.Lcd.setTextSize(5);
    M5.Lcd.println("Prueba");
    delay(1000);     
    Serial.begin(9600);
}

void BotonA(){
  if(M5.BtnA.wasPressed()){
    Serial.write(estado);
    if(estado==1) {
      estado=0;
    }
    else if(estado==0) {
      estado=1;
    }
  }
  M5.Lcd.setCursor(70,90);
  if(estado==1){
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(35,90);
    M5.Lcd.println("Jugando");
    delay(100);
  }
  delay(20);
}


void BotonB(){
  if(M5.BtnB.wasPressed()){
    Serial.write(estado);
    if(estado==2) {
      estado=0;
    }
    else if(estado==0) {
      estado=2;
    }
  }
  M5.Lcd.setCursor(70,90);
  if(estado==2){
   M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(80,90);
    M5.Lcd.println("IMU");
    delay(100);
  }
  delay(20);
}


void BotonC(){
  if(M5.BtnC.wasPressed()){
    Serial.write(estado);    
    if(estado==3) {
      estado=0;
    }
    else if(estado==0) {
      estado=3;
    }
  }
  M5.Lcd.setCursor(30,90);
  if(estado==3){
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(70,90);
    M5.Lcd.println("Pista");
    delay(100);
  }
  delay(20);
}


void loop() {
  M5.update(); 
  switch (estado) {
    case 1:
      BotonA();
      break;
    case 2:
      BotonB();
      break;
    case 3:
      BotonC();
      break;
    default:
      M5.Lcd.clear(BLACK);
      BotonA();
      BotonB();
      BotonC();
      break;
}
}

