#include "stdio.h"
#include "stdlib.h"
#include "MyNetSetup.h"
#include <System.h>

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
static uint8_t ip[] = { 192, 168, 1, 1 };

#define PREFIX ""
//WebServer webserver(PREFIX, 80);

/* This creates an pointer to instance of the webserver. */
WebServer * webserver;

struct config_mic
{
byte mic_config_set;
unsigned char ccu_ch[24]; // global array for saving gain value
char ccu_name[12][16];    //string array for ccu names. names length is 16 char
} cfg;

