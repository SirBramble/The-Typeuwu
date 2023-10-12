#ifndef I2CINTERFACE
#define I2CINTERFACE

#include "Wire.h"
#include <vector>

#define PIN_SDA 4
#define PIN_SCL 5

#define I2C_BUFFER_SIZE 32    // According to Arduino documentation the buffer for a I2C trans is 32 bytes.
                              // With the RP2040 the buffer is 128 bytes. May need to limit to 32 bytes to keep compatibility with other MCUs.

#define ANALOG_DB 2

// I2C Commands
#define IDLE_I2C 0x10
#define GET_STATES 0x11



class i2cInterface{
public:
  void init();
  void update(uint8_t address); //Recieves values between 0-255 for each button
  void probe();
  uint8_t getVal(uint8_t button, uint8_t address);
  uint8_t getVal_db(uint8_t button, uint8_t address);
  uint8_t * getStates(uint8_t address);
  bool isAddressValid(uint8_t address);
private:
  uint8_t button_states[I2C_BUFFER_SIZE];
  std::vector<uint8_t> address_valid;
  uint8_t current_address;
  uint8_t current_command = 0;
};

#endif // I2CINTERFACE