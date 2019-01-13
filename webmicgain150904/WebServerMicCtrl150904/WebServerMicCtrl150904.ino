#include <EEPROM.h>

#include <WebServer.h>

//#include <NetSetup.h>
#include <EEPROMAnything.h>
#include <System.h>
#include <MyNetSetup.h>

#include <TFT.h>


/*

 */

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"
#include "MicRemote.h"
#include "stdio.h"
#include "stdlib.h"
#include "MyNetSetup.h"
#include <EEPROM.h>

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
static uint8_t ip[] = { 192, 168, 1, 210 };

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

//byte mic_config_set;
//unsigned char ccu_ch[24]; // global array for saving gain value
//char ccu_name[12][16];    //string array for ccu names. names length is 16 char
char ccu;
char mic1;
char mic2;

// commands are functions that get called by the webserver framework
// they can read any posted data from client, and they output to server

void outputPins(WebServer &server, WebServer::ConnectionType type, bool addControls = false)
{
  P(htmlHead) =
    "<html>"
    "<head>"
    "<title>Mic control</title>"
    "<meta http-equiv='Content-Type' content='text/html; charset=windows-1251'>"
    "<meta name='Keywords' content='Mic control'>"
    
    "<script type=\"text/javascript\">"
    "function send_param(ccu_ch, index)" 
     "{"
        "var body = ccu_ch + '=' + index;"
        "xhr.open('POST', '" PREFIX "/form', true);"
        "xhr.setRequestHeader('Content-Type', 'text-plain');"
        "xhr.send(body);"                
     "}" 
     
     "function send_name(ccu_namber, ccu_name)" 
     "{"
        "var body = ccu_namber + '=' + ccu_name;"
        "xhr.open('POST', '" PREFIX "/ccuName', true);"
        "xhr.setRequestHeader('Content-Type', 'text-plain');"
        "xhr.send(body);"                
     "}"
     
     "function save_param()" 
     "{"
        "var body = 'eeprom' + '=' + '50';"       
        "xhr.open('POST', '" PREFIX "/saveParam', true);"
        "xhr.setRequestHeader('Content-Type', 'text-plain');"
        "xhr.send(body);" 
        "window.alert('Mic Settings are Saved');"        
     "}"
     
    "</script>"   
    
    "</head>"
    "<body>";

  server.httpSuccess();
  server.printP(htmlHead);
  
  int i;
  int j;
  int k;
  char ccu_ch_name[4];

//    server << "<h1 align='right'>Sony cameras remout mic control</h1>";
//    server << "<table border='1' bgcolor='white' align='right' bordercolor='black' cellpadding='10'>";
    server << "<h1>Sony cameras remout mic control</h1>";
    server << "<table border='1' bgcolor='white' bordercolor='black' cellpadding='10'>";
    server << "<tr>";
//    server << "<form action='" PREFIX "/form' method='POST' name='form1'>";
    server << "<form action='' method='POST' name='form1'>";

    server << "<td>";
    server << "<p align=right>";// <<  "CCU1" << "<br> </p>";
    server << "<input type='text' style='background-color:Khaki;' name='ccu01' maxlength='11' size='11' value='" << cfg.ccu_name[0] << "' onchange=\"send_name('ccu01', document.form1.ccu01.value)\">";
    server << "<br> </p>";
        server << "<p>"<< "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s011"<< "' onchange=\"send_param('s011', document.form1.s011.selectedIndex)\">"; 
        selectedOptions(server, cfg.ccu_ch[0]);  //output select list with previous selected options 
//      server << "<option>- 20 dB</option>";  
//	server << "<option>- 30 dB</option>";
//	server << "<option>- 40 dB</option>";
//	server << "<option>- 50 dB</option>";
//	server << "<option>- 60 dB</option>";        
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s012"<< "' onchange=\"send_param('s012', document.form1.s012.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[1]);          
       	server << "</select> </p>";
   server << "</td>";  
   
   server << "<td>";
   server << "<p align=right>";// <<  "CCU2" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu02' maxlength='11' size='11' value='" << cfg.ccu_name[1] << "' onchange=\"send_name('ccu02', document.form1.ccu02.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s021"<< "' onchange=\"send_param('s021', document.form1.s021.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[2]);        
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s022"<< "' onchange=\"send_param('s022', document.form1.s022.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[3]);        
       	server << "</select> </p>";
   server << "</td>";    
   
   server << "<td>";
   server << "<p align=right>";// <<  "CCU3" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu03' maxlength='11' size='11' value='" << cfg.ccu_name[2] << "' onchange=\"send_name('ccu03', document.form1.ccu03.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s031"<< "' onchange=\"send_param('s031', document.form1.s031.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[4]);        
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s032"<< "' onchange=\"send_param('s032', document.form1.s032.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[5]);        
       	server << "</select> </p>";
   server << "</td>";
   
   server << "<td>";
   server << "<p align=right>";// <<  "CCU4" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu04' maxlength='11' size='11' value='" << cfg.ccu_name[3] << "' onchange=\"send_name('ccu04', document.form1.ccu04.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s041"<< "' onchange=\"send_param('s041', document.form1.s041.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[6]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s042"<< "' onchange=\"send_param('s042', document.form1.s042.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[7]);        
       	server << "</select> </p>";
   server << "</td>";
   
   server << "<td>";
   server << "<p align=right>";// <<  "CCU5" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu05' maxlength='11' size='11' value='" << cfg.ccu_name[4] << "' onchange=\"send_name('ccu05', document.form1.ccu05.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s051"<< "' onchange=\"send_param('s051', document.form1.s051.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[8]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s052"<< "' onchange=\"send_param('s052', document.form1.s052.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[9]);        
       	server << "</select> </p>";
   server << "</td>";   
    
   server << "<td>";
   server << "<p align=right>";// <<  "CCU6" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu06' maxlength='11' size='11' value='" << cfg.ccu_name[5] << "' onchange=\"send_name('ccu06', document.form1.ccu06.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s061"<< "' onchange=\"send_param('s061', document.form1.s061.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[10]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s062"<< "' onchange=\"send_param('s062', document.form1.s062.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[11]);        
       	server << "</select> </p>";
   server << "</td> </tr> <p> <br> </p>";
   
   server << "<tr>";
    
   server << "<td>";
   server << "<p align=right>";// <<  "CCU7" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu07' maxlength='11' size='11' value='" << cfg.ccu_name[6] << "' onchange=\"send_name('ccu07', document.form1.ccu07.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s071"<< "' onchange=\"send_param('s071', document.form1.s071.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[12]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s072"<< "' onchange=\"send_param('s072', document.form1.s072.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[13]);        
       	server << "</select> </p>";
   server << "</td>";
    
   server << "<td>";
   server << "<p align=right>";// <<  "CCU8" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu08' maxlength='11' size='11' value='" << cfg.ccu_name[7] << "' onchange=\"send_name('ccu08', document.form1.ccu08.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s081"<< "' onchange=\"send_param('s081', document.form1.s081.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[14]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s082"<< "' onchange=\"send_param('s082', document.form1.s082.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[15]);        
       	server << "</select> </p>";
   server << "</td>";
    
   server << "<td>";
   server << "<p align=right>";// <<  "CCU9" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu09' maxlength='11' size='11' value='" << cfg.ccu_name[8] << "' onchange=\"send_name('ccu09', document.form1.ccu09.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s091"<< "' onchange=\"send_param('s091', document.form1.s091.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[16]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s092"<< "' onchange=\"send_param('s092', document.form1.s092.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[17]);        
       	server << "</select> </p>";
   server << "</td>"; 
    
   server << "<td>";
   server << "<p align=right>";// <<  "CCU10" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu10' maxlength='11' size='11' value='" << cfg.ccu_name[9] << "' onchange=\"send_name('ccu10', document.form1.ccu10.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s101"<< "' onchange=\"send_param('s101', document.form1.s101.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[18]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s102"<< "' onchange=\"send_param('s102', document.form1.s102.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[19]);        
       	server << "</select> </p>";
   server << "</td>";
    
   server << "<td>";
   server << "<p align=right>";// <<  "CCU11" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu11' maxlength='11' size='11' value='" << cfg.ccu_name[10] << "' onchange=\"send_name('ccu11', document.form1.ccu11.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s111"<< "' onchange=\"send_param('s111', document.form1.s111.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[20]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s112"<< "' onchange=\"send_param('s112', document.form1.s112.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[21]);        
       	server << "</select> </p>";
   server << "</td>";
    
   server << "<td>";
   server << "<p align=right>";// <<  "CCU12" << "<br> </p>";
   server << "<input type='text' style='background-color:Khaki;' name='ccu12' maxlength='11' size='11' value='" << cfg.ccu_name[11] << "' onchange=\"send_name('ccu12', document.form1.ccu12.value)\">";
   server << "<br> </p>";
        server << "<p>" << "CH1 ";
        //server << " <select name='" <<ccu_ch<< "' onchange=\"javascript:window.alert ('" <<ccu_ch<< "'+' '+ document.form1." <<ccu_ch<< ".selectedIndex)\">";
        server << "<select name='" <<"s121"<< "' onchange=\"send_param('s121', document.form1.s121.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[22]);     
       	server << "</select> </p>";
        server << "<p>" << "CH2 ";
        server << "<select name='" <<"s122"<< "' onchange=\"send_param('s122', document.form1.s122.selectedIndex)\">";
        selectedOptions(server, cfg.ccu_ch[23]);        
       	server << "</select> </p>";
   server << "</td>";   
  //for (i=1; i<=12; i++)

  server << "</tr>";
//  server << "<p> <br> <a href=\"setupNet.html\">NETWORK SETUP</a> </p>";
  server << "</table>";// <p> <br> </p>";
//  server << "<p> <input type='submit' name='button' value='Save Settings'  onClick=\"save_param()\"> </p>";
  server << "</form>";
  server << "<p> <br> <br> <br> <input type='submit' name='button' value='Save Settings'  onClick=\"save_param()\"> </p>";
  server << "<p> <br> <br> <br> <br> <br> <br> <br> <br> <a href=\"setupNet.html\">NETWORK SETUP</a> </p>";
 
  
  server << "<script type=\"text/javascript\">";
  server << "xhr = new XMLHttpRequest();";
  server << "</script>";
  
  server << "</body></html>";

};

