/*****************************************************************************************
*     Serial Bluetooth communication between 2 ESP32 boards - OK1TK www.ok1tk.com        *
*                   The CLIENT (SLAVE) part code v1.21 - 01 May 2023                     *
******************************************************************************************/
#include "BluetoothSerial.h"                                                  // BT: Include the Serial bluetooth library 
#define LED_BT 2                                                              // BT: Internal LED (or LED on the pin D2) for the connection indication (connected solid/disconnected blinking)
unsigned long previousMillis;                                                 // BT: Variable used for comparing millis counter (LED blinking)
bool ledBtState = false;                                                      // BT: Variable used to chage the indication LED state
bool MasterConnected;                                                         // BT: Variable used to store the current connection state (true=connected/false=disconnected)
String device_name = "ESP32-BT-Slave";                                        // BT: Variable used to store the CLIENT(slave) bluetooth device name
String MACadd = "D4:8A:FC:CF:BB:5A";  //ESP32                                          // BT: Variable used to store the CLIENT(slave) bluetooth Mac address; Use your own MAC address
//String MACadd = "98:FA:AB:6B:1D:FA";

int estado=0;

// BT: Bluetooth availabilty check
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)                           
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif
// BT: Serial Bluetooth availabilty check
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;                                                     // BT: Set the Object SerialBT

// BT: Bt_Status callback function
void Bt_Status (esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {

  if (event == ESP_SPP_SRV_OPEN_EVT) {                                        // BT: Checks if the SPP Server connection is open, the event comes 
    Serial.println ("Client Connected");                                      // BT: Write to the serial monitor
    digitalWrite (LED_BT, HIGH);                                              // BT: Turn ON the bluetooth indication LED (solid light)
    MasterConnected = true;                                                   // BT: Set the variable true = SERVER is connected to the CLIENT
  }
  else if (event == ESP_SPP_CLOSE_EVT ) {                                     // BT: Checks if the SPP connection is closed, the event comes
    Serial.println ("Client Disconnected");                                   // BT: Write to the serial monitor
    digitalWrite (LED_BT, LOW);                                               // BT: Turn OFF the bluetooth indication LED
    MasterConnected = false;                                                  // BT: Set the variable false = SERVER connection lost
  }
}

void setup() {
  pinMode(LED_BT,OUTPUT);                                                     // BT: Set up the onboard LED pin as output
  Serial.begin(115200);                                                       // Sets the data rate in bits per second (baud) for serial data transmission
  
  // BT: Define the Bt_Status callback
  SerialBT.register_callback (Bt_Status);
  // BT: Starts the bluetooth device with the name stored in the device_name variable
  SerialBT.begin(device_name);
  Serial.printf("The device with name \"%s\" and MAC address \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), MACadd.c_str());
}

void loop() {
  // BT: Blinking the bluetooth indication LED if the connection is not established
  if (!MasterConnected) {
    if (millis() - previousMillis >= 500) {                                     // BT: Checks if 500ms is passed
      if (ledBtState == false) {                                                // BT: Checks the leddState and toggles it 
      ledBtState = true;
      estado=1;
    }
    else {
      ledBtState = false;
    }
    digitalWrite(LED_BT, ledBtState);                                             // BT: Set LED ON/OFF based onthe ledBtState variable
    previousMillis = millis();                                                    // BT: Set previousMillis to current millis
    }
    
  }
  // BT: Data send/receive via bluetooth
  if (Serial.available()) {                                                       // BT: Checks if there are data from the serial monitor available
    SerialBT.write(Serial.read());                                                // BT: Sends the data via bluetooth
    SerialBT.write(estado);
  }

  if (SerialBT.available()) {                                                     // BT: Checks if there are data from the bluetooth available
    Serial.write(SerialBT.read());                                                // BT: Write the data to the serial monitor
  }
}