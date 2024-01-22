#include <M5Stack.h>

int estado=1;

void setup() {
    M5.begin();        // Init M5Core. 
    M5.Power.begin();  // Init Power module.  
    M5.Lcd.setTextColor(GREEN);  // Set the font color to yellow. 
    M5.Lcd.setTextSize(4);     // Set the font size. 
    Serial.begin(9600);
}

void loop() {
  M5.update();    //You need to add M5.update () to read the status of the keystroke. For details, please s((ee System. 
  if(M5.BtnA.wasPressed()){
    Serial.println(estado);
    if(estado==1) {
      estado=0;
    }
    else if(estado==0) {
      estado=1;
    }
  }

  M5.Lcd.setCursor(70,90);
  if (estado==0) {    //If the key is pressed. 
    M5.Lcd.clear(BLACK);  
    M5.Lcd.println("Jugando");
    delay(100);
  }
  else if(estado==1){
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(50,90);
    M5.Lcd.println("Jugandon't");
    delay(100);
  }
  delay(20);
}