//output select list with previous selected options 
void selectedOptions(WebServer &server, unsigned char ccu_ch_stored)
{
          //set the previous seved value as selected option
        if (ccu_ch_stored == 0)
          server << "<option selected>- 20 dB</option>";
        else
          server << "<option>- 20 dB</option>";
          
        if (ccu_ch_stored == 1)
          server << "<option selected>- 30 dB</option>";
        else
          server << "<option>- 30 dB</option>";

        if (ccu_ch_stored == 2)
          server << "<option selected>- 40 dB</option>";
        else
          server << "<option>- 40 dB</option>";

        if (ccu_ch_stored == 3)
          server << "<option selected>- 50 dB</option>";
        else
          server << "<option>- 50 dB</option>";
          
        if (ccu_ch_stored == 4)
          server << "<option selected>- 60 dB</option>";
        else
          server << "<option>- 60 dB</option>";
};

void formCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (type == WebServer::POST)
  {    

    bool repeat;
    char pname[16];
    char value[16];
    int  ch_number;

    do
    {
      repeat = server.readPOSTparam(pname, 16, value, 16);
      //ch_number = atoi((pname+1));
      ch_number = (int) strtoul((pname+1), NULL, 10);      

      if (repeat)
     {
      
       switch (ch_number)
          {
            case 11:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[1-1] = mic1;
                  mic2 = cfg.ccu_ch[2-1];
                  gain_change_ccu(CCU1_out, mic1, mic2);
                  break;
            case 12:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[2-1] = mic2;
                  mic1 = cfg.ccu_ch[1-1];
                  gain_change_ccu(CCU1_out, mic1, mic2);
                  break;
                  
             case 21:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[3-1] = mic1;
                  mic2 = cfg.ccu_ch[4-1];
                  gain_change_ccu(CCU2_out, mic1, mic2);
                  break;
            case 22:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[4-1] = mic2;
                  mic1 = cfg.ccu_ch[3-1];
                  gain_change_ccu(CCU2_out, mic1, mic2);
                  break;                 
                  
             case 31:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[5-1] = mic1;
                  mic2 = cfg.ccu_ch[6-1];
                  gain_change_ccu(CCU3_out, mic1, mic2);
                  break;
            case 32:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[6-1] = mic2;
                  mic1 = cfg.ccu_ch[5-1];
                  gain_change_ccu(CCU3_out, mic1, mic2);
                  break; 

             case 41:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[7-1] = mic1;
                  mic2 = cfg.ccu_ch[8-1];
                  gain_change_ccu(CCU4_out, mic1, mic2);
                  break;
            case 42:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[8-1] = mic2;
                  mic1 = cfg.ccu_ch[7-1];
                  gain_change_ccu(CCU4_out, mic1, mic2);
                  break; 
                  
             case 51:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[9-1] = mic1;
                  mic2 = cfg.ccu_ch[10-1];
                  gain_change_ccu(CCU5_out, mic1, mic2);
                  break;
            case 52:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[10-1] = mic2;
                  mic1 = cfg.ccu_ch[9-1];
                  gain_change_ccu(CCU5_out, mic1, mic2);
                  break; 

             case 61:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[11-1] = mic1;
                  mic2 = cfg.ccu_ch[12-1];
                  gain_change_ccu(CCU6_out, mic1, mic2);
                  break;
            case 62:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[12-1] = mic2;
                  mic1 = cfg.ccu_ch[11-1];
                  gain_change_ccu(CCU6_out, mic1, mic2);
                  break; 

             case 71:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[13-1] = mic1;
                  mic2 = cfg.ccu_ch[14-1];
                  gain_change_ccu(CCU7_out, mic1, mic2);
                  break;
            case 72:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[14-1] = mic2;
                  mic1 = cfg.ccu_ch[13-1];
                  gain_change_ccu(CCU7_out, mic1, mic2);
                  break;

             case 81:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[15-1] = mic1;
                  mic2 = cfg.ccu_ch[16-1];
                  gain_change_ccu(CCU8_out, mic1, mic2);
                  break;
            case 82:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[16-1] = mic2;
                  mic1 = cfg.ccu_ch[15-1];
                  gain_change_ccu(CCU8_out, mic1, mic2);
                  break;     
 
             case 91:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[17-1] = mic1;
                  mic2 = cfg.ccu_ch[18-1];
                  gain_change_ccu(CCU9_out, mic1, mic2);
                  break;
            case 92:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[18-1] = mic2;
                  mic1 = cfg.ccu_ch[17-1];
                  gain_change_ccu(CCU9_out, mic1, mic2);
                  break;     
 
             case 101:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[19-1] = mic1;
                  mic2 = cfg.ccu_ch[20-1];
                  gain_change_ccu(CCU10_out, mic1, mic2);
                  break;
            case 102:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[20-1] = mic2;
                  mic1 = cfg.ccu_ch[19-1];
                  gain_change_ccu(CCU10_out, mic1, mic2);
                  break;     
 
             case 111:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[21-1] = mic1;
                  mic2 = cfg.ccu_ch[22-1];
                  gain_change_ccu(CCU11_out, mic1, mic2);
                  break;
            case 112:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[22-1] = mic2;
                  mic1 = cfg.ccu_ch[21-1];
                  gain_change_ccu(CCU11_out, mic1, mic2);
                  break; 
 
             case 121:
                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
                  cfg.ccu_ch[23-1] = mic1;
                  mic2 = cfg.ccu_ch[24-1];
                  gain_change_ccu(CCU12_out, mic1, mic2);
                  break;
            case 122:
                  mic2 = (char) strtoul(value, NULL, 10);
                  cfg.ccu_ch[24-1] = mic2;
                  mic1 = cfg.ccu_ch[23-1];
                  gain_change_ccu(CCU12_out, mic1, mic2);
                  break;                      
                    
            default:
                  break;
          };


      };
    } while (repeat);
    
//    server.httpSeeOther(PREFIX "/form");
  }
  else
  ;
