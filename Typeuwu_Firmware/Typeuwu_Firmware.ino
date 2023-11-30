#include "modules.h"
#include "hidInterface.h"
#include "lighting.h"
#include "i2cinterface.h"

//#define SERIAL_DEBUG
#define ADDRESS_MODULE1 0x20
#define ADDRESS_Bluetooth 0x69

// Uncomment to enable
#define IS_MACRUWU

uint8_t registered_I2C_addresses[] = {ADDRESS_MODULE1,ADDRESS_Bluetooth};

_keyboard keyboard("keyboard");
_numpad numpad1("numpad1", ADDRESS_MODULE1);

i2cInterface i2c(registered_I2C_addresses, sizeof(registered_I2C_addresses));
hidInterface hid(1000, ADDRESS_Bluetooth);
RGB_LIGHTING light(1, 88, 50);    // RGB_LIGHTING(uint8_t led_pin, uint16_t led_count, uint8_t led_max_brightness)


uint64_t prev;
uint8_t error;

bool miditrigger = 0;
int midibrokencount = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("uwu Setup 1");

  filesystemSetup();
  Serial.println("uwu Setup 2");

  filesystemCreateConfig();
  Serial.println("uwu Setup 3");

  hid.init();
  light.setup();
  light.set_led_remap(keyboard.get_led_remap(), keyboard.get_ammount_keys());
  Serial.println("uwu Setup 4");

  keyboard.init();
  Serial.println("uwu Setup 5");

  keyboard.updateKeymapsFromFile();
  numpad1.updateKeymapsFromFile();

  i2c.init();

  if(hid.bluetooth_mode == 0){
    error = i2c.disableESP(ADDRESS_Bluetooth);
  }

  prev = 0;
}





