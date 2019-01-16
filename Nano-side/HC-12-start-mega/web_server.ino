/*  xml file structure
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
  if (type == WebServer::POST)
  {    

    bool repeat;
    char pname[16];
    char value[16];
    int  ch_number;

    repeat = server.readPOSTparam(pname, 16, value, 16);
  }
}  

/*  to-do fuction for set mode for all transmitter
 *  shuld perfom sequentalli sending 0100MODE-X 0200MODE-X ..
 *  command with delay betwin command
 *
 */
