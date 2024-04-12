#include <Adafruit_NeoPixel.h>
#include<elapsedMillis.h>
#define PIN        5
#define NUMPIXELS 15

const size_t dataLength = 2;
int data[dataLength]; 
int especiales[2] = {10,18};
int side = 0;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int lastAngle = 0;
int inicial = 0;
int pwm = 1;
elapsedMillis writing;
elapsedMillis flagcounter;
elapsedMillis uartTimer;
int angulos[13]={
  0, 30, 60, 90, 120, 150, 180, -30, -60, -90, -120, -150, -180
};
int calValues[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
int danger = 0;
int angle = 0 , flag;

int instruction  =1;
double cosx [12]={                                                                                                                  
  -1.0,
  -0.8660,
  -0.5,
  -0,
  0.5,
  0.8660,
  1.0,
  0.8660,
  0.5,
  0,
  -0.5,
  -0.8660
};

double sinx [12]={
  0.0,
  0.5,
  0.8660,
  1.0,
  0.8660,
  0.5, 
  0.0,
  -0.5,
  -0.8660,
  -1.0,
  -0.8660,
  -0.5
};

int pines [12] = {
  A12, //s2
  A5, //s9
  A4, //s8
  A7, //s7
  A6, //s6
  A3, //s5
  A0,
  A17, //
  A16, 
  A14, 
  A19, 
  A13
};
//vn 39
// vp 36
//S2 S9   s8   s7    s6   s5 s4 s12 s13 s14 s10 s3 
//a12  



void calibrar(){
  for (int i = 0; i< 13; i++){
    int analog = analogRead(pines[i]);
    calValues[i] = analog + 30;
  }
  delay(300);
    for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(0, 200, 0));
    pixels.show();
  }
  delay(300);
    for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(200, 0, 0));
    pixels.show();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);
  pixels.begin(); 
  pixels.clear(); 
  for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(200, 0, 0));
    pixels.show();
  }
  analogReadResolution(10);
  xTaskCreatePinnedToCore(loop2, "loop2", 4096, NULL, 0, NULL, 0);
}

void loop() {
  float x1 = 0;
  float y1 = 0;
  flag = 0;
 if (instruction == 1){
      calibrar();
      instruction = 0;
  }

  for(int i=0; i<13; i++){
    int analog = analogRead(pines[i]);
    //Serial.print(analog);
    //Serial.print("\t");
    if(i != 12 && analog > calValues[i]){
      flag = 1;
      x1 = x1 + (sinx[i] );
      y1 = y1 + (cosx[i] );


    }
  }
  /*
  Serial.print(x1);
  Serial.print("\t");

  Serial.print(y1);
  Serial.println("\t");
  */
  angle = atan2(x1, y1)*180/M_PI;

  if(angle < 0 ){
    angle = 360 + angle;
  }
  //Serial.println(angle);  
  //Serial.println(flag);
  if (danger != 1){
  if(flag == 1){
    writing = 0;
    if(side == 0){
    lastAngle = angle;
    side = 1; 
    inicial = angle;
    flagcounter = 0;
    }
    else if(flag == 1 && side == 1){
      if(inicial - angle >= 90 || inicial - angle <= -90){
        lastAngle = angle;
        flagcounter = 0;
 
      }
      else{
        angle = lastAngle;
        flagcounter = 0;
      }
    }

  }
  }
  else {
    if(side == 0){
    lastAngle = 180;
    inicial = 180;
    flagcounter = 0;
    side = 1;
    }
    else{
      if(inicial <= 211 && inicial >= 149){
        lastAngle = 180;
        inicial = 180;
        flagcounter = 0;
        side = 1;
      }
      else{
        flagcounter = 0;
        side = 1;
      }
    }
  }
  if(writing <= 20){
    Serial1.write(lastAngle/2);

  }
  else if(danger == 1){
    Serial1.write(lastAngle/2);
  }
  else{
    Serial1.write(200);
    Serial.println("cancha");
  }

  if(flagcounter >= 50)
  {

    side = 0;
  }

  Serial.print(lastAngle);
  Serial.print(" ");
  Serial.print(side);
  Serial.print(" ");
  Serial.print(angle);
  Serial.print(" ");
  Serial.println(inicial);
  //Serial.println(xPortGetCoreID());*/

  delay(100);
  uartTimer = 0;

}

void loop2 (void* pvParameters){
  while (1){
  elapsedMillis peligro;
  int especial1, especial2, especial3;
    especial1 = analogRead(A10);
    especial2 = analogRead(A18);
    especial3 = analogRead(A12);
    //Serial.println(especial3);
    if(especial1 >150 || especial2 > 150 || especial3 > 150){
      danger = 1;
      
      Serial.print("peligro");
    }
    else{
      danger = 0;
    }
  }
}