/*  Data packet structure
 *   
 *   |_ _|   |_ _| |_ _ _ ..._| - up to 64 byte
 *    /\      /\       /\
 *    ||      ||       ||
 * receiver sender   payload
 *    ID      ID
 *    
 *    allowed command
 *    Command example: 0100AT-C001 -> set nano with ID=01 to 1 radio chanel
 *                  0200AT-RX   -> get all HC12 settings for nano with ID=02
 *                  0100MODE-0  -> set nano with ID=01 to LASER_CALIBRATION mode
 *                  0100MODE-1  -> set nano with ID=01 to LASER_RUN mode
 *                  0100MODE-2  -> set nano with ID=01 to LASER_CONFIG mode
 *                  0100MODE    -> get active mod from nano with ID=01
 *                  0000MODE-1  -> set all nanos to LASER_RUN mode
 *                  0100THRESHOLD-300 -> set light sensor threshold level
 *                  0100THRESHOLD     -> get light sensor threshold level
 * 
 *    Sending sequence
 *    0003 MODE: RUN THRES: 600 SENSOR: 555 VSS: 3999 BTN: OFF LASERCROSS 1//OFF->1 ON->0
 *    0003;0;600;555;3999;1;1;
 *    0003 THRESHOLD DONE
 *    0003 BTN: ON
 * 
 */
#include <SoftwareSerial.h>

#define settingsPin 12
#define buttonPin 2

// Mode types supported:
enum  mode {LASER_CALIBRATION, LASER_RUN, LASER_CONFIG};

mode activeMode = LASER_CALIBRATION;
unsigned long Interval = 5000;     // milliseconds between updates
unsigned long lastUpdate = 0;   // last update of position
int sensorPin = A0;  // select the input pin for the photoresestor
int sensorValue = 0;        // variable to store the value coming from the sensor
int sensor_threshold = 1024;  // threshold for light sensor
long vssValue = 0;  

int buttonState = HIGH;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin

int laserCrossState = 1;             // the current reading from the light sensor, 1- laser crossed
int lastlaserCrossState = 1;   // the previous reading from the light sensor

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

byte incomingByte;
String readBuffer = "";
String header;
String payload;
String _ID = "01"; //Nano transceiver ID - unique for each Nano-HC-12 accemble
String MASTER_ID = "00";

SoftwareSerial HC12(10,11); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(19200);             // Serial port to computer
  HC12.begin(19200);               // Serial port to HC12
  pinMode(settingsPin, OUTPUT);
  digitalWrite(settingsPin, HIGH); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(LED_BUILTIN, LOW);
  analogReference(DEFAULT);
}

void loop() {

  //sensorValue = analogRead(sensorPin);
  readButtonState(); 
  readLaserState();
  readBuffer = "";                       // Clear readBuffer

  if (Serial.available()) {
    
    while (Serial.available()) {      // If Serial monitor has data
      incomingByte = Serial.read();          // Store each icoming byte from HC-12
      readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable 
      delay(20);  
    };
    Serial.println(readBuffer);
    checkATCommand();
    
  }

  else if (HC12.available()) {
    delay(100);
    if (receiveRadioData()) {
        //Serial.println(readBuffer);
        checkATCommand();
        checkModeCommand();
    }
  }

  else {

     switch (activeMode)
        {
          case LASER_CALIBRATION:           
            if ((millis()-lastUpdate) > Interval)
                {
                  lightMeasurement();
                  payload = getModuleState();
                  Serial.println(payload);
                  if (!HC12.available()){                    
                     HC12.print(payload);
                     //delay(5000);
                  }
                  lastUpdate = millis();
                }
            break;
          case LASER_RUN:
            //checkLaserCross();
            break;
          case LASER_CONFIG:
            delay(50);
            break;  
          default:
            delay(50);
            break;
        }
  };

  

}

// ==== Custom function - Check whether we have received an AT Command via the Serial Monitor
void checkATCommand () {

  if (readBuffer.startsWith("AT")) {     // Check whether the String starts with "AT"
    
    digitalWrite(settingsPin, LOW);           // Set HC-12 into AT Command mode
    delay(200);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print(readBuffer);              // Send AT Command to HC-12
    HC12.flush();
    delay(200);
    while (HC12.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(HC12.read());           // Send the data to Serial monitor
    }
    delay(200);
    digitalWrite(settingsPin, HIGH);          // Exit AT Command mode
  }
}

void lightMeasurement() {

        sensorValue = analogRead(sensorPin);
        vssValue = readVcc();
        vssValue = vssValue*1.38;
        //1.265 for ID=01 - enother type of mega328 chip
        
}

/*
 * return true if packet intended for this sensor
 */
