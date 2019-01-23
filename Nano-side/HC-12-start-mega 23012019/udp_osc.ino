void sendOSCommand(int id){

  if (transmittersState[id].OSC_Port.toInt() == 0)
      return;

  int bufflength = transmittersState[id].OSC_adress.length() + 1;
  const char buff[bufflength];
  
  transmittersState[id].OSC_adress.toCharArray(buff, bufflength);
  
  IPAddress outIp;
  if (outIp.fromString(transmittersState[id].OSC_IP)) {   // str = char * or String
      // it is a valid IP address
      udpOutPort = transmittersState[id].OSC_Port.toInt();
      //the message wants an OSC address as first argument
//      Serial.println("sendOSCommand()");
//      Serial.print("OSC_IP: "); Serial.println(outIp);
//      Serial.print("OSC_Port: "); Serial.println(udpOutPort);
//      Serial.print("OSCMessage: "); Serial.println(transmittersState[id].OSC_command);
      OSCMessage msg(buff);
      msg.add(transmittersState[id].OSC_command.c_str());
      
      Udp.beginPacket(outIp, udpOutPort);
        msg.send(Udp); // send the bytes to the SLIP stream
        msg.send(Serial); // send the bytes to the SLIP stream
      Udp.endPacket(); // mark the end of the OSC Packet
//      msg.empty(); // free space occupied by message
      msg.set(0, "");
      delay(20);

//      OSCMessage msg(buff);
//      msg.add("");
      
      Udp.beginPacket(outIp, udpOutPort);
        msg.send(Udp); // send the bytes to the SLIP stream
        msg.send(Serial); // send the bytes to the SLIP stream
      Udp.endPacket(); // mark the end of the OSC Packet
      msg.empty(); // free space occupied by message
    
      delay(20);
  }
}

void receiveOSCcommand(){

   char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
   
   OSCBundle bundleIN;
   char udpBuff[48];
   String strbuff = "";
   //OSCMessage msg("/mega");
   int size;
 
   if( (size = Udp.parsePacket())>0)
   {
     //Serial.print("size = "); Serial.println(size);
//     for (int i=0; i<=size; i++) {
//          packetBuffer[i] = Udp.read();
//     }
    // read the packet into packetBufffer
    Udp.read(udpBuff,48);
//    strbuff = packetBuffer;
//    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
//    strbuff += packetBuffer;
//    Serial.println("Contents:");
//    Serial.println(packetBuffer);
     
    // while(size--) {
       //strbuff += Udp.read();
       //bundleIN.fill(Udp.read());
     //};
     Serial.println(udpBuff);
//      if(!bundleIN.hasError())
//        bundleIN.dispatch("/test", routeOSCmega);
   }
     //Serial.println();
}

void routeOSCmega(OSCMessage &msg){
     char strBuffer[16];
     Serial.println("routeOSCmega");
     if (msg.isString(0)){        
        msg.getString(0, strBuffer);
        Serial.print("OSC receive: "); Serial.println(strBuffer);
     }
}

void msgReceive(){
  OSCMessage msgIN;
  int size;
  if((size = Udp.parsePacket())>0){
    while(size--)
      msgIN.fill(Udp.read());
    if(!msgIN.hasError()){
      msgIN.route("/test", routeOSCmega);
      //msgIN.route("/digital", handleDigital);
    }
  }
}
