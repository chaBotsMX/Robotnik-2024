#include <M5Stack.h>

int estado=0;
float xLinea;
float yLinea;
float angleLinea;


void setup() {
    M5.begin();        
    M5.Power.begin();   
    M5.Lcd.setTextColor(GREEN);  
    M5.Lcd.setTextSize(5);
    M5.Lcd.println("ocelot");
    delay(250);     
    Serial.begin(115200);
}

void BotonA(){
  if(M5.BtnA.wasPressed()){
    if(estado==1) {
      estado=0;
    }
    else if(estado==0) {
      estado=1;
    }
  }
  Serial.write(estado);
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
    if(estado==2) {
      estado=0;
    }
    else if(estado==0) {
      estado=2;
    }
  }
  Serial.write(estado);
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
    if(estado==3) {
      Serial.write(254);
      estado=0;
    }
    else if(estado==0) {
      Serial.write(estado);
      estado=3;
    }
  }
  
  //delay(1);
  M5.Lcd.setCursor(30,90);
  if(estado==3){
    M5.Lcd.clear(BLACK);
    if(Serial.available()){
      angleLinea = Serial.read();
      angleLinea=angleLinea*2;
      //Serial.println(angleLinea);
      M5.Lcd.println(angleLinea);  
    }   
    xLinea=cos((angleLinea-90)*3.1416/180);
    yLinea=sin((angleLinea-90)*3.1416/180);
    M5.Lcd.drawCircle(160, 120, 100, GREEN);  // Draw a red circle of radius 50 at (x,y)
    M5.Lcd.drawLine(160, 120, xLinea*100+160, yLinea*100+120, GREEN);
  }
}


void loop() {
  M5.update(); 
  /*if(Serial.available()){
    angleLinea = Serial.read();
    angleLinea=angleLinea*2;
  }*/
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

