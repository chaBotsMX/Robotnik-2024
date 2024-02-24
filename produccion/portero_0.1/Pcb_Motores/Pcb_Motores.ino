#include<elapsedMillis.h>
#define pi 3.14159265358

elapsedMillis uartMillis;
int angle;
int M1D1 = 0, M1D2 = 1, M2D1 = 3, M2D2 = 2, M3D1 = 5, M3D2 = 4, M4D1 = 6, M4D2 = 7; //pines de los motores
double dir1, dir2, dir3, dir4, pow1, pow2, pow3, pow4, vel = 125; //variables usadas para calcular pwm de cada motor
double IMUM; //offset del imu
int wait = 0;
const size_t dataLength = 5;
int data[5] = {0,0,0,0,0};


//variables del pid
unsigned long lastTime;
double Input, Output, SetPoint;
double errSum, LastErr;
double kp, ki, kd;

void compute()
{ 
  // tiempo desde el ultimo calculo
  unsigned long now = millis();
  double timeChange = (double)(now - lastTime);

  //calcular error
  double error = IMUM;
  errSum += (error * timeChange);
  double dErr = (error - LastErr) / timeChange;
  //calcular output
  Output = kp * error + ki * errSum + kd * dErr;
  //guardar valores para siguientes calculos
  LastErr= error;
  lastTime = now;
}

void setTunings(double Kp, double Ki, double Kd)
{
  kp = Kp;
  ki = Ki;
  kd = Kd;
}

void motor1( int dir, int pow){      ////////// mover los motores indicando direcciÃ³n (1 fwd, -1 back) y cuanto poder asignarle

    if( dir == 1){

      analogWrite(M1D1, pow);
      analogWrite(M1D2, 1);

    }

    if( dir == -1){

      analogWrite(M1D1, 1);
      analogWrite(M1D2, pow);

    }

    if( dir == 0){

      
      analogWrite(M1D1, pow);
      analogWrite(M1D2, pow);


    }

}

void motor2( int dir, int pow){

    if( dir == 1){

      
      analogWrite(M2D1, pow);
      analogWrite(M2D2, 1);

    }

    if( dir == -1){
      analogWrite(M2D1, 1);
      analogWrite(M2D2, pow);

    }

    if( dir == 0){

      analogWrite(M2D1, pow);
      analogWrite(M2D2, pow);


    }

}

void motor3( int dir, int pow){

    if( dir == 1){

      
      analogWrite(M3D1, pow);
      analogWrite(M3D2, 1);

    }

    if( dir == -1){

      analogWrite(M3D1, 1);
      analogWrite(M3D2, pow);

    }

    if( dir == 0){

      
      analogWrite(M3D1, pow);
      analogWrite(M3D2, pow);


    }

}

void motor4( int dir, int pow){

    if( dir == 1){

      
      analogWrite(M4D1, pow);
      analogWrite(M4D2, 1);

    }

    if( dir == -1){

      analogWrite(M4D1, 1);
      analogWrite(M4D2, pow);

    }

    if( dir == 0){

      
      analogWrite(M4D1, pow);
      analogWrite(M4D2, pow);


    }

}

void motores(int num, int pot){
    
  if( num == 1){

    if( pot > 0){


      analogWrite(M1D1, pot);
      analogWrite(M1D2, 1);

    }
    else{

      analogWrite(M1D1, 1);
      analogWrite(M1D2, -pot);
      
    }
        
  }

  if( num == 2){

    if( pot > 0){

      analogWrite(M2D1, pot);
      analogWrite(M2D2, 1);

    }
    else{

      analogWrite(M2D1, 1);
      analogWrite(M2D2, -pot);
      
    }
        
  }

  if( num == 3){

    if( pot > 0){

      
      analogWrite(M3D1, pot);
      analogWrite(M3D2, 1);

    }
    else{

      analogWrite(M3D1, 1);
      analogWrite(M3D2, -pot);
      
    }
        
  }

  if( num == 4){

    if( pot > 0){

      
      analogWrite(M4D1, pot);
      analogWrite(M4D2, 1);

    }
    else{

      analogWrite(M4D1, 1);
      analogWrite(M4D2, -pot);
      
    }
        
  }
  
    
}

