#include <Wire.h>
#include <BohleBots_BNO055.h>
//declaramos el tamaño de los array a enviar, son 5, pero para organizar los declaramos cuando se necesiten
const size_t dataLength1 = 2;
const size_t dataLength5 = 5;

int stop = 0;
int angEsp = 200;
int data1[2] = {0,0}; //Angulo de la pelota IR partido en 2 bytes

int data5[5] = {0,0,0,0,0}; // byte 1 y 2 son para enviar el anguloIR de la pelota, 3 y 4 el offset del imu y el 5 para enviar el pwm maximo 

int pelota1 = 0, pelota2 = 0, orientacion1 = 0, orientacion2 = 0, anguloIR = 200; // declaramos los bytes para poder trabajar con ellos 

int IMU, norte; //varaibles usadas para los calculos del imu

int estado, intensidad = 0;

int calibracionImu;

int ceroFake;

int xOpenMV; int yOpenMV;
int communicationMV = 1;
enum State {
  WAIT_FOR_START,
  READ_ANGLE_HIGH,
  READ_ANGLE_LOW,
  READ_INTENSITY_HIGH,
  READ_INTENSITY_LOW,
  READ_CHECKSUM,
  WAIT_FOR_END
};

// Variables globales para almacenar el ángulo e intensidad
int globalAngle = 0;
int globalIntensity = 0;

// Estado inicial
State currentState = WAIT_FOR_START;

// Variable para calcular el checksum
byte calculatedChecksum = 0;


BNO bno;  //create bno from the Class BNO

elapsedMillis stopTimer;

elapsedMillis uart2timer;
void uartStart(){
  Serial.begin(115200); //comunicacion con pc
  delay(1000);
  Serial1.begin(115200); //anillo de ir
  delay(1000);
  Serial2.begin(115200); //M5
  delay(1000);
  Serial3.begin(115200, 0X00);//sensor de linea
  delay(1000);
  Serial4.begin(115200); //openMV
  delay(1000);
  Serial5.begin(115200); // comunicacion con placa de motores
  delay(1000); //tiempo para sincronizar

}
       
void getUartInfo(){

 if (Serial1.available() > 0) {
    byte incomingByte = Serial1.read();
    
    switch (currentState) {
      case WAIT_FOR_START:
        if (incomingByte == 255) {
          currentState = READ_ANGLE_HIGH;
          calculatedChecksum = 0; // Resetea el checksum al empezar un nuevo mensaje
        }
        break;

      case READ_ANGLE_HIGH:
        calculatedChecksum += incomingByte;
        currentState = READ_ANGLE_LOW;
        break;

      case READ_ANGLE_LOW:
        calculatedChecksum += incomingByte;
        currentState = READ_INTENSITY_HIGH;
        break;

      case READ_INTENSITY_HIGH:
        calculatedChecksum += incomingByte;
        currentState = READ_INTENSITY_LOW;
        break;

      case READ_INTENSITY_LOW:
        calculatedChecksum += incomingByte;
        currentState = READ_CHECKSUM;
        break;

      case READ_CHECKSUM:
        // Verifica el checksum
        if (calculatedChecksum == incomingByte) {
          currentState = WAIT_FOR_END;
        } else {
          // Checksum no coincide, intenta resincronizar
          Serial.println("Error en el checksum, intentando resincronizar...");
          currentState = WAIT_FOR_START;
        }
        break;

      case WAIT_FOR_END:
        if (incomingByte == 254) {
          // Mensaje completo y correcto, procesa los datos
          Serial.println("Mensaje recibido correctamente.");
          // Aquí iría la lógica para procesar los datos recibidos
        } else {
          // Fin de mensaje incorrecto, intenta resincronizar
          Serial.println("Fin de mensaje incorrecto, intentando resincronizar...");
        }
        currentState = WAIT_FOR_START; // Vuelve al inicio para recibir un nuevo mensaje
        break;
    }
  }


  if (Serial2.available())
  {
    estado = Serial2.read();
    //Serial.println(estado);
  }
  if(Serial3.available())
  {
    angEsp = Serial3.read();
  }

  openMVSetup();

}

void openMVSetup(){
  if(communicationMV==1){
    Serial4.write(1);
    communicationMV=3;
  }
  else if(communicationMV==2){
    Serial4.write(2);
    communicationMV=4;
  }
  if (communicationMV==3){
    int entrante=Serial4.read();
    if (entrante!=-1){
      xOpenMV=entrante;
      communicationMV=2;
    }
  }
  if (communicationMV==4){
    int entrante=Serial4.read();
    if (entrante!=-1){
      yOpenMV=entrante;
      communicationMV=1;
    }
  }
}