//    outputPins(server, type, false);
};

//===========================================================================================================================================================

void ccuNameCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (type == WebServer::POST)
  {    

    bool repeat;
    char pname[16];
    char value[16];
    int  ccu_number;

    do
    {
      repeat = server.readPOSTparam(pname, 16, value, 16);
      //ch_number = atoi((pname+1));
      ccu_number = (int) strtoul((pname+3), NULL, 10);      

      if (repeat)
     {
      
       switch (ccu_number)
          {
            case 1:
                  strcpy(cfg.ccu_name[0], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 2:
                  strcpy(cfg.ccu_name[1], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;                  
            case 3:
                  strcpy(cfg.ccu_name[2], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 4:
                  strcpy(cfg.ccu_name[3], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 5:
                  strcpy(cfg.ccu_name[4], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 6:
                  strcpy(cfg.ccu_name[5], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 7:
                  strcpy(cfg.ccu_name[6], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 8:
                  strcpy(cfg.ccu_name[7], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;                 
            case 9:
                  strcpy(cfg.ccu_name[8], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 10:
                  strcpy(cfg.ccu_name[9], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 11:
                  strcpy(cfg.ccu_name[10], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            case 12:
                  strcpy(cfg.ccu_name[11], value); //save changed ccu name to global array ccu_name[12][16] 
                  break;
            default:
                  break;
          };

      };//if(repeat)
    } while (repeat);
    
//    server.httpSeeOther(PREFIX "/form");
  }
  else
  ;
//    outputPins(server, type, false);
};
//===========================================================================================================================================================

void saveParamCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{  
/*  if (type == WebServer::POST)
    {    
  
      bool repeat;
      char pname[16];
      char value[16];
      
      do
      {            
        repeat = server.readPOSTparam(pname, 16, value, 16);  
        if (repeat)
         {
         };
      } while (repeat);        
 */       
     EEPROM_writeAnything(EEPROM_SET, cfg);
//    }
}

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{  

    outputPins(server, type, false);  

}

/** 
* set_mic_default() function
*
* The default settings. 
* This settings are used when no config is present.
*/
void set_mic_default()
{
  int i, j;
  char buf[20];
  char* num; 
  
  cfg.mic_config_set = 1; // dont change! It's used to check if the config is already set
  
  for (j = 0; j <= 12; j++)
  {
    strcpy(cfg.ccu_name[j], "CCU");
    num = itoa(j+1,buf,10);
    strcat(cfg.ccu_name[j], num);
  }; 
  // global unsigned char ccu_ch[24];
  for (i = 0; i <= 23; i++)
  {
    cfg.ccu_ch[i] = 2; //-40dB потом инициализация будет значениями из EEPROM
  };
}

/**
* read_EEPROM_Settings function
* This function is used to read the EEPROM settings at startup
*
* Overview:
* - Load the stored data from EEPROM
* - Check if a config is stored. If the conditions is ture, set the defaults
*/
void read_mic_settings() 
{ 
  // read the current config
  EEPROM_readAnything(EEPROM_SET, cfg);
  
  // check if config is present or if reset button is pressed
  if (cfg.mic_config_set != 1)// || digitalRead(RESET_PIN) == LOW) 
  {
    // set default values
    set_mic_default();   
    // write the config to eeprom
    EEPROM_writeAnything(EEPROM_SET, cfg);
  } 
}

void setup()
{
  delay(200); // some time to settle
  // set pins for digital outputs
  Set_pin_out();
 
  read_mic_settings(); 
  
//  set_mic_default();
  
  //delay(500);
  //set out pins to saved value
  gain_change_ccu(CCU1_out, cfg.ccu_ch[0], cfg.ccu_ch[1]);
  gain_change_ccu(CCU2_out, cfg.ccu_ch[2], cfg.ccu_ch[3]);
  gain_change_ccu(CCU3_out, cfg.ccu_ch[4], cfg.ccu_ch[5]);  
  gain_change_ccu(CCU4_out, cfg.ccu_ch[6], cfg.ccu_ch[7]); 
  gain_change_ccu(CCU5_out, cfg.ccu_ch[8], cfg.ccu_ch[9]); 
  gain_change_ccu(CCU6_out, cfg.ccu_ch[10], cfg.ccu_ch[11]);
  gain_change_ccu(CCU7_out, cfg.ccu_ch[12], cfg.ccu_ch[13]);
  gain_change_ccu(CCU8_out, cfg.ccu_ch[14], cfg.ccu_ch[15]);
  gain_change_ccu(CCU9_out, cfg.ccu_ch[16], cfg.ccu_ch[17]);
  gain_change_ccu(CCU10_out, cfg.ccu_ch[18], cfg.ccu_ch[19]);
  gain_change_ccu(CCU11_out, cfg.ccu_ch[20], cfg.ccu_ch[21]);
  gain_change_ccu(CCU12_out, cfg.ccu_ch[22], cfg.ccu_ch[23]);
  
  /* initialize the Ethernet adapter with the settings from eeprom */

  setupNetwork();
  delay(200); // some time to settle
  webserver = new WebServer(PREFIX, eeprom_config.webserverPort);
  
  webserver->setDefaultCommand(&defaultCmd);
  webserver->addCommand("form", &formCmd);  
  webserver->addCommand("ccuName", &ccuNameCmd); 
  webserver->addCommand("saveParam", &saveParamCmd);   
 
   /* setup our default command that will be run when the user accesses
   * a page NOT on the server */
  webserver->setFailureCommand(&errorHTML);
  /* display a network setup form. The configuration is stored in eeprom */
  webserver->addCommand("setupNet.html", &setupNetHTML);

  /* start the webserver */
  webserver->begin(); 

};

void loop()
{
  // renew DHCP lease
  renewDHCP(eeprom_config.dhcp_refresh_minutes);

  char buff[200];
  int len = 200;

  /* process incoming connections one at a time forever */
  webserver->processConnection(buff, &len);
  // if you wanted to do other work based on a connecton, it would go here
};
