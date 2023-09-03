#ifndef USB_INTERFACE
#define USB_INTERFACE

#include "filesystem.h"

class hidInterface{
public:
  hidInterface();
  void init();
  void press(key * inputKey);       //send single key
  void release(key * inputKey);     //release single key
  void sendMacro(key * inputKey);
private:
  uint8_t keycodeBuffer[6];
  uint8_t modifier;
  void send(uint8_t reportID);
  void clear(uint8_t reportID);
};


#endif //USB_INTERFACE