#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>
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
int angulos [13]={
  0, 30, 60, 90, 120, 150, 180, -30, -60, -90, -120, -150, -180
};

int danger = 0;
int angle = 0 , flag;
int objetivo1, objetivo2;

double cosx [12]={                                                                                                                  
  -1.0,
  -0.8660254037844387,
  -0.5000000000000001,
  -6.123233995736766e-17,
  0.4999999999999998,
  0.8660254037844387,
  1.0,
  0.8660254037844387,
  0.5000000000000001,
  6.123233995736766e-17,
  -0.4999999999999998,
  -0.8660254037844387
};

double sinx [12]={
  0.0,
  0.49999999999999994
  ,0.8660254037844386,
  1.0,
  0.8660254037844387,
  0.49999999999999994, 
  0.0,
  -0.49999999999999994,
  -0.8660254037844386,
  -1.0,
  -0.8660254037844387,
  -0.49999999999999994
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



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 16, 17);
  delay(5000);
  xTaskCreatePinnedToCore(loop2, "loop2", 4096, NULL, 0, NULL, 0);
  pixels.begin(); 
  pixels.clear(); 
  for(int i=0; i<NUMPIXELS; i++) { 
    pixels.setPixelColor(i, pixels.Color(200, 0, 0));
    pixels.show();
  }
  analogReadResolution(10);
}

void loop() {
  float x1 = 0;
  float y1 = 0;
  flag = 0;
  for(int i=0; i<13; i++){
    int analog = analogRead(pines[i]);
    //Serial.print(analog);
    //Serial.print("\t");
    if(i != 12 && analog > 150){
      flag = 1;
      x1 = x1 + (sinx[i] );
      y1 = y1 + (cosx[i] );


    }
  }
  //sensores objetivos s14 y s7
  //                   A14   A7
  if(analogRead(A14) > 150){
    objetivo1 = 1;
  }
  if(analogRead(A7) > 150){
    objetivo2 = 1;
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
  if(objetivo1 == 1 && objetivo2 == 1){
    lastAngle = 500;
    writing = 0;
  }  
  else if(objetivo1 == 0 && objetivo2 == 0 && flag == 1){
    lastAngle = angle;
    writing = 0;
  }

  if(writing <= 50 ){
    Serial1.write(lastAngle/2);

  }

  else{
    Serial1.write(200);
    Serial.println("cancha");
  }

  if(flagcounter >= 50)
  {
    danger = 0;
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


  uartTimer = 0;

}

void loop2 (void* pvParameters){
  while (1){
 int especial1, especial2, especial3;
    especial1 = analogRead(A10);
    especial2 = analogRead(A18);
    especial3 = analogRead(A12);
    //Serial.println(especial3);
    if(especial1 >150 || especial2 > 150){
      flag = 1;
      danger = 1;
      flagcounter = 0;
    }
    else{
      danger = 0;
    }
  
  }
}