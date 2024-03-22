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

#define intensidad_constante 140
#define intensidad_cuadrado 19600

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
String inputString = ""; 
bool stringComplete = false;

// Variables globales para almacenar el ángulo e intensidad
int globalAngle = 0;
int globalIntensity = 0;
int localAngle = 0;
int localIntensity=0;
// Estado inicial
State currentState = WAIT_FOR_START;

// Variable para calcular el checksum
byte calculatedChecksum = 0;


BNO bno;  //create bno from the Class BNO

elapsedMillis stopTimer;

elapsedMillis uart2timer;
elapsedMillis irTimer;
void uartStart(){
  Serial.begin(115200); //comunicacion con pc
  delay(1000);
  Serial1.begin(57600); //anillo de ir
  delay(1000);
  Serial2.begin(115200); //M5
  delay(1000);
  Serial3.begin(115200, 0X00);//sensor de linea
  delay(1000);
  Serial4.begin(9600); //openMV
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
        localAngle = incomingByte * 255;
       // Serial.println(incomingByte);
        
        calculatedChecksum += incomingByte;
        currentState = READ_ANGLE_LOW;
        break;

      case READ_ANGLE_LOW:
        localAngle = localAngle + incomingByte;
        //Serial.println(incomingByte);
        //Serial.println(localAngle);
        calculatedChecksum += incomingByte;
        currentState = READ_INTENSITY_HIGH;
        break;

      case READ_INTENSITY_HIGH:
        localIntensity=incomingByte*255;

        calculatedChecksum += incomingByte;
        currentState = READ_INTENSITY_LOW;
        break;

      case READ_INTENSITY_LOW:
        localIntensity+=incomingByte;

        calculatedChecksum += incomingByte;
        currentState = READ_CHECKSUM;
        break;

      case READ_CHECKSUM:
        // Verifica el checksum
        if (calculatedChecksum == incomingByte) {
          currentState = WAIT_FOR_END;
        } else {
          // Checksum no coincide, intenta resincronizar
          //Serial.println("Error en el checksum, intentando resincronizar...");
          currentState = WAIT_FOR_START;
        }
        break;

      case WAIT_FOR_END:
        if (incomingByte == 254) {
          // Mensaje completo y correcto, procesa los datos
          //Serial.println("Mensaje recibido correctamente.");
          // Aquí iría la lógica para procesar los datos recibidos
          if (localAngle==400){
            globalAngle=4001;
          }
          else{
            globalAngle = ajusteAngulo(localAngle, localIntensity);
          }
          globalIntensity=localIntensity;
          //Serial.println(globalAngle);
          irTimer = 0;
        } else {
          // Fin de mensaje incorrecto, intenta resincronizar
         // Serial.println("Fin de mensaje incorrecto, intentando resincronizar...");
        }
        currentState = WAIT_FOR_START; // Vuelve al inicio para recibir un nuevo mensaje
        break;
    }
  }
  else{
    if(irTimer > 5){
    globalAngle= 400;
   // Serial.println(globalAngle);
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
    //Serial.println(angEsp);
  }

  openMVSetup();

}

