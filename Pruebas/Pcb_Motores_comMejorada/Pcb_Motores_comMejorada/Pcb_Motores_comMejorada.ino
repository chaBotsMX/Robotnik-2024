#include<elapsedMillis.h>
#include <PID_v1.h>
#define pi 3.14159265358

elapsedMillis uartMillis;
int angle = 400;
int M1D1 = 0, M1D2 = 1, M2D1 = 3, M2D2 = 2, M3D1 = 5, M3D2 = 4, M4D1 = 6, M4D2 = 7; //pines de los motores
double dir1, dir2, dir3, dir4, pow1, pow2, pow3, pow4, vel = 100; //variables usadas para calcular pwm de cada motor
double IMUM; //offset del imu
int wait = 0;

const byte startMarker = 0xFF;
const byte endMarker = 0xFE;
const size_t packetSize = 7; // Actualizado para incluir el nuevo byte de ángulo

// Variables para almacenar los datos recibidos
int receivedPWM = 0;
int receivedOrientationError = 0;
int receivedMovementIndicator = 0;
int receivedAngle = 0; // Nuevo

bool newData = false;

double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,1,5,1,P_ON_M, DIRECT)


void processReceivedData() {
  // Asegúrate de realizar este cálculo rápidamente para no retrasar el loop
  angle = receivedAngle * 2;
  IMUM = receivedOrientationError * 2; // Ajustar según necesidades reales
  wait = receivedMovementIndicator;
  vel = receivedPWM;

  // Ajuste de IMUM a un rango adecuado
  if (IMUM >= 180) {
    IMUM = IMUM - 360;
  }

  // Restablecer el temporizador para indicar la recepción de datos nuevos
  uartMillis = 0;

  // Aquí podrías llamar a funciones para mover los motores basado en los nuevos datos
  if (newData && angle < 360) {
    Input = IMUM;
    mover(angle); // Asumiendo que mover() ajusta los motores basándose en `angle` y otros datos globales
    newData = false; // Asegúrate de restablecer `newData`
  }
  else{
    newData = false;
    stop(10);  
  }
}

void receiveData() {
  static byte rcvPacket[packetSize];
  static unsigned int bytesRcvd = 0;
  byte rc;

  // Procesar todos los datos disponibles en el buffer de manera eficiente
  while (Serial1.available() > 0 && !newData) {
    rc = Serial1.read();

    // Capturar el marcador de inicio para comenzar a llenar el paquete
    if (bytesRcvd == 0 && rc != startMarker) {
      continue; // Si no es el marcador de inicio, sigue escuchando
    }

    rcvPacket[bytesRcvd++] = rc;

    if (bytesRcvd >= packetSize) {
      if (rcvPacket[0] == startMarker && rcvPacket[packetSize - 1] == endMarker) {
        // Calcular el checksum
        byte checksum = 0;
        for (unsigned int i = 1; i < packetSize - 2; i++) {
          checksum += rcvPacket[i];
        }

        if (checksum == rcvPacket[packetSize - 2]) {
          // Desempaquetar los datos
          receivedPWM = rcvPacket[1];
          receivedOrientationError = rcvPacket[2];
          receivedMovementIndicator = rcvPacket[3];
          receivedAngle = rcvPacket[4];
          newData = true;
          
          // Interpretar y ajustar los datos recibidos aquí si es necesario
          processReceivedData();
        } else {
          Serial.println(F("Error en checksum"));
        }
      } else {
        Serial.println(F("Error en paquete"));
      }
      bytesRcvd = 0; // Restablecer para el próximo paquete
    }
  }
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

  Serial.print(angle);
  Serial.print(" ");
  Serial.println(IMUM);


  if(angle != 400){
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

  myPID.Compute();

  pow1=pow1 + Output;
  pow2=pow2 - Output;
  pow3=pow3 - Output;
  pow4=pow4 + Output;

 

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

  Setpoint = 0;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

}

void loop() {
  receiveData(); // Continuamente verifica y procesa datos nuevos

  // El resto de tu lógica de control aquí
  if (uartMillis < 100 && newData) { // Asegúrate de que newData sea usado correctamente
    // mover() se llama desde processReceivedData() cuando newData es true
  } else if (uartMillis >= 100) {
    stop(100); // Detiene los motores si no hay datos nuevos en 100 ms
  }

  Serial.print(angle);
  Serial.print(" ");
  Serial.print(Output);
  Serial.print(" ");
  Serial.println(IMUM);
  

}  