bool receiveRadioData() {

    while (HC12.available()) {        // If HC-12 has data
      incomingByte = HC12.read();          // Store each icoming byte from HC-12
      readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
    };
    //Serial.println(readBuffer);
    header = readBuffer.substring(0,4);
    //Serial.println(header);
    //Serial.println(_ID+MASTER_ID);
    if (header.equals(_ID+MASTER_ID) || header.equals(MASTER_ID+MASTER_ID) ) { //separate only broadcast command or command for _ID-module 
       readBuffer =  readBuffer.substring(4);
       return true;
    }
    else {
       readBuffer = "";
       return false; 
    }
}

void checkModeCommand() {
    String payload = MASTER_ID + _ID;
    if (readBuffer.startsWith("MODE")) { // Check whether the String starts with "MODE"
      if (readBuffer.length() == 5) {
          payload = getModuleState();
          Serial.println(payload);
          if (!HC12.available()){                    
              HC12.print(payload);
          }    
      }
      else {
          payload = readBuffer.substring(5,6);   // Set active mode
          activeMode = payload.toInt();        // 
          
          payload = getModuleState();
          Serial.println(payload);
          if (!HC12.available()){                    
              HC12.print(payload);
              //delay(5000);
          }
      }
    }
      
    if (readBuffer.startsWith("THRESHOLD")) { // Check whether the String starts with "MODE"
      if (readBuffer.length() == 10) {
          payload = getModuleState();
          Serial.println(payload);
          if (!HC12.available()){                    
              HC12.print(payload);
          }
      }
      else {
          payload = readBuffer.substring(10);   // Get threshold level
          if (payload.toInt()>0)
              sensor_threshold = payload.toInt(); 
          payload = getModuleState();
          Serial.println(payload);
          if (!HC12.available()){                    
              HC12.print(payload);
          } 
      }
     }
  readBuffer = "";
}

void checkLaserCross() {
  
     String payload = MASTER_ID + _ID;
     char serialbuff[64];
     sensorValue = analogRead(sensorPin);
     delay(1);
     
     if (sensorValue > sensor_threshold)
     {
        //sprintf(serialbuff, "0001Sensor Value:  %d", sensorValue);
        sprintf(serialbuff, " THRESHOLD DONE");
        payload.concat(serialbuff);
        Serial.println(payload);
        while (activeMode == LASER_RUN) {    //wait for transmission available
              if (!HC12.available()) {
                 HC12.print(payload);
                 delay(20);
                 activeMode = LASER_CALIBRATION;
              }
        }
     }
  
}

void readButtonState() {
    // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from HIGH to LOW), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only send message if the new button state is LOW
      if (buttonState == LOW) {
         String payload = MASTER_ID + _ID;
         char serialbuff[64];
            //sprintf(serialbuff, "0001Sensor Value:  %d", sensorValue);
            sprintf(serialbuff, " BTN: ON");
            payload.concat(serialbuff);
            Serial.println(payload);
            //while (activeMode == LASER_RUN) {    //wait for transmission available
                  if (!HC12.available()) {
                     HC12.print(payload);
                     delay(20);
                     //activeMode = LASER_CALIBRATION;
                  }
            //}
      }
    }
    
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

void readLaserState() {
      // read the state of the switch into a local variable:
      int reading;
      sensorValue = analogRead(sensorPin);
      delay(1);     
      if (sensorValue > sensor_threshold)
          reading = 1;
      else     
          reading = 0;

    // if the laser state has changed:
    if (reading != laserCrossState) {
      laserCrossState = reading;

      // only send message if the new laserCrossState state is 1
      if (laserCrossState == 1) {
         String payload = MASTER_ID + _ID;
         char serialbuff[64];
            //sprintf(serialbuff, "0001Sensor Value:  %d", sensorValue);
            sprintf(serialbuff, " THRESHOLD DONE");
            payload.concat(serialbuff);
            Serial.println(payload);
            //while (activeMode == LASER_RUN) {    //wait for transmission available
                  if (!HC12.available()) {
                     HC12.print(payload);
                     delay(300);
                     //activeMode = LASER_CALIBRATION;
                  }
            //}
      }
    }
    
 

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastlaserCrossState = reading;
}

void moduleReplayDelay() {
  
  int replayDelay = 1000 + _ID.toInt()*100;
  delay(replayDelay);
  
}

long readVcc() { //measure applyed vss +5V on arduino board
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

/*    return sending sequence
 *    0003 MODE: RUN THRES: 600 SENSOR: 555 VSS: 3999 BTN: OFF
 *    0003;0;600;555;3999;0;
 */
String getModuleState() {

        String payload = MASTER_ID + _ID + ';';
        payload += String(activeMode) + ';';
        payload += String(sensor_threshold) + ';';
        payload += String(sensorValue) + ';';
        payload += String(vssValue) + ';';
        payload += String(buttonState) + ';';
        payload += String(laserCrossState) + ';';

        return payload;
}
