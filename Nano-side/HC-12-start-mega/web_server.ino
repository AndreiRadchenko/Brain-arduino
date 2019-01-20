/*  
 *  xml file structure:
 *   
 *  <?xml version = "1.0" ?> 
 *  <transmitters>
 *      <mode>0</mode>
 *      <mode>0</mode>
 *      <mode>0</mode>
 *      <mode>0</mode>
 *      <threshold>500</threshold>
 *      <threshold>500</threshold>
 *      <threshold>500</threshold>
 *      <threshold>500</threshold>
 *      <sensor>0</sensor>
 *      <sensor>0</sensor>
 *      <sensor>0</sensor>
 *      <sensor>0</sensor>
 *      <battery>0</battery>
 *      <battery>0</battery>
 *      <battery>0</battery>
 *      <battery>0</battery>
 *      <laser_crossed>false</laser_crossed>
 *      <laser_crossed>false</laser_crossed>
 *      <laser_crossed>false</laser_crossed>
 *      <laser_crossed>false</laser_crossed>
 *      <button_pressed>false</button_pressed>
 *      <button_pressed>false</button_pressed>
 *      <button_pressed>false</button_pressed>
 *      <button_pressed>false</button_pressed>
 *      <OSC_IP>192.168.1.1</OSC_IP>
 *      <OSC_IP>192.168.1.1</OSC_IP>
 *      <OSC_IP>192.168.1.1</OSC_IP>
 *      <OSC_IP>192.168.1.1</OSC_IP>
 *      <OSC_command>test</OSC_command>
 *      <OSC_command>test</OSC_command>
 *      <OSC_command>test</OSC_command>
 *      <OSC_command>test</OSC_command>
 *  </transmitters>
*/

// no-cost stream operator as described at 
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }

void initSDcard(){
    //Serial.begin(19200);       // for debugging
    
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

void getAJAXxmlCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  unsigned long duration = millis();
  
  int count;                 // used by 'for' loops
  
  server.println("HTTP/1.1 200 OK");
  
  if (type == WebServer::POST)
  {    
    //Serial.println("getAJAXxmlCmd() responce");
    server.println("Content-Type: text/xml");
    server.println("Connection: keep-alive");

    server.println();

    server << "<?xml version = \"1.0\" ?>";
    server << "<transmitters>";
    //loop for all parameters of 4s trasmitters
    for (count = 0; count < 4; count++) 
        server << "<mode>" << transmittersState[count].mode << "</mode>";
    for (count = 0; count < 4; count++) 
        server << "<threshold>" << transmittersState[count].threshold << "</threshold>";
    for (count = 0; count < 4; count++) 
        server << "<sensor>" << transmittersState[count].sensor << "</sensor>";
    for (count = 0; count < 4; count++)
        server << "<battery>" << transmittersState[count].battery << "</battery>";  
    for (count = 0; count < 4; count++) {
        server << "<laser_crossed>" << transmittersState[count].laser_crossed << "</laser_crossed>";
//        Serial.print("transmittersState[count].laser_crossed: ");
//        Serial.println(transmittersState[count].laser_crossed);
    }
    for (count = 0; count < 4; count++)
        server << "<button_pressed>" << transmittersState[count].button_pressed << "</button_pressed>"; 
    server << "</transmitters>";             
  };
   

    duration = millis() - duration;
    Serial.print("getAJAXxmlCmd() finishad with ");
    Serial.println(duration);
}  

void sendTransmitterModeCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{ 
  server.println("HTTP/1.1 200 OK");
  
  if (type == WebServer::POST)
  {    
    bool repeat;
    char pname[16];
    char value[16];
    String serialbuff = "0";
    int id;
    
    Serial.println("sendTransmitterModeCmd() statrt");
    do
    {
      repeat = server.readPOSTparam(pname, 16, value, 16);     

      if (repeat)
     {
       //sprintf(serialbuff, "0%c00MODE-%c", pname, value);
       serialbuff += pname;
       serialbuff += "00MODE-";
       serialbuff += value;
       Serial.println(serialbuff);
       //Serial.println(String(pname).toInt());
       Serial1.print(serialbuff);
       
//       //reset transmitter data
//       id = String(pname).toInt()-1;
//       transmittersState[id].threshold = 0;
//       transmittersState[id].sensor = 0;
//       transmittersState[id].battery = 0;
//       transmittersState[id].laser_crossed = 0;
//       transmittersState[id].button_pressed = 0;
       
//       switch (pname)
//          {
//             case 1:
//                  mic1 = (char) strtoul(value, NULL, 10); //save changed value to global array ccu_ch and give from array unchaged mic value
//                  cfg.ccu_ch[23-1] = mic1;
//                  mic2 = cfg.ccu_ch[24-1];
//                  gain_change_ccu(CCU12_out, mic1, mic2);
//                  break;
//            case 2:
//                  mic2 = (char) strtoul(value, NULL, 10);
//                  cfg.ccu_ch[24-1] = mic2;
//                  mic1 = cfg.ccu_ch[23-1];
//                  gain_change_ccu(CCU12_out, mic1, mic2);
//                  break;                      
//                    
//            default:
//                  break;
//          };


      };
    } while (repeat);
    
//    server.httpSeeOther(PREFIX "/form");
  }
  else
  ;
//    outputPins(server, type, false);
};
  
void sendTransmitterThresholdCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{ 
  server.println("HTTP/1.1 200 OK");
  
  if (type == WebServer::POST)
  {    
    bool repeat;
    char pname[16];
    char value[16];
    String serialbuff = "0";
    int id;
    
    Serial.println("sendTransmitterThreshold()");
    do
    {
      repeat = server.readPOSTparam(pname, 16, value, 16);     

      if (repeat)
     {
       serialbuff += pname;
       serialbuff += "00THRESHOLD-";
       serialbuff += value;
       Serial.println(serialbuff);
       //Serial.println(String(pname).toInt());
       Serial1.print(serialbuff);


      };
    } while (repeat);
    
//    server.httpSeeOther(PREFIX "/form");
  }
  else
  ;
//    outputPins(server, type, false);
};

void sendOSCparametersCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{ 
  server.println("HTTP/1.1 200 OK");
  
  if (type == WebServer::POST)
  {    
    bool repeat;
    char pname[16];
    char value[128];
    String buffName = "";
    String buffValue = "";
    //String buff;
    int index;
    int id;
    
    Serial.println("sendTransmitterThreshold()");
    do
    {
      repeat = server.readPOSTparam(pname, 16, value, 128);     

      if (repeat)
     {
       buffName = String(pname);
       buffValue = String(value);
       
       id = buffName.toInt();
       index = buffValue.indexOf(';');
       transmittersState[id].OSC_IP = buffValue.substring(0, index);
       transmittersState[id].OSC_command = buffValue.substring(index+1);
      
//       for (int i=0; i <= buffValue.length(); i++) {
//           //simbol = readBuffer.charAt(i);
//           if (buffValue.charAt(i) != ';') {
//              buff += buffValue.charAt(i);
//           }
//           else {
//              transmittersState[id].OSC_IP = buff;
//              transmittersState[id].OSC_command = buffValue.substring(i+1);
//              break; 
//           };
//       }

      };
    } while (repeat);

    Serial.print("transmitterID: ");
    Serial.println(id);
    Serial.print("OSC_IP: ");
    Serial.println(transmittersState[id].OSC_IP);
    Serial.print("OSC_command: ");
    Serial.println(transmittersState[id].OSC_command);    
//    server.httpSeeOther(PREFIX "/form");
  }
  else
  ;
//    outputPins(server, type, false);
};

/*  to-do fuction for set mode for all transmitter
 *  shuld perfom sequentalli sending 0100MODE-X 0200MODE-X ..
 *  command with delay betwin command
 *
 */
