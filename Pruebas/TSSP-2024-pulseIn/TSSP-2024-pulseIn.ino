#define NUM_TSSP 16
int tssp[NUM_TSSP]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,18,19};
double vecx[NUM_TSSP]={-0.3826, -0.7071, -0.9238, -1, -0.9238, -0.7071, -0.3826, 0, 0.3826, 0.7071, 0.9238, 1, 0.9238, 0.7071, 0.3826, 0};
double vecy[NUM_TSSP]={-0.9238, -0.7071, -0.3826, 0, 0.3826, 0.7071, 0.9238, 1, 0.9238, 0.7071, 0.3826, 0, -0.3826, -0.7071, -0.9238,-1};
int data[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double maxX[6] = {0,0,0,0,0};
double maxY[6] = {0,0,0,0,0};
int values[5] = {0,0,0,0,0};
double mag[5]= {0,0,0,0,0};
bool check = 1;
double angle;

void getVector(){
  double resX, resY;

  for(int i=0;i<5;i++){
    maxY[i] = maxY[i] * values[i];
    maxX[i] = maxX[i] * values[i];
  }
  resX = (maxX[0] * 1.5) + (maxX[1]) + (maxX[2] * 0.8) + (maxX[3] * 0.5) + (maxX[4] * 0.1);
  resY = (maxY[0] * 1.5) + (maxY[1]) + (maxY[2]) * 0.8 + (maxY[3] * 0.5) + (maxY[4] * 0.1);
  /*resX = maxX[0] + maxX[1] + maxX[2] + maxX[3] + maxX[4];
  resY = maxY[0] + maxY[1] + maxY[2] + maxY[3] + maxY[4];
*/
  angle = atan2(resY,resX);
  
}


void getMaxValues(){
  int max = 0, max1 = 0, max2 = 0, max3 = 0, max4 = 0;

  for(int i = 0;i<16;i++){
    if(max < data[i]){
      max = data[i];
      maxX[0] = vecx[i];
      maxY[0] = vecy[i];
      //Serial.print("max");
    }
    else if(max1 < data[i]){
      max1 = data[i];
      maxX[1] = vecx[i];
      maxY[1] = vecy[i];
      //Serial.print("max1");
    }
    else if(max2 < data[i]){
      max2 = data[i];
      maxX[2] = vecx[i];
      maxY[2] = vecy[i];
      //Serial.print("max2");
    }
    else if(max3 < data[i]){
      max3 = data[i];
      maxX[3] = vecx[i];
      maxY[3] = vecy[i];
    //  Serial.print("max3");
    }
    else if(max4 < data[i]){
      max4 = data[i];
      maxX[4] = vecx[i];
      maxY[4] = vecy[i];
      //Serial.print("max4");
    }
    
   if(max1 < data[i]){
      max1 = data[i];
      maxX[1] = vecx[i];
      maxY[1] = vecy[i];
      //Serial.print("max1");
    }
    else if(max2 < data[i]){
      max2 = data[i];
      maxX[2] = vecx[i];
      maxY[2] = vecy[i];
      //Serial.print("max2");
    }
    else if(max3 < data[i]){
      max3 = data[i];
      maxX[3] = vecx[i];
      maxY[3] = vecy[i];
    //  Serial.print("max3");
    }
    else if(max4 < data[i]){
      max4 = data[i];
      maxX[4] = vecx[i];
      maxY[4] = vecy[i];
      //Serial.print("max4");
    }
/*  Serial.print(data[i]);
  Serial.print(" ");
*/
  }
  values[0] = max;
  values[1] = max1;
  values[2] = max2;
  values[3] = max3;
  values[4] = max4; 
}

void filter(){
      for(int i=0;i<16;i++){
       if(data[i] < 100){
          check = 0;
          break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(230400);
  for(int i = 0;i<20;i++){
    pinMode(i, INPUT);
  }
}

void loop() {
  int a;
  int state = HIGH;
  check = 1;
  for(int i = 0;i<16;i++){
    /*state = LOW;
    if(state == HIGH){
      state == digitalRead(tssp[i]);
    }*/
    a = pulseIn(tssp[i],HIGH);
    data[i] = a;
    Serial.print(a);
    Serial.print(" ");
  }
   Serial.println(" ");
  filter();

  if(check == 1){
    getMaxValues();
    for(int i = 0;i<5;i++){
      Serial.print(maxX[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
    for(int i = 0;i<5;i++){
      Serial.print(maxY[i]);
      Serial.print(" ");
    }
    Serial.println(" ");
    for(int i = 0;i<5;i++){
      Serial.print(values[i]);
      Serial.print(" ");
    }

  getVector();
  angle = angle * 57.2957;
  angle = angle + 180;
  Serial.println(" ");
  Serial.print(angle);
  Serial.println("");
  delay(1000);
  }
}
