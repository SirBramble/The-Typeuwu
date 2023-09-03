#include "hidInterface.h"

uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(RID_KEYBOARD) ),
  TUD_HID_REPORT_DESC_GAMEPAD(HID_REPORT_ID(RID_GAMEPAD)),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(RID_CONSUMER_CONTROL) )
};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_NONE, 2, false);

//////////////////////////////USB_INTERFACE//////////////////////////

hidInterface::hidInterface(){
  this->modifier = 0;
}

void hidInterface::init(){
  usb_hid.begin();
  while(!TinyUSBDevice.mounted()){
    delay(100);
    Serial.println("crashing...");
  }

  for(int i = 0; i < 6; i++){
    this->keycodeBuffer[i] = 0;
  }
}

void hidInterface::press(key * inputKey){
  keysycode inputKeycode = inputKey->getKeysycode(0);

  if(inputKeycode.modifier != 0) this->modifier = inputKeycode.modifier;

  for(uint8_t i = 0; i < 6; i++){
    if(this->keycodeBuffer[i] == 0){
      keycodeBuffer[i] = inputKeycode.keycode;
    }
  }

  this->send(inputKeycode.reportID);

  return;
}

void hidInterface::release(key * inputKey){
  keysycode inputKeycode = inputKey->getKeysycode(0);

  if(inputKeycode.modifier != 0) this->modifier = 0;

  for(uint8_t i = 0; i < 6; i++){
    if(this->keycodeBuffer[i] == inputKeycode.keycode){
      keycodeBuffer[i] = 0;
    }
  }

  this->send(inputKeycode.reportID);
  
  return;
}

void hidInterface::sendMacro(key * inputKey){
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

void hidInterface::clear(uint8_t reportID){
  for(uint8_t i = 0; i < 6; i++){
    keycodeBuffer[i] = 0;
  }
  this->modifier = 0;
  while(!usb_hid.ready());
  usb_hid.keyboardReport(reportID, this->modifier, this->keycodeBuffer);

  return;
}