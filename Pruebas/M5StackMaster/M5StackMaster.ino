/*****************************************************************************************
*     Serial Bluetooth communication between 2 ESP32 boards - OK1TK www.ok1tk.com        *
*                   The SERVER (Master) part code v1.32 - 04 May 2023                    *
******************************************************************************************/
#include "BluetoothSerial.h"                                                              // BT: Include the Serial bluetooth library
#include <M5Stack.h>

//#define LED_BT_BLUE 2                                                                     // BT: Internal LED (or LED on the pin D2) for the connection indication (connected LED ON / disconnected LED OFF)
//#define LED_BT_RED 15                                                                     // BT: LED (LED on the pin D4) for the connection indication (connected LED OFF / disconnected LED ON)
unsigned long previousMillisReconnect;                                                    // BT: Variable used for comparing millis counter for the reconnection timer
bool ledBtState = false;                                                                  // BT: Variable used to chage the indication LED state
bool SlaveConnected;                                                                      // BT: Variable used to store the current connection state (true=connected/false=disconnected)
int recatt = 0;                                                                           // BT: Variable used to count the reconnection attempts
int estado=0;


// BT: Bluetooth availabilty check
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

String myName = "M5STACK-BT-Master";                                                        // BT: Variable used to store the SERVER(Master) bluetooth device name; just for prinitng
String slaveName = "ESP32-BT-Slave";                                                      // BT: Variable used to store the CLIENT(Slave) bluetooth device name; just for prinitng; just for printing in this case
//String MACadd = "98:F4:AB:6B:1D:FA";  //M5STACK                                                      // BT: Variable used to store the CLIENT(Slave) bluetooth device Mac address; just for prinitng; just for printing in this case
String MACadd = "D4:8A:FC:CF:BB:5A";
uint8_t address[6] = { 0xD4, 0x8A, 0xFC, 0xCF, 0xBB, 0x5A };                              // BT: Variable used to store the CLIENT(Slave) MAC address used for the connection; Use your own andress in the same format

BluetoothSerial SerialBT;                                                                 // BT: Set the Object SerialBT


// BT: Bt_Status callback function
void Bt_Status(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_OPEN_EVT) {                                                        // BT: Checks if the SPP connection is open, the event comes// event == Client connected
    Serial.println ("Client Connected");                                                  // BT: Write to the serial monitor
    //digitalWrite (LED_BT_BLUE, HIGH);                                                     // BT: Turn ON the BLUE bluetooth indication LED (solid light)
    //digitalWrite (LED_BT_RED, LOW);                                                       // BT: Turn OFF the RED bluetooth indication LED
    M5.Lcd.clear(BLACK);
    M5.Lcd.clear(BLUE);
    estado=1;
    SlaveConnected = true;                                                                // BT: Set the variable true = CLIENT is connected to the SERVER
    recatt = 0;                                                                           // BT: Reset the reconnect attempts counter
  }
  else if (event == ESP_SPP_CLOSE_EVT) {                                                  // BT: event == Client disconnected
    Serial.println("Client Disconnected");                                                // BT: Write to the serial monitor
    //digitalWrite(LED_BT_RED, HIGH);                                                       // BT: Turn ON the RED bluetooth indication LED (solid light)
    //digitalWrite(LED_BT_BLUE, LOW);                                                       // BT: Turn OFF the BLUE bluetooth indication LED
    M5.Lcd.clear(BLACK);
    M5.Lcd.clear(RED);
    estado=0;
    SlaveConnected = false;                                                               // BT: Set the variable false = CLIENT connection lost
  }
}

void setup() {
  M5.begin();        
  M5.Power.begin();
  //pinMode(LED_BT_BLUE, OUTPUT);                                                           // BT: Set up the onboard LED pin as output
  //pinMode(LED_BT_RED, OUTPUT);                                                            // BT: Set up the onboard LED pin as output
  //digitalWrite(LED_BT_RED, HIGH);                                                         // BT: Turn ON the RED LED = no connection established
  M5.Lcd.clear(RED);
  estado=0;
  SlaveConnected = false;                                                                 // BT: Set the variable false = CLIENT is not connected
  Serial.begin(115200);                                                                   // Sets the data rate in bits per second (baud) for serial data transmission
  
  // BT: Define the Bt_Status callback
  SerialBT.register_callback(Bt_Status);
  // BT: Starts the bluetooth device with the name stored in the myName variable as SERVER(Master)
  SerialBT.begin(myName, true);
  Serial.printf("The device \"%s\" started in master mode, make sure slave BT device is on!\n", myName.c_str());
  SlaveConnect();                                                                         // BT: Calls the bluetooth connection function to cnnect to the CLIENT(Slave)
}

void SlaveConnect() {                                                                     // BT: This function connects/reconnects to the CLIENT(Slave)
  Serial.println("Function BT connection executed");                                      // BT: Write to the serial monitor
  Serial.printf("Connecting to slave BT device named \"%s\" and MAC address \"%s\" is started.\n", slaveName.c_str(), MACadd.c_str());  // BT: Write to the serial monitor
  SerialBT.connect(address);                                                              // BT: Establishing the connection with the CLIENT(Slave) with the Mac address stored in the address variable
}

void loop() {
  
  if (!SlaveConnected) {                                                                  // BT: Condition to evalute if the connection is established/lost 
    if (millis() - previousMillisReconnect >= 10000) {                                    // BT: Check that 10000ms is passed
      previousMillisReconnect = millis();                                                 // BT: Set previousMillisReconnect to current millis
      recatt++;                                                                           // BT: Increase the the reconnection attempts counter +1 
      Serial.print("Trying to reconnect. Attempt No.: ");                                 // BT: Write to the serial monitor
      Serial.println(recatt);                                                             // BT: Write the attempts count to the serial monitor
      Serial.println("Stopping Bluetooth...");                                            // BT: Write to the serial monitor
      SerialBT.end();                                                                     // BT: Close the bluetooth device
      Serial.println("Bluetooth stopped !");                                              // BT: Write to the serial monitor
      Serial.println("Starting Bluetooth...");                                            // BT: Write to the serial monitor
      SerialBT.begin(myName, true);                                                       // BT: Starts the bluetooth device with the name stored in the myName variable as SERVER(Master)
      Serial.printf("The device \"%s\" started in master mode, make sure slave BT device is on!\n", myName.c_str());
      SlaveConnect();                                                                     // BT: Calls the bluetooth connection function to cnnect to the CLIENT(Slave)
    }
    
  }
  // BT: Data send/receive via bluetooth
  if (Serial.available()) {                                                               // BT: Checks if there are data from the serial monitor available
    SerialBT.write(Serial.read());                                                        // BT: Sends the data via bluetooth
    SerialBT.write(estado);
  }
  if (SerialBT.available()) {                                                             // BT: Checks if there are data from the bluetooth available
    Serial.write(SerialBT.read());                                                        // BT: Write the data to the serial monitor
  }
}