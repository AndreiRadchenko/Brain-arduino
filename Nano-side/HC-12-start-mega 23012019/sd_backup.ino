/*
 * Backup file sructure:
 * 
 * {"threshold":[500,500,500,500],"OSC_IP":["192.168.1.100","192.168.1.100","192.168.1.100","192.168.1.100"],
 *    "OSC_Port":["9999","9999","9999","9999"],"OSC_adress":["/test","/test","/test","/test"],"OSC_command":["trans1","trans2","trans3","trans4"]}
 * 
 */

 
// Loads the configuration from a file
void loadConfiguration(const char *filename) {
  // Open file for reading
  File file = SD.open(filename);

  // Allocate the memory pool on the stack.
  // Don't forget to change the capacity to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<512> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.parseObject(file);

  if (!root.success())
    Serial.println(F("Failed to read file, using default configuration"));

  // Copy values from the JsonObject to the Config
  for (int i=0; i<4; i++){
      transmittersState[i].threshold = root["threshold"][i];
      transmittersState[i].OSC_IP = root["OSC_IP"][i].as<String>();
      transmittersState[i].OSC_Port = root["OSC_Port"][i].as<String>();
      transmittersState[i].OSC_adress = root["OSC_adress"][i].as<String>();
      transmittersState[i].OSC_command = root["OSC_command"][i].as<String>();
  };

  // Close the file (File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void saveConfiguration(const char *filename) {
  // Delete existing file, otherwise the configuration is appended to the file
  SD.remove(filename);

  // Open file for writing
  File file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate the memory pool on the stack
  // Don't forget to change the capacity to match your JSON document.
  // Use https://arduinojson.org/assistant/ to compute the capacity.
  StaticJsonBuffer<512> jsonBuffer;

  // Parse the root object
  JsonObject &root = jsonBuffer.createObject();

  // Set the values
  JsonArray& threshold = root.createNestedArray("threshold");
  JsonArray& OSC_IP = root.createNestedArray("OSC_IP");
  JsonArray& OSC_Port = root.createNestedArray("OSC_Port");
  JsonArray& OSC_adress = root.createNestedArray("OSC_adress");
  JsonArray& OSC_command = root.createNestedArray("OSC_command");

  for (int i=0; i<4; i++){
      threshold.add(transmittersState[i].threshold);
      OSC_IP.add(transmittersState[i].OSC_IP);
      OSC_Port.add(transmittersState[i].OSC_Port);
      OSC_adress.add(transmittersState[i].OSC_adress);
      OSC_command.add(transmittersState[i].OSC_command);
  };

  // Serialize JSON to file
  if (root.printTo(file) == 0) {
    Serial.println(F("Failed to write to file"));
  }

  // Close the file (File's destructor doesn't close the file)
  file.close();
}