void loop() {
  //filesystemLoop();

  light.update();
  light.set_effect(keyboard.getLayerLightingEffect());

  if(check_fs_changed()){
    keyboard.updateKeymapsFromFile();
    numpad1.updateKeymapsFromFile();
    for(int i = 0; i < numpad1.ammountKeys; i++){
      key *currentKey = numpad1.getKeyPointer(i);
      if(currentKey->color_mode != no_override){
        i2c.set_led_override(numpad1.address(), numpad1.remap_led_key(i), currentKey->color);
      }
    }
    
    set_fs_changed(false);
  }

  if(millis() > prev + 1000){
    prev = millis();
    i2c.probe();
    #ifdef SERIAL_DEBUG
      Serial.println("uwu");
      if(error == 0) Serial.println("ESP disabled successfully");
      else Serial.print("ESP has given error on disable: ");Serial.println(error);
    #endif
  }

  keyboard.update();

  for(int i = 0; i < AMMOUNT_KEYS; i++){
    key *currentKey = keyboard.getKeyPointer(i + 1);

    if(currentKey->color_mode == midi_bound){
      if(hid.get_midi_CC_state(currentKey->MIDI_channel, currentKey->MIDI_data1) && hid.get_midi_CC_update_available(currentKey->MIDI_channel, currentKey->MIDI_data1)){
        hid.set_midi_CC_update_handled(currentKey->MIDI_channel, currentKey->MIDI_data1);
        light.enable_override(i, currentKey->color);
      }
      else if(hid.get_midi_CC_update_available(currentKey->MIDI_channel, currentKey->MIDI_data1)){
        hid.set_midi_CC_update_handled(currentKey->MIDI_channel, currentKey->MIDI_data1);
        light.disable_override(i);
      }
    }

    if(keyboard.isPressed(i)){
      if(currentKey->hasLayerChange){
        keyboard.setLayer(currentKey->changeToLayer);
      }
      if(currentKey->isMIDI){
        hid.sendMidi_Analog(keyboard.getKeyPointer(i + 1), 255);
      }
      else{
        Serial.println("pressing");
        hid.press(currentKey);
      }
      switch(currentKey->color_mode){
        case no_override:
          break;
        case midi_bound:
          break;
        case pressed:
          light.enable_override(i, currentKey->color);
          break;
        case not_pressed:
          light.disable_override(i);
          break;
        case toggle:
          light.toggle_override(i, currentKey->color);
          break;
        case disabled:
          light.disable_override(i);
      }
    }
    else if(keyboard.isReleased(i)){
      if(currentKey->isMIDI){
        hid.sendMidi_Analog(keyboard.getKeyPointer(i + 1), 0);
      }
      else{
        Serial.println("releasing");
        hid.release(keyboard.getKeyPointer(i + 1));
      }

      switch(currentKey->color_mode){
        case no_override:
          break;
        case midi_bound:
          break;
        case pressed:
          light.disable_override(i);
          break;
        case not_pressed:
          light.enable_override(i, currentKey->color);
          break;
        case toggle:
          break;
        case disabled:
          light.disable_override(i);
      }
    }
  }
  
  i2c.update(numpad1.address());
  numpad1.update(i2c.getStates(numpad1.address()));
  if(i2c.isAddressValid(numpad1.address())){
    for(int i = 0; i < I2C_BUFFER_SIZE; i++){
      key *currentKey = numpad1.getKeyPointer(i + 1);
      if(currentKey == nullptr) continue;

      if(currentKey->color_mode == midi_bound){
        if(hid.get_midi_CC_state(currentKey->MIDI_channel, currentKey->MIDI_data1) && hid.get_midi_CC_update_available(currentKey->MIDI_channel, currentKey->MIDI_data1)){
          hid.set_midi_CC_update_handled(currentKey->MIDI_channel, currentKey->MIDI_data1);
          //light.enable_override(i, currentKey->color);
          i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), true);
        }
        else if(hid.get_midi_CC_update_available(currentKey->MIDI_channel, currentKey->MIDI_data1)){
          hid.set_midi_CC_update_handled(currentKey->MIDI_channel, currentKey->MIDI_data1);
          //light.disable_override(i);
          i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), false);
        }
      }
      
      if(currentKey->isMIDI && currentKey->isAnalog){         //don't forget to compare val with previous val when setting. Yes mum...
        hid.sendMidi_Analog(numpad1.getKeyPointer(i + 1), map(i2c.getVal(i + 1, numpad1.address()), 0, 255, 0, 127));
      }

      else if(numpad1.isPressed_hold(i)){
        if(currentKey->hasLayerChange){
          numpad1.setLayer(currentKey->changeToLayer);
        }
        if(currentKey->isMIDI){
          hid.sendMidi_Analog(numpad1.getKeyPointer(i + 1), 255);
        }
        else{
          Serial.println("pressing");
          hid.press(numpad1.getKeyPointer(i + 1));
        }
        switch(currentKey->color_mode){
          case no_override:
            break;
          case midi_bound:
            break;
          case pressed:
            //light.enable_override(i, currentKey->color);
            i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), true);
            break;
          case not_pressed:
            //light.disable_override(i);
            i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), false);
            break;
          case toggle:
            //light.toggle_override(i, currentKey->color);
            i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), false);
            break;
          case disabled:
            //light.disable_override(i);
            i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), false);
        }
      }
      else if(numpad1.isReleased_hold(i)){
        if(currentKey->isMIDI){
          hid.sendMidi_Analog(numpad1.getKeyPointer(i + 1), 0);
        }
        else{
          Serial.println("releasing");
          hid.release(numpad1.getKeyPointer(i + 1));
        }
        switch(currentKey->color_mode){
          case no_override:
            break;
          case midi_bound:
            break;
          case pressed:
            //light.disable_override(i);
            i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), false);
            break;
          case not_pressed:
            //light.enable_override(i, currentKey->color);
            i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), true);
            break;
          case toggle:
            break;
          case disabled:
            //light.disable_override(i);
            i2c.send_led_override_update(numpad1.address(), numpad1.remap_led_key(i), false);
        }
      }
      
    }
  }

  hid.clear_midi_CC_update_available();

  //rainbow(10);
  while(hid.readMidi()) hid.readMidi();
  delay(1);
}
