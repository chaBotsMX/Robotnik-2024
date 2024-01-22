#include <Wire.h>
#include <BohleBots_BNO055.h>

const size_t dataLength1 = 2;
const size_t dataLength5 = 4;

int data1[4] = {0,0};
int pelota1 = 0, pelota2 = 0;
int orientacion1 = 0, orientacion2 = 0;
int data5[4] = {0,0,0,0};
int IMU;
int norte;

BNO bno;  //create bno from the Class BNO

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial5.begin(74880);
  Wire.begin();
  Wire.setClock(400000);
  bno.startBNO(200, false);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  while(!bno.isCalibrated())	//wait until everything is fully calibrated once....
  {
    bno.serialPrintCalibStat();	//print the current calibration levels via serial
    delay(1000);
  }
  bno.saveOffsets(100);  //....then save the Offsets into the eeprom at the specified address 

} 

void loop()
{
    if(Serial1.available() >= dataLength1 * sizeof(data1[0]))
  { 
    Serial1.readBytes((byte*)data1, dataLength1 * sizeof(data1[0]));
  }
   if(bno.getImpact()) //check if an high_g event occured (impact)
  {
    Serial.println("Impact Detected!");
    bno.loadOffsets(100);  //if yes load the calibration values saved in eeprom onto the bno
    while(bno.getHeading()==1);	//discard the first output from the bno because its most likely junk
  }
  data1[0] = pelota1;
  data1[1] = pelota2; 
  
  IMU = bno.getHeading();
  
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


  data5[0] = pelota1;
  data5[1] = pelota2;
  data5[2] = orientacion1;
  data5[3] = orientacion2;


  Serial5.write((byte*)data5, dataLength5 * sizeof(data5[0]));
  //Serial.println("enviado");
  Serial.print(data5[2]);
  Serial.print(" ");
  Serial.println(data5[3]);

  delay(10);
}