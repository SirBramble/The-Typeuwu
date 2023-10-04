#include "modules.h"
#include "hidInterface.h"
#include "lighting.h"

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
  lightingSetup();
  keyboard.updateKeymapsFromFile();
  prev = 0;
}



void loop() {
  //filesystemLoop();
  //keyboard.updateKeymapsFromFile();

  if(check_fs_changed()){
    keyboard.updateKeymapsFromFile();
    set_fs_changed(false);
  }

  if(millis() > prev + 1000){
    prev = millis();
    Serial.println("uwu");
    //Serial.println(digitalRead(11));
  }

  //keyboard.testKeys();
  keyboard.update();

  for(int i = 0; i < AMMOUNT_KEYS; i++){
    if(keyboard.isPressed_hold(i)){
      Serial.println("pressing");
      hid.press(keyboard.getKeyPointer(1, i + 1));
    }
    else if(keyboard.isReleased_hold(i)){
      hid.release(keyboard.getKeyPointer(1, i + 1));
    }
  }


  //key * tmp = keyboard.getKeyPointer(1,1);
  //if(tmp != nullptr){
  //  for(uint16_t i = 0; i < tmp->getKeycodesSize(); i++){
  //    keysycode tmpk = tmp->getKeysycode(i);
  //    Serial.print("Keysycode "); Serial.print(i); Serial.print(":"); Serial.print(tmpk.keycode); Serial.print(" "); Serial.print(tmpk.modifier); Serial.print(" "); Serial.print(tmpk.reportID); Serial.print(" "); Serial.println(tmpk.immediateSend);
  //  }
  //}
  //else Serial.println("There is no point... :(");
  //lightingLoop();
  rainbow(10);
  delay(1);
}
