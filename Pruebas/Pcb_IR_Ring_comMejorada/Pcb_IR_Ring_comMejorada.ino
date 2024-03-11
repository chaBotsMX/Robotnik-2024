#include "TSSP.h"
const size_t dataLength = 2; //Tamaño del array a enviar
int data[2] = {0,0}; //Iniciamos el array

long readLimit;
int angle;
int intensidad;
int enviar;
int lastInten;
TSSP sensor_IR;

const byte startMarker = 0xFF;
const byte endMarker = 0xFE;
const size_t packetSize = 5; // Incluyendo el checksum

byte receivedPacket[packetSize] = {0};
bool newData = false;

// Variables para almacenar los datos recibidos
int receivedPWM = 0;
int receivedOrientationError = 0;
int receivedMovementIndicator = 0;


void receiveData() {
  static byte ndx = 0;
  byte startByte, rc;

  while (Serial1.available() > 0 && !newData) {
    rc = Serial1.read();

    if (ndx == 0 && rc != startMarker) {
      return; // Si no es el byte de inicio, ignora este byte
    }

    if (ndx > 0 && ndx < packetSize - 1) {
      // Verifica el checksum solo si no es el byte de inicio o fin
      receivedPacket[ndx] = rc;
    }
    
    ndx++;
    
    if (ndx == packetSize) {
      ndx = 0; // Restablecer para el próximo paquete
      byte checksum = calculateChecksum(receivedPacket, packetSize - 2); // -2 para excluir el checksum y endMarker
      if (rc == endMarker && checksum == receivedPacket[packetSize - 2]) {
        newData = true;
      } else {
        Serial.println("Error en checksum o paquete"); // Manejar errores según sea necesario
      }
    }
  }
}

byte calculateChecksum(byte *packet, size_t len) {
    byte checksum = 0;
    for (size_t i = 0; i < len; i++) {
        checksum += packet[i];
    }
    return checksum;
}

void processData() {
  receivedPWM = receivedPacket[1];
  // Mapear el error de orientación de vuelta a -180 a 180
  receivedOrientationError = map(receivedPacket[2], 0, 255, -180, 180);
  receivedMovementIndicator = receivedPacket[3];
}

int angulo(int x){
  if(x >= 180 && x <= 360){
    // Serial.println("debug");
    intensidad = 1000 - intensidad;
    return  x-90*intensidad/1200;
    

  }
  else if(x <180 && x >= 0){
//    y = 1762.2e-0.002x


    intensidad = 1000 - intensidad;
    return  x+90*intensidad/1200;
  }
  else{return 200;}
}

void setup() {
  Serial.begin(115200);   //puerto serial para debugear
  Serial1.begin(115200);  //puerto serial para enviar informacion, pines GPIO 16 RX y GPIO 17 TX, al actualizar las librerias pueden cambiarse los pines
  delay(1000); // se da tiempo para iniciar el uart de forma correcta o puede que los relojes no se sincronicen
  pinMode(25, OUTPUT);  //led integrado en la rasp para debugear
  readLimit=millis()+20;
}

void loop() {
  int angle = sensor_IR.angleIR(); // Obtiene el ángulo
  int intensity = sensor_IR.intIR(); // Obtiene la intensidad, en el rango de 0 a 1000

  // Ajustar si es necesario
  if(angle < 0 || angle > 360) angle = 200;
  if(intensity < 0) intensity = 0; // Asegura que la intensidad sea al menos 0
  if(intensity > 1000) intensity = 1000; // Asegura que la intensidad no exceda 1000

  // Estructura del paquete
  byte startMarker = 255;
  byte angleHigh = highByte(angle);
  byte angleLow = lowByte(angle);
  byte intensityHigh = highByte(intensity);
  byte intensityLow = lowByte(intensity);
  byte endMarker = 254;
  byte checksum = angleHigh + angleLow + intensityHigh + intensityLow;
  // Envío del paquete
  Serial1.write(startMarker);
  Serial1.write(angleHigh);
  Serial1.write(angleLow);
  Serial1.write(intensityHigh);
  Serial1.write(intensityLow);
  Serial1.write(checksum);
  Serial1.write(endMarker);



  // Debugging
  Serial.print("Ángulo: ");
  Serial.print(angle);
  Serial.print(", Intensidad: ");
  Serial.println(intensity);
}
