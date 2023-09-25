int M1D1 = 0, M1D2 = 1, M2D1 = 2 , M2D2 = 3, M3D1 = 4, M3D2 = 5, M4D1 = 6, M4D2 = 7;
double dir1, dir2, dir3, dir4, pow1, pow2, pow3, pow4, vel=30;
int IMUM;


void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode( M1D1 , OUTPUT );
  pinMode( M1D2 , OUTPUT );
  pinMode( M2D1 , OUTPUT );
  pinMode( M2D2 , OUTPUT );
  pinMode( M3D1 , OUTPUT );
  pinMode( M3D2 , OUTPUT );
  pinMode( M4D1 , OUTPUT );
  pinMode( M4D2 , OUTPUT );


}

void loop() {
  analogWrite(M1D1, 60);
  analogWrite(M1D2, 0);
  delay(1500);
  analogWrite(M2D1, 60);
  analogWrite(M2D2, 0);
  delay(1500);
  analogWrite(M3D1, 60);
  analogWrite(M3D2,0);
  delay(1500);
  analogWrite(M4D1, 100);
  analogWrite(M4D2, 0);
  delay(1500);
  
}
