#define NUM_TSSP 16
int tssp[NUM_TSSP]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,18,19};
double vecx[NUM_TSSP]={-0.3826, -0.7071, -0.9238, -1, -0.9238, -0.7071, -0.3826, 0, 0.3826, 0.7071, 0.9238, 1, 0.9238, 0.7071, 0.3826, 0};
double vecy[NUM_TSSP]={-0.9238, -0.7071, -0.3826, 0, 0.3826, 0.7071, 0.9238, 1, 0.9238, 0.7071, 0.3826, 0, -0.3826, -0.7071, -0.9238,-1};
int data[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double maxX[6] = {0,0,0,0,0};
double maxY[6] = {0,0,0,0,0};
int values[5] = {0,0,0,0,0};
double mag[5]= {0,0,0,0,0};
int tsspNum[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
bool check = 1;
double angle;

void getVector(){
  double resX, resY;

  for(int i=0;i<5;i++){
    maxY[i] = maxY[i] * values[i];
    maxX[i] = maxX[i] * values[i];
  }
  //resX = (maxX[0] * 0.5) + (maxX[1] * 1) + (maxX[2] * 1.5) + (maxX[3] * 1) + (maxX[4] * 0.5);
  //resY = (maxY[0] * 0.5) + (maxY[1] * 1) + (maxY[2] * 1.5) + (maxY[3] * 1) + (maxY[4] * 0.5);
  resX = maxX[0] + maxX[1] + maxX[2] + maxX[3] + maxX[4];
  resY = maxY[0] + maxY[1] + maxY[2] + maxY[3] + maxY[4];

  angle = atan2(resY,resX);
  
}


void getMaxValues(){
  int max = 0, max1 = 0, max2 = 0, max3 = 0, max4 = 0, num = -1, tssp0, tssp1, tssp2, tssp3, tssp4;

  for(int i = 0;i<16;i++){
    if(max < data[i]){
      max = data[i];
      maxX[0] = vecx[i];
      maxY[0] = vecy[i]; 
      num = tsspNum[i];
      //Serial.print("max");
    }
  }
    tssp0 = num - 2;
      if(tssp0 < 0){
        tssp0 = tssp0 + 15;
      }
      else if(tssp0 >15){
        tssp0 = tssp0 - 16;
      }
    tssp1 = num - 1;
      if(tssp1 < 0){
        tssp1 = tssp0 + 15;
      }
      else if(tssp1 >15){
        tssp0 = tssp0 - 16;
      }
    tssp2 = num;
      if(tssp2 < 0){
        tssp2 = tssp0 + 15;
      }
      else if(tssp2 >15){
        tssp2 = tssp0 - 16;
      }
    tssp3 = num + 1;
      if(tssp3 < 0){
        tssp3 = tssp3 + 15;
      }
      else if(tssp3 >15){
        tssp3 = tssp3 - 16;
      }
    tssp4 = num + 2;
      if(tssp4 < 0){
        tssp4 = tssp4 + 15;
      }
      else if(tssp4 >15){
        tssp4 = tssp4 - 16;
      }

    


    values[0] = data[tssp0];
    values[1] = data[tssp1];
    values[2] = data[tssp2];
    values[3] = data[tssp3];
    values[4] = data[tssp4];

    maxX[0] = vecx[tssp0];
    maxX[1] = vecx[tssp1];
    maxX[2] = vecx[tssp2];
    maxX[3] = vecx[tssp3];
    maxX[4] = vecx[tssp4];

    maxY[0] = vecy[tssp0];
    maxY[1] = vecy[tssp1];
    maxY[2] = vecy[tssp2];
    maxY[3] = vecy[tssp3];
    maxY[4] = vecy[tssp4];
    /*
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
      maxY[4] = vecy[i];*/
      //Serial.print("max4");
   // }
/*  Serial.print(data[i]);
  Serial.print(" ");
*/
  
}

void filter(){
      for(int i=0;i<16;i++){
       if(data[i] < 1){
          check = 0;
          break;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for(int i = 0;i<20;i++){
    pinMode(i, INPUT);
  }
}

void loop() {
  int a;
  int state = HIGH;
  double x1=0;
  double y1=0;
  int counter=0;
  double angle;
  check = 1;
  for(int i = 0;i<16;i++){
    /*state = LOW;
    if(state == HIGH){
      state == digitalRead(tssp[i]);
    }*/
    a = pulseIn(tssp[i],HIGH, 830);
    
    data[i] = a;
    Serial.print(a);
    Serial.print(" ");
  } 

  Serial.println();
  for (int i=0; i<NUM_TSSP;  i++){
    //Si algun tssp marca lectura se hace la suma del vector registrado
    
      x1=x1+(data[i]*vecx[i]);
      y1=y1+(data[i]*vecy[i]);
      counter++;
    
  }
  counter !=0 ? angle=atan2(x1,y1)/M_PI*180.00 : angle= -1;
  //Serial.println(angle);
  delay(10);
}