void openMVSetup(){
 while (Serial4.available()) {
    char inChar = (char)Serial4.read(); // Lee el siguiente carácter disponible
    inputString += inChar; // Añade el carácter a la cadena
    if (inChar == '\n') { // Si el carácter es un salto de línea, marca el fin del mensaje
      stringComplete = true;
    }
  }
  procesarDatos();
} 
void procesarDatos(){
   if (stringComplete) {
    Serial.println(inputString); // Imprime el string completo
    
    // Variables para almacenar los valores X y Y
    int vectorAmarilloX, vectorAmarilloY, vectorAzulX, vectorAzulY;

    // Extraer los valores de los vectores del string
    sscanf(inputString.c_str(), "YA:%d %d, BA:%d %d", &vectorAmarilloX, &vectorAmarilloY, &vectorAzulX, &vectorAzulY);

    // Imprimir los valores para verificar
    Serial.print("Vector Amarillo X: "); Serial.println(vectorAmarilloX);
    Serial.print("Vector Amarillo Y: "); Serial.println(vectorAmarilloY);
    Serial.print("Vector Azul X: "); Serial.println(vectorAzulX);
    Serial.print("Vector Azul Y: "); Serial.println(vectorAzulY);

    // Limpia la cadena para el próximo mensaje y resetea el indicador de string completo
    inputString = "";
    stringComplete = false;
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
}

void cero(){
  ceroFake=IMU-calibracionImu;
  if (ceroFake<0){
    ceroFake=360+ceroFake;
  }
  ceroFake = ceroFake/2;
  //Serial.println(ceroFake);
}

void sendToMotorController(int pwm, int orientationError, int movementIndicator, int angleToMove) {
    const byte startMarker = 0xFF;
    const byte endMarker = 0xFE;
    
    // Aseguramos que los valores estén dentro de un rango válido para un byte
    byte pwmByte = constrain(pwm, 0, 255);
    byte orientationErrorByte = constrain(orientationError, 0, 255);
    byte movementIndicatorByte = movementIndicator ? 1 : 0; // Asegurar que solo sea 0 o 1.
    byte angleToMoveByte = constrain(angleToMove, 0, 255); 
    
    // Calculamos el checksum como la suma simple de los datos
    byte checksum = pwmByte + orientationErrorByte + movementIndicatorByte + angleToMoveByte;
    
    // Enviamos los datos
    Serial5.write(startMarker);
    Serial5.write(pwmByte);
    Serial5.write(orientationErrorByte);
    Serial5.write(movementIndicatorByte);
    Serial5.write(angleToMoveByte);
    Serial5.write(checksum);
    Serial5.write(endMarker);
    //Serial.println(orientationErrorByte);
    //Serial.println(angleToMoveByte);
   // Serial.println(pwmByte);
}

void traslado(){
  

  float trasladoX= 15 -xOpenMV;
  float trasladoY=10 -yOpenMV;
  float anguloTraslado;
  
  anguloTraslado=atan2f(trasladoY,trasladoX);
  globalAngle=anguloTraslado*180/3.1416;
 // Serial.println(globalAngle);

  if (xOpenMV>=10 && xOpenMV<=20 && yOpenMV>=5 && yOpenMV<=15){
    Serial.println("stop");
    globalAngle=400;
  }
  globalAngle = floor(globalAngle);
 
}

int ajusteAngulo(int x, int intensidad){
  int anguloModificado = x;
  if(anguloModificado<=350 && anguloModificado >=30){
    anguloModificado>=180 ? x=1 : x=0; 
    anguloModificado>=180 ? anguloModificado=360.00-anguloModificado : anguloModificado=anguloModificado;
    double t=sqrt(((intensidad*intensidad)+(intensidad_cuadrado))-2*(intensidad*intensidad_constante)*cos(anguloModificado*M_PI/180.00));
    double d=asin(((intensidad*sin(anguloModificado*M_PI/180.00))/t))*180.00/M_PI;
    if(!x)
      anguloModificado=180.00-(180.00-anguloModificado-d);
    else
      anguloModificado=180.00+(180.00-anguloModificado-d);

    return anguloModificado;
  } else {
    return 0;
  }
}





void jugar(){
  if(angEsp == 200 && stop == 0){
   sendToMotorController(100,ceroFake,0,globalAngle/2); 
  // Serial.print("mandado globalangle");
  }

  else if(angEsp != 200 && stop == 0 && anguloIR <= 180 && anguloIR >= 0){
    //angEsp = sumarAng(angEsp,*2);
    sendToMotorController(100,ceroFake,0, angEsp);
    //Serial.print("mandado esp");
  }
  else{
    sendToMotorController(100,ceroFake,0, angEsp);
    //Serial.print("mandado angEsp");
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
  inputString.reserve(200); 
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
  Serial.println(globalAngle);
  //estado = 1;
  switch (estado){
    case 1: //setMode
      calibracionImu=IMU;
      break;

    case 2: //partido
      jugar();
  
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
     Serial2.write(globalAngle);
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