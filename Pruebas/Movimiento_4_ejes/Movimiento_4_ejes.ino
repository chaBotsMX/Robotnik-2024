#include <Wire.h>
//#include "IRLocator360.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>

#define pi 3.14159265358

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
//IRLocator360 IR;

int M1D1 = 0, M1D2 = 1, M2D1 = 2 , M2D2 = 3, M3D1 = 4, M3D2 = 5, M4D1 = 6, M4D2 = 7;
double dir1, dir2, dir3, dir4, pow1, pow2, pow3, pow4, vel=70;
int IMUM;


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
  dir1=angle-30;
  dir2=angle-150;
  dir3=angle+30;
  dir4=angle+150;

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

 
  if(angle != -1){
    IMUM = IMUM*130/180;
  }
  else{
    IMUM = IMUM*360/90;
  }

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

  Serial.begin(115200);
  Serial1.begin(74880);
  Wire.begin();
  //IR.sensorInitialization();  
  bno.begin();
  delay(100);

  analogWriteFreq(50000);
  pinMode( M1D1 , OUTPUT );
  pinMode( M1D2 , OUTPUT );
  pinMode( M2D1 , OUTPUT );
  pinMode( M2D2 , OUTPUT );
  pinMode( M3D1 , OUTPUT );
  pinMode( M3D2 , OUTPUT );
  pinMode( M4D1 , OUTPUT );
  pinMode( M4D2 , OUTPUT );
  analogWrite(M1D1, 0);
  analogWrite(M1D2, 10);
  analogWrite(M2D1, 0);
  analogWrite(M2D2, 10);
  analogWrite(M3D1, 0);
  analogWrite(M3D2, 10);
  analogWrite(M4D1, 0);
  analogWrite(M4D2, 10);
  delay(1500);


}

void loop() {
  mover(-1);
  //int pelota = IR.angleDirection1200hz();
  sensors_event_t event;
  bno.getEvent(&event);
  IMUM = event.orientation.x;
  IMUM > 180 ? IMUM = IMUM - 360 : IMUM = IMUM;
  Serial.print(IMUM);
  Serial.print(" ");
  /*magn=mag();
  double erro = getRawRotation() - Set;
  erro < 0 ? erro= 360.00+erro : erro=erro;
  float IMUM=error(erro);
  Serial.println(IMUM);*/

  /*motor1(1, 100);
  motor2(1, 100);
  motor3(1, 100);
  delay(1000);
  stop( 100 );*/

  
  /*delay(1000);
  stop(10);
  mover(180);
  delay(1000);
  stop(10);
  mover(150);
  delay(1000);
  stop(10);
  mover(270);
  delay(1000);  
  stop(10);
  mover(60);
  delay(1000);*/
 // stop(9000);

}
