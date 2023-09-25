#define NUM_TSSP 17
int tssp[NUM_TSSP]={2,3,4,5,6,7,8,9,10,11,12,13,14,15,18,19};
double vecx[NUM_TSSP]={-0.3826, -0.7071, -0.9238, -1, -0.9238, -0.7071, -0.3826, 0, 0.3826, 0.7071, 0.9238, 1, 0.9238, 0,7071, 0.3826, 0};
double vecy[NUM_TSSP]={-0.9238, -0.7071, -0.3826, 0, 0.3826, 0.7071, 0.9238, 1, 0.9238, 0.7071, 0.3826, 0, -0.3826, -0.7071, -0.9238,-1};
int data[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
double maxX[6] = {0,0,0,0,0};
double maxY[6] = {0,0,0,0,0};
int values[5] = {0,0,0,0,0};
bool check = 1;

void getMaxValues(){
  int max = 0, max1 = 0, max2 = 0, max3 = 0, max4 = 0;


  for(int i = 0;i<16;i++){
    if(max < data[i]){
      max4 = max3;
      maxX[4] = maxX[3];
      maxY[4] = maxY[3];

      max3 = max2;
      maxX[3] = maxX[2];
      maxY[3] = maxY[2];

      max1 = max; 
      maxX[2] = maxX[1];
      maxY[2] = maxY[1];

      max = data[i];
      maxX[0] = vecx[i];
      maxY[0] = vecy[i];
    }
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
  Serial.begin(115200);
  for(int i = 0;i<16;i++){
    pinMode(i, INPUT);
  }
}

void loop() {
  int a;
  int state = HIGH;
  check = 1;
  for(int i = 0;i<16;i++){
    state = LOW;
    if(state == HIGH){
      state == digitalRead(tssp[i]);
    }
    a = pulseIn(tssp[i],HIGH);
    data[i] = a;
  }

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
  Serial.println("");
  }
}
