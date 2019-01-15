/*  Data packet structure
 *   
 *   |_ _|   |_ _| |_ _ _ ..._| - up to 64 byte
 *    /\      /\       /\
 *    ||      ||       ||
 * receiver sender   payload
 *    ID      ID
 *    
 * Command example: 0100AT-C001 -> set nano with ID=01 to 1 radio chanel
 *                  0200AT-RX   -> get all HC12 settings for nano with ID=02
 *                  0100MODE-0  -> set nano with ID=01 to LASER_CONFIG mode
 *                  0100MODE-1  -> set nano with ID=01 to LASER_RUN mode
 *                  0100MODE    -> get active mod from nano with ID=01
 *                  0000MODE-1  -> set all nanos to LASER_RUN mode
 *                  0100THRESHOLD-300 -> set light sensor threshold level
 *                  0100THRESHOLD     -> get light sensor threshold level
 *                  
 * Receiving sequence
 *                  0003 MODE: RUN THRES: 600 SENSOR: 555 VSS: 3999 BTN: OFF
 *                  0003;0;600;555;3999;0
 *                  0003 THRESHOLD DONE
 *                  0003 BTN: ON
*/
//#include <SoftwareSerial.h>

// Mode types supported:
enum  mode {LASER_CONFIG, LASER_RUN};

mode activeMode = LASER_CONFIG;

unsigned long Interval;   // milliseconds between updates
unsigned long lastUpdate; // last update of position

#define setPin 22

byte incomingByte;
String readBuffer = "";
String header;
String payload;
int sensor_threshold = 500;  // threshold for light sensor
String _ID = "00"; //Nano transceiver ID - unique for each Nano-HC-12 accemble
String MASTER_ID = "00";

//SoftwareSerial HC12(10,11); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(19200);             // Serial port to computer
  Serial1.begin(19200);               // Serial port to HC12
  pinMode(setPin, OUTPUT);
  digitalWrite(setPin, HIGH); 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {

  readBuffer = "";                       // Clear readBuffer

  if (Serial.available()) {
    delay(100);
    while (Serial.available()) {      // If Serial monitor has data
      incomingByte = Serial.read();          // Store each icoming byte from HC-12
      readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable   
    };
    Serial.println(readBuffer);
    checkATCommand();
    checkModeCommand(); 
    
  }

  else if (Serial1.available()) {
    delay(40);
    while (Serial1.available()) {        // If HC-12 has data
      incomingByte = Serial1.read();          // Store each icoming byte from HC-12
      readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
    };
    Serial.println(readBuffer);
    
  }

  else {
    
  };

  

}

// ==== Custom function - Check whether we have received an AT Command via the Serial Monitor
void checkATCommand () {

  if (readBuffer.startsWith("AT")) {     // Check whether the String starts with "AT"

    payload = _ID + MASTER_ID;
    payload.concat(readBuffer);
    //Serial.print(payload); 
    Serial1.print(payload);              // Set remote HC-12 into AT Command mode
    delay(200); 
    
    digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(200);                          // Wait for the HC-12 to enter AT Command mode
    Serial1.print(readBuffer);              // Send AT Command to HC-12
    delay(200);
    while (Serial1.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(Serial1.read());           // Send the data to Serial monitor
    }
    delay(200);
    digitalWrite(setPin, HIGH);          // Exit AT Command mode
  }
}

void checkModeCommand() {

    payload = readBuffer.substring(4);
    Serial1.flush();
      
    if (payload.startsWith("MODE")) { // Check whether the String starts with "MODE"
       
       Serial1.print(readBuffer);
    }
      
    else if (payload.startsWith("THRESHOLD")) { // Check whether the String starts with "THRESHOLD"
    
       Serial1.print(readBuffer); 
  }
}
