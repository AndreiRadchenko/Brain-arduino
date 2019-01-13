/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>

#define LED_N_SIDE 2
#define LED_P_SIDE 3

unsigned long Interval;   // milliseconds between updates
unsigned long lastUpdate; // last update of position

#define setPin 12

byte incomingByte;
String readBuffer = "";
const char exchangebuff[50];

SoftwareSerial HC12(10,11); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12
  pinMode(setPin, OUTPUT);
  digitalWrite(setPin, HIGH); 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {

  //readBuffer = lightMeasurement();
  
//  while (HC12.available()) {        // If HC-12 has data
//    //Serial.write(HC12.read());      // Send the data to Serial monitor
//    readBuffer = "";
//    incomingByte = HC12.read();          // Store each icoming byte from HC-12
//    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
//  }
  
  while (Serial.available()) {      // If Serial monitor has data
    //HC12.write(Serial.read());      // Send that data to HC-12
    
    incomingByte = Serial.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
    Serial.println(readBuffer);
    
  }

  //HC12.println(readBuffer);

  Serial.println(readBuffer);
  delay(1000);
  checkATCommand();
  readBuffer = "";                       // Clear readBuffer
}

// ==== Custom function - Check whether we have received an AT Command via the Serial Monitor
void checkATCommand () {

  if (readBuffer.startsWith("AT")) {     // Check whether the String starts with "AT"

      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    
    digitalWrite(setPin, LOW);           // Set HC-12 into AT Command mode
    delay(200);                          // Wait for the HC-12 to enter AT Command mode
    HC12.print(readBuffer);              // Send AT Command to HC-12
    delay(200);
    while (HC12.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(HC12.read());           // Send the data to Serial monitor
    }
    delay(1000);
    digitalWrite(setPin, HIGH);          // Exit AT Command mode
  }
}

String lightMeasurement() {
        unsigned int j;
        char serialbuff[50];
        //char buf[12];
    
      // Apply reverse voltage, charge up the pin and led capacitance
      pinMode(LED_N_SIDE, OUTPUT);
      pinMode(LED_P_SIDE, OUTPUT);
      digitalWrite(LED_N_SIDE, HIGH);
      digitalWrite(LED_P_SIDE, LOW);
    
      // Isolate the pin 2 end of the diode by changing it from OUTPUT HIGH to 
      // INPUT LOW (high impedance input with internal pull-up resistor off)
      pinMode(LED_N_SIDE, INPUT);
      digitalWrite(LED_N_SIDE,LOW);  // turn off internal pull-up resistor
    
      // Count how long it takes the diode to bleed back down to a logic 0 at pin 2
      lastUpdate = millis();
      for ( j = 0; j < 100000; j++) {
        //delay(20);
        if ( digitalRead(LED_N_SIDE)==0) 
        {
          Interval = millis()-lastUpdate;
          sprintf(serialbuff, "j = %d \nCalculation time:  %d ms ", j, Interval);
//          Serial.println(serialbuff);
//          Serial.print("Calculation time: ");
//          Serial.print(Interval, DEC);
//          Serial.println(" ms");
//          Serial.println(j, DEC);
          break;
        }
      }
      // You could use 'j' for something useful, but here we are just using the
      // delay of the counting.  In the dark it counts higher and takes longer, 
      // increasing the portion of the loop where the LED is off compared to 
      // the 1000 microseconds where we turn it on.
    
      // Turn the light on for 1000 microseconds
    //  digitalWrite(LED_P_SIDE, HIGH);
    //  digitalWrite(LED_N_SIDE, LOW);
    //  pinMode(LED_P_SIDE, OUTPUT);
    //  pinMode(LED_N_SIDE, OUTPUT);
    //  delayMicroseconds(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(10);
      digitalWrite(LED_BUILTIN, LOW);
      delay(10);
      // we could turn it off, but we know that is about to happen at the loop() start
   return serialbuff;
}
