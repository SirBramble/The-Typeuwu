#include "modules.h"
#include "hidInterface.h"

#define SERIAL_DEBUG
_keyboard keyboard("keyboard");
hidInterface hid;
uint64_t prev;

void setup() {
  Serial.begin(115200);
  //while(!Serial);       //Dont use, if you want the filesystem to mount
  Serial.println("uwu Setup 1");
  filesystemSetup();
  Serial.println("uwu Setup 2");
  filesystemCreateConfig();
  Serial.println("uwu Setup 3");
  hid.init();
  Serial.println("uwu Setup 4");
  while( !TinyUSBDevice.mounted() ) Serial.println("crashing");
  keyboard.init();
  Serial.println("uwu Setup 5");
  prev = 0;
}



void loop() {
  filesystemLoop();
  //keyboard.updateKeymapsFromFile();
  if(millis() > prev + 1000){
    prev = millis();
    Serial.println("uwu");
    //Serial.println(digitalRead(11));
  }

  keyboard.testKeys();
  

  //key * tmp = keyboard.getKeyPointer(1,1);
  //if(tmp != nullptr){
  //  for(uint16_t i = 0; i < tmp->getKeycodesSize(); i++){
  //    keysycode tmpk = tmp->getKeysycode(i);
  //    Serial.print("Keysycode "); Serial.print(i); Serial.print(":"); Serial.print(tmpk.keycode); Serial.print(" "); Serial.print(tmpk.modifier); Serial.print(" "); Serial.print(tmpk.reportID); Serial.print(" "); Serial.println(tmpk.immediateSend);
  //  }
  //}
  //else Serial.println("There is no point... :(");
  
  delay(1);
}
