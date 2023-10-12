#include "hidInterface.h"

uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(RID_KEYBOARD) ),
  TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(RID_GAMEPAD)),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(RID_CONSUMER_CONTROL) )
};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

//////////////////////////////USB_INTERFACE//////////////////////////

hidInterface::hidInterface(){
  this->modifier = 0;
}

void hidInterface::init(){
  usb_hid.begin();
  MIDI.begin(MIDI_CHANNEL_OMNI);
  while(!TinyUSBDevice.mounted()){
    delay(100);
    Serial.println("crashing...");
  }

  for(int i = 0; i < 6; i++){
    this->keycodeBuffer[i] = 0;
  }
  //for(int i = 0; i < 16; i++){
  //  for(int n = 0; n < 256; n++){
  //    midi_value_storage[i][n] = 0;
  //  }
  //}
}

void hidInterface::press(key * inputKey){
  if(inputKey == nullptr) return;
  Serial.print("isSingle: "); Serial.println(inputKey->isSingleKey);
  if(inputKey->isSingleKey == 0) return;
  if(inputKey->isAnalog == 1) return;

  keysycode inputKeycode = inputKey->getKeysycode(0);

  if(inputKey->isModifier && ((this->modifier & inputKeycode.modifier) == 0)){
    this->modifier += inputKeycode.modifier;
  }
  Serial.print("KeycodeBuffer: ");
  for(uint8_t i = 0; i < 6; i++){
    Serial.print(keycodeBuffer[i]);
    if(this->keycodeBuffer[i] == 0){
      keycodeBuffer[i] = inputKeycode.keycode;
      break;
    }
  }
  Serial.println();
  //this->sendEmpty(inputKeycode.reportID);
  this->send(inputKeycode.reportID);

  return;
}

void hidInterface::release(key * inputKey){
  if(inputKey == nullptr) return;
  Serial.print("input keysycode: "); Serial.println(inputKey->getKeysycode(0).keycode);
  Serial.print("input keysycode 1: "); Serial.println(inputKey->getKeysycode(1).keycode);
  if(inputKey->isSingleKey == 0) return;
  if(inputKey->isAnalog == 1) return;

  keysycode inputKeycode = inputKey->getKeysycode(0);
  if(inputKey->isModifier && ((this->modifier & inputKeycode.modifier) > 0)){
    this->modifier -= inputKeycode.modifier;
  }

  for(uint8_t i = 0; i < 6; i++){
    if(this->keycodeBuffer[i] == inputKeycode.keycode){
      keycodeBuffer[i] = 0;
    }
  }

  this->send(inputKeycode.reportID);
  
  return;
}

void hidInterface::sendMacro(key * inputKey){
  if(inputKey == nullptr) return;
  
  int keycodeBufferIndex = 0;

  this->clear(inputKey->getKeysycode(0).reportID);

  for(int i = 0; i < inputKey->getKeycodesSize(); i++){
    this->keycodeBuffer[keycodeBufferIndex] = inputKey->getKeysycode(i).keycode;
    this->modifier = inputKey->getKeysycode(i).modifier;
    if(inputKey->getKeysycode(i).immediateSend || keycodeBufferIndex > 6){
      this->send(inputKey->getKeysycode(i).reportID);
      this->clear(inputKey->getKeysycode(i).reportID);
      keycodeBufferIndex = 0;
    }
    else keycodeBufferIndex++;
  }

  return;
}

void hidInterface::send(uint8_t reportID){
  while(!usb_hid.ready());
  usb_hid.keyboardReport(reportID, this->modifier, this->keycodeBuffer);

  return;
}

void hidInterface::sendEmpty(uint8_t reportID){
  while(!usb_hid.ready());
  uint8_t emptyBuffer[6] = {0,0,0,0,0,0};
  usb_hid.keyboardReport(reportID, this->modifier, emptyBuffer);

  return;
}

void hidInterface::clear(uint8_t reportID){
  for(uint8_t i = 0; i < 6; i++){
    keycodeBuffer[i] = 0;
  }
  this->modifier = 0;
  while(!usb_hid.ready());
  usb_hid.keyboardReport(reportID, this->modifier, this->keycodeBuffer);

  return;
}

void hidInterface::sendMidi_Analog(key * inputKey, uint8_t value){
  if(!inputKey->isMIDI) return;
  
  if(value != midi_value_storage[inputKey->MIDI_channel][inputKey->MIDI_data1]){
    midi_value_storage[inputKey->MIDI_channel][inputKey->MIDI_data1] = value;
    switch (inputKey->MIDI_mode) {
      case MIDI_CC:
        MIDI.sendControlChange(inputKey->MIDI_data1, value, inputKey->MIDI_channel);
        Serial.print("Sending MIDI CC value: ");Serial.println(value);
        break;
      default:
        break;
    }
  }
}