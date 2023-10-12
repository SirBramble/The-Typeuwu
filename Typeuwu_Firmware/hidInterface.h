#ifndef USB_INTERFACE
#define USB_INTERFACE

#include "filesystem.h"
#include "MIDI.h"

class hidInterface{
public:
  hidInterface();
  void init();
  void press(key * inputKey);       //send single key
  void release(key * inputKey);     //release single key
  void sendMacro(key * inputKey);
  void sendMidi_Analog(key * inputKey, uint8_t value);
private:
  uint8_t keycodeBuffer[6];
  uint8_t modifier;
  void send(uint8_t reportID);
  void sendEmpty(uint8_t reportID);
  void clear(uint8_t reportID);
  uint8_t midi_value_storage[16][256];
};


#endif //USB_INTERFACE