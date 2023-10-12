#include "modules.h"
#include "hidInterface.h"
#include "lighting.h"
#include "i2cinterface.h"

#define SERIAL_DEBUG
#define ADDRESS_MODULE1 0x20



_keyboard keyboard("keyboard");
_numpad numpad1("numpad1", ADDRESS_MODULE1);
i2cInterface i2c;
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
  numpad1.updateKeymapsFromFile();
  i2c.init();

  //pinMode(PIN_SDA, OUTPUT);
  //pinMode(PIN_SCL, OUTPUT);

  prev = 0;
}





void loop() {
  //filesystemLoop();
  //keyboard.updateKeymapsFromFile();

  if(check_fs_changed()){
    keyboard.updateKeymapsFromFile();
    numpad1.updateKeymapsFromFile();
    set_fs_changed(false);
  }

  if(millis() > prev + 1000){
    prev = millis();
    i2c.probe();
    //void test_i2c_pins();
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
  
  i2c.update(numpad1.address());
  numpad1.update(i2c.getStates(numpad1.address()));
  if(i2c.isAddressValid(numpad1.address())){
    for(int i = 0; i < I2C_BUFFER_SIZE; i++){
      key *currentkey = numpad1.getKeyPointer(1, i + 1);
      if(currentkey == nullptr) continue;
      if(currentkey->isMIDI && currentkey->isAnalog){         //don't forget to compare val with previous val when setting. Yes mum...
        hid.sendMidi_Analog(numpad1.getKeyPointer(1, i + 1), map(i2c.getVal(i + 1, numpad1.address()), 0, 255, 0, 127));
        //Serial.print(i2c.getVal(19, numpad1.address()));Serial.println(i);
      }
      else if(numpad1.isPressed_hold(i)){
        Serial.println("pressing");
        hid.press(numpad1.getKeyPointer(1, i + 1));
      }
      else if(numpad1.isReleased_hold(i)){
        Serial.println("releasing");
        hid.release(numpad1.getKeyPointer(1, i + 1));
      }
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

int state_test = 0;        // DELETE ME!!!!!

void test_i2c_pins(){
  switch(state_test){
      case 0:
        digitalWrite(PIN_SDA, HIGH);
        state_test = 1;
        break;
      case 1:
        digitalWrite(PIN_SDA, LOW);
        state_test = 2;
        break;
      case 2:
        digitalWrite(PIN_SCL, HIGH);
        state_test = 3;
        break;
      case 3:
        digitalWrite(PIN_SCL, LOW);
        state_test = 0;
        break;
    }
}
