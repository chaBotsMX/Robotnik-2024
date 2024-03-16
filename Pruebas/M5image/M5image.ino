#include "M5Stack.c"
#include <M5Stack.h>

void setup() {
  M5.begin();        
  M5.Power.begin();
  M5.Lcd.drawBitmap(0,0,320,240,(uint8_t *)M5Stack_map);
}

void loop() {

}