void getImuInfo(){
  if(bno.getImpact()) //check if an high_g event occured (impact)
  {
    Serial.println("Impact Detected!");
    bno.loadOffsets(100);  //if yes load the calibration values saved in eeprom onto the bno
    while(bno.getHeading()==1);	//discard the first output from the bno because its most likely junk
  }
  IMU = bno.getHeading(); // se actualiza informacion del imu
  //si se registro un impacto en el imu entonces se cargan los valores calibrados en el eeprom para evitar error en el imu
  //Serial.println(IMU);
  if(IMU >= 255 )
  {
    orientacion1 = 255;
    orientacion2 = IMU - 255;
  }  
  else
  {
    orientacion1 = IMU;
    orientacion2 = 0;
  }
}

void cero(){
  ceroFake=IMU-calibracionImu;
  if (ceroFake<0){
    ceroFake=360+ceroFake;
  }
  Serial.println(ceroFake);

  if(ceroFake >= 255 )
  {
    orientacion1 = 255;
    orientacion2 = ceroFake - 255;
  }  
  else
  {
    orientacion1 = ceroFake;
    orientacion2 = 0;
  }
}

void sendToMotorController(int pwm, int orientationError, int movementIndicator) {
    const byte startMarker = 0xFF;
    const byte endMarker = 0xFE;
    
    // Aseguramos que los valores estén dentro de un rango válido para un byte
    byte pwmByte = constrain(pwm, 0, 255);
    byte orientationErrorByte = constrain(orientationError, 0, 255);
    byte movementIndicatorByte = movementIndicator ? 1 : 0; // Asegurar que solo sea 0 o 1.
    
    // Calculamos el checksum como la suma simple de los datos
    byte checksum = pwmByte + orientationErrorByte + movementIndicatorByte;
    
    // Enviamos los datos
    Serial5.write(startMarker);
    Serial5.write(pwmByte);
    Serial5.write(orientationErrorByte);
    Serial5.write(movementIndicatorByte);
    Serial5.write(checksum);
    Serial5.write(endMarker);
}

void jugar(){
  if(angEsp == 200 && stop == 0){
    sendToMotors(anguloIR,20,orientacion1,orientacion2, 0);     
  }

  else if(angEsp != 200 && stop == 0 && anguloIR <= 180 && anguloIR >= 0){
    //angEsp = sumarAng(angEsp,*2);
    sendToMotors(angEsp,20,orientacion1,orientacion2, 0);
    
  }
  else{
    sendToMotors(angEsp,20,orientacion1,orientacion2, 0);
  }
}

int sumarAng(int ang1, int  ang2){
  int x ,  y;
  x = cos(ang1) + cos(ang2);
  y = sin(ang1) + sin(ang2);
  return atan2(y,x);
}

void setup()
{
  uartStart();
  Wire.begin();   //inicia i2C 
  delay(10); //tiempo para que se inicialize i2c
  Wire.setClock(400000); //velocidad el bus i2c, 400000 modo de ultra alta velocidad
  bno.startBNO(200, false);
  pinMode(13, OUTPUT); // led integrado para debugear
  digitalWrite(13, HIGH); 
  if(Serial3.available() > 0){
    Serial3.clear();
    delay(1);
  }
 /* while(!bno.isCalibrated())	//wait until everything is fully calibrated once....
  {
    bno.serialPrintCalibStat();	//print the current calibration levels via serial
    delay(1000);
  }
  bno.saveOffsets(100);  //....then save the Offsets into the eeprom at the specified address 
*/
} 

int semaforo = 0;
void loop()
{
  getImuInfo();
  getUartInfo();

  cero();
  switch (estado){
    case 1:
      jugar();
      Serial.println("jugarJuan");
      break;
    case 2:
      Serial.println("calibrandoJuan");
      calibracionImu=IMU;
      break;
     case 3:
      Serial.println("calibrarJuanEnLaPista");
      
      
      break;
      case 254:
        estado = 0;
      break;
    default:
     // Serial.println("esperandoAJuan");
     if(uart2timer > 50){
     Serial2.write(anguloIR);
     uart2timer = 0;
     }
      break;
  }
  /*if(angEsp == 200 && stop == 0){
    sendToMotors(anguloIR,100,orientacion1,orientacion2, 0);    
  }

  else if(angEsp != 200 && stop == 0 && anguloIR <= 180 && anguloIR >= 0){
    angEsp = sumarAng(angEsp,anguloIR*2);
    sendToMotors(angEsp,100,orientacion1,orientacion2, 0);
    
  }
  else{
    sendToMotors(angEsp,100,orientacion1,orientacion2, 0);
  }*/
      
  
 
 //Serial.println(data5[0] * 2);
 //delay(100);
}