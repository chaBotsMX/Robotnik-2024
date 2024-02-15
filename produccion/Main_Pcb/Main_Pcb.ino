#include <Wire.h>
#include <BohleBots_BNO055.h>
//declaramos el tamaño de los array a enviar, son 5, pero para organizar los declaramos cuando se necesiten
const size_t dataLength1 = 2;
const size_t dataLength5 = 5;

int stop = 0;
int angEsp = 200;
int data1[2] = {0,0}; //Angulo de la pelota IR partido en 2 bytes

int data5[5] = {0,0,0,0,0}; // byte 1 y 2 son para enviar el angulo de la pelota, 3 y 4 el offset del imu y el 5 para enviar el pwm maximo 

int pelota1 = 0, pelota2 = 0, orientacion1 = 0, orientacion2 = 0, angulo = 200; // declaramos los bytes para poder trabajar con ellos 

int IMU, norte; //varaibles usadas para los calculos del imu

int estado, intensidad = 0;

BNO bno;  //create bno from the Class BNO

elapsedMillis stopTimer;

void uartStart(){
  Serial.begin(115200); //comunicacion con pc
  delay(1000);
  Serial1.begin(115200); //anillo de ir
  delay(1000);
  Serial2.begin(115200); //M5
  delay(1000);
  Serial3.begin(115200);//sensor de linea
  delay(1000);
  Serial5.begin(115200); // comunicacion con placa de motores
  delay(1000); //tiempo para sincronizar

}

void getUartInfo(){

  if(Serial1.available()) //si hay informacion nueva se lee y se mueve
  { 
     angulo = Serial1.read();
     Serial.println(angulo);
     if(angulo < 0 || angulo > 180 ){
      angulo = 200;
     } 
  }
  else {
    angulo = 200;
  }

  if (Serial2.available())
  {
    estado = Serial2.read();
   // Serial.println(estado);
  }
  if(Serial3.available())
  {
    angEsp = Serial3.read();
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
void sendToMotors(int angle, int pwm, int imu1, int imu2, int wait)
{
  data5[0] = angle;
  data5[1] = pwm;
  data5[2] = imu1;
  data5[3] = imu2;
  data5[4] = wait;

  Serial5.write((byte*)data5, dataLength5 * sizeof(data5[0]));
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
 /* while(!bno.isCalibrated())	//wait until everything is fully calibrated once....
  {
    bno.serialPrintCalibStat();	//print the current calibration levels via serial
    delay(1000);
  }
  bno.saveOffsets(100);  //....then save the Offsets into the eeprom at the specified address 
*/
} 

void loop()
{
 getImuInfo();
 getUartInfo();

  if(angEsp == 200 && stop == 0){
    sendToMotors(angulo,100,orientacion1,orientacion2, 0);    
  }

  else{
    sendToMotors(angEsp,100,orientacion1,orientacion2, 0);
    
  }
 
 Serial.println(data5[0] * 2);
}