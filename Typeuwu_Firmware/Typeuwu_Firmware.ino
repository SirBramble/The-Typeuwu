#include "filesystem.h"

#define SERIAL_DEBUG

module keyboard("keyboard");


void setup() {
  Serial.begin(115200);
  filesystemSetup();
  filesystemCreateConfig();
  Serial.println("uwu Setup");
  while( !TinyUSBDevice.mounted() ) Serial.println("crashing");

}

void loop() {
  filesystemLoop();
  keyboard.updateKeymapsFromFile();
  Serial.println("uwu");
  delay(1000);
}
