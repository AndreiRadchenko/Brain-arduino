
#include "stdio.h"
#include "stdlib.h"
#include <MyNetSetup.h>
#include <System.h>
#include <SD.h>

#define EEPROM_ETHER 0
#define EEPROM_SET 50
#define EEPROM_CCU_CH 60
#define EEPROM_CCU_NAME 100

// no-cost stream operator as described at 
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }


// CHANGE THIS TO YOUR OWN UNIQUE VALUE
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// CHANGE THIS TO MATCH YOUR HOST NETWORK 
static uint8_t ip[] = { 192, 168, 1, 21 };

#define PREFIX ""
//WebServer webserver(PREFIX, 80);

/* This creates an pointer to instance of the webserver. */
WebServer * webserver;
File webFile;               // the web page file on the SD card

struct config_mic
{
byte mic_config_set;
unsigned char ccu_ch[24]; // global array for saving gain value
char ccu_name[12][16];    //string array for ccu names. names length is 16 char
} cfg;



void setup() {

  initSDcard();
  // put your setup code here, to run once:
  Ethernet.begin(mac, ip);  // initialize Ethernet device
  webserver = new WebServer();

  webserver->setDefaultCommand(&defaultCmd);
    /* start the webserver */
  webserver->begin(); 

}

void loop() {

  char buff[200];
  int len = 200;
   /* process incoming connections one at a time forever */
  webserver->processConnection(buff, &len);

}

void initSDcard(){
    Serial.begin(19200);       // for debugging
    
    // initialize SD card
    Serial.println("Initializing SD card...");
    if (!SD.begin(4)) {
        Serial.println("ERROR - SD card initialization failed!");
        return;    // init failed
    }
    Serial.println("SUCCESS - SD card initialized.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERROR - Can't find index.htm file!");
        return;  // can't find index file
    }
    Serial.println("SUCCESS - Found index.htm file.");
}

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{  
    server.httpSuccess();
    // send web page
    webFile = SD.open("index.htm");        // open web page file
    if (webFile) {
        while(webFile.available()) {
        server.write(webFile.read()); // send web page to client
        }
      webFile.close();
    }
     

}