void mover ( int angle){      ///////////Funcion para calcular en que direccion moverte con omnidireccional (angulo y tiempo)

  if(angle != -1){
  dir1=angle+45;
  dir2=angle+135;
  dir3=angle+45;
  dir4=angle+135;

  dir1=dir1*pi/180;
  dir2=dir2*pi/180;
  dir3=dir3*pi/180;
  dir4=dir4*pi/180;

  pow1=vel*cos(dir1);
  pow2=vel*cos(dir2);
  pow3=vel*cos(dir3);
  pow4=vel*cos(dir4);
  }
  else{
    pow1 = 0;  
    pow2 = 0;
    pow3 = 0;
    pow4 = 0;
    }
  //Serial.println(" ");

 // Serial.println( IMUM );

  IMUM = IMUM * 1.05;

  pow1=pow1 + IMUM;
  pow2=pow2 - IMUM;
  pow3=pow3 - IMUM;
  pow4=pow4 + IMUM;

 

  Serial.print(pow1);
  Serial.print(" ");
  Serial.print(pow2);
  Serial.print(" ");
  Serial.print(pow3);
  Serial.print(" ");
  Serial.print(pow4);
  Serial.println("\n");
  
  if( pow1 < 0 ){

    pow1=min( abs(pow1), vel);
    motores(1, -pow1);
    
  }
  else{

    pow1=min( abs(pow1), vel);
    motores(1, pow1);    
    
  }

  if( pow2 < 0 ){

    pow2=min( abs(pow2), vel);
    motores(2, -pow2);
    
  }
  else{

    pow2=min( abs(pow2), vel);
    motores(2, pow2);    
    
  }

  if( pow3 < 0 ){

    pow3=min( abs(pow3), vel);
    motores(3, -pow3);
    
  }
  else{

    pow3=min( abs(pow3), vel);
    motores(3, pow3);    
    
  }
   if( pow4 < 0 ){

    pow3=min( abs(pow4), vel);
    motores(4, -pow3);
    
  }
  else{

    pow3=min( abs(pow4), vel);
    motores(4, pow4);    
    
  }


}


void stop ( int t){                  ///////////Frenar todos los motores (tiempo)

  motor1( 0, 200);
  motor2( 0, 200);
  motor3( 0, 200);
  motor4( 0, 200);
  

}


void setup() {

  Serial.begin(115200); //comunicacion pc
  Serial1.begin(115200); // comunicacion con placa principal
  // tiempo para que los uart se inicialicen de forma correcta y se sincronicen
  analogWriteResolution(8); //resolucion del pwm, 8 bytes, 0 a 255 
  analogWriteFreq(25000); //frecuencia del pwm, idealmente ams de 20,000 pero no 20,0000
  pinMode( M1D1 , OUTPUT );
  pinMode( M1D2 , OUTPUT );
  pinMode( M2D1 , OUTPUT );
  pinMode( M2D2 , OUTPUT );
  pinMode( M3D1 , OUTPUT );
  pinMode( M3D2 , OUTPUT );
  pinMode( M4D1 , OUTPUT );
  pinMode( M4D2 , OUTPUT );
  pinMode(25 , OUTPUT);
  //se arman los motores, se prenden al 100% por poco tiempo
  analogWrite(M1D1, 0);
  analogWrite(M1D2, 255);
  analogWrite(M2D1, 0);
  analogWrite(M2D2, 255);
  analogWrite(M3D1, 0);
  analogWrite(M3D2, 255);
  analogWrite(M4D1, 0);
  analogWrite(M4D2, 255);
  delay(1);


}

void loop() {
  if(Serial1.available() >= dataLength * sizeof(data[0])) //si hay informacion nueva se lee y se mueve
  { 
    digitalWrite(25,HIGH); // se prende para debugear
    Serial1.readBytes((byte*)data, dataLength * sizeof(data[0]));
    angle = data[0] * 2; //se interpreta la informacion
    IMUM  = data[2] + data[3];
    wait = data[4];
    vel = data[1];
    if(IMUM >= 180) //se calcula el offset del imu 
    {
      IMUM = IMUM -360;
    }
    uartMillis = 0; // se reinicia el contador para saber que si hubo informacion nueva
  }
  else{
    digitalWrite(25,LOW); //apagamos led para debugear
  }
  if(uartMillis <100)// si hay informacion reciente se mueve al angulo de la informacion
  {
    if(angle >= 0 && angle <= 360 && wait == 0){
      mover(angle);
    }
    else{
      mover(-1);
    }

  }
  else
  { //si lleva mucho tiempo sin angulo nuevo significa que no se debe de mover o hubo un error
    stop(100);
  }
  Serial.print(angle);
  Serial.print(" ");
  Serial.println(IMUM);
  

}  

