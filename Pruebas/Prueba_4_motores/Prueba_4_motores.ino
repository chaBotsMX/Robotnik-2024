int M1D1 = 0, M1D2 = 1;
double dir1, dir2, dir3, dir4, pow1, pow2, pow3, pow4, vel=30;
int IMUM;


void setup() {

  Serial.begin(115200);
  
  
  pinMode( M1D1 , OUTPUT );
  pinMode( M1D2 , OUTPUT );
  delay(1000);


}

void loop() {
  
  analogWrite(M1D2, 255);
  analogWrite(M1D1, 0);

}
