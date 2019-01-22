#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>    

#include <OSCBundle.h>
#include <OSCBoards.h>

/*
* UDPReceiveOSC
* Set a tone according to incoming OSC control
*                           Adrian Freed
*/

//UDP communication


EthernetUDP Udp;
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // you can find this written on the board of some Arduino Ethernets or shields

//the Arduino's IP
IPAddress ip(192, 168, 1, 21);

//port numbers
const unsigned int inPort = 8888;


void routeTone(OSCMessage &msg, int addrOffset ){
     char strBuffer[16];
     Serial.println("routeTone");
     if (msg.isString(0)){        
        msg.getString(0, strBuffer);
        Serial.print("OSC receive: "); Serial.println(strBuffer);
     }
}

void setup() {
  //setup ethernet part
  Ethernet.begin(mac,ip);
  Udp.begin(inPort);
  Serial.begin(19200);
  Serial.println("Start Serial");

}
//reads and dispatches the incoming message
void loop(){ 
    OSCBundle bundleIN;
   int size;
 
   if( (size = Udp.parsePacket())>0)
   {
     Serial.println("Udp.parsePacket()");
     while(size--)
       bundleIN.fill(Udp.read());

      if(!bundleIN.hasError())
        bundleIN.route("/test", routeTone);
   }
}
