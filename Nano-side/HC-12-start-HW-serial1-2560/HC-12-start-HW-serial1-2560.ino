/*    Arduino Long Range Wireless Communication using HC-12
                      Example 01
   by Dejan Nedelkovski, www.HowToMechatronics.com
*/
#include <SoftwareSerial.h>

#define setPin 6

byte incomingByte;
String readBuffer = "";

//SoftwareSerial HC12(4,5); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(9600);             // Serial port to computer
  Serial1.begin(9600);               // Serial port to HC12
  pinMode(setPin, OUTPUT);
  digitalWrite(setPin, HIGH); 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  readBuffer = "";                       // Clear readBuffer
  while (Serial1.available()) {        // If HC-12 has data
    Serial.write(Serial1.read());      // Send the data to Serial monitor
  }
  
  while (Serial.available()) {      // If Serial monitor has data
    //HC12.write(Serial.read());      // Send that data to HC-12
    incomingByte = Serial.read();          // Store each icoming byte from HC-12
    readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
  }

  //Serial.println(readBuffer);
  checkATCommand();
  
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
    Serial1.print(readBuffer);              // Send AT Command to HC-12
    delay(200);
    while (Serial1.available()) {           // If HC-12 has data (the AT Command response)
      Serial.write(Serial1.read());           // Send the data to Serial monitor
    }
    digitalWrite(setPin, HIGH);          // Exit AT Command mode
  }
}
