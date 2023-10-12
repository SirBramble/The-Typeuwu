#include "i2cinterface.h"

bool sda_set = 0;
bool scl_set = 0;

void i2cInterface::init(){
  sda_set = Wire.setSDA(PIN_SDA);
  scl_set = Wire.setSCL(PIN_SCL);
  Wire.begin();
  for(int i = 0; i < I2C_BUFFER_SIZE; i++){
    button_states[i] = 0;
  }
}

void i2cInterface::probe(){
  uint8_t error;
  address_valid.clear();
  for(uint8_t i = 0x10; i < 0x80; i++){
    Wire.beginTransmission(i);
    error = Wire.endTransmission();
    if(error == 0){
      address_valid.push_back(i);
      Serial.print("Found Address: ");Serial.println(i);
    }
    if(i == 0x20){
      Serial.print("error: ");Serial.println(error);
      Serial.print("SDA set: ");Serial.println(sda_set);
      Serial.print("SCL set: ");Serial.println(scl_set);
    }
  }

  if(address_valid.size() == 0){
    current_address = 0;
  }
}

void i2cInterface::update(uint8_t address){
  bool addressIsValid = 0;
  for(int i = 0; i < I2C_BUFFER_SIZE; i++){
      button_states[i] = 0;
  }
  for(uint8_t i = 0; i < address_valid.size(); i++){
    if(address == address_valid.at(i)){
      addressIsValid = 1;
    }
  }
  if(!addressIsValid){
    //Serial.println("address not found valid!");
    return;
  }
  if(current_command != GET_STATES || current_address != address){
    current_address = address;
    current_command = GET_STATES;
    Wire.beginTransmission(address);
    Wire.write(GET_STATES);
    Wire.endTransmission();
    delay(1);
  }
  Wire.requestFrom(address, I2C_BUFFER_SIZE, true);
  uint8_t i = 0;
  
  while(Wire.available() && i < I2C_BUFFER_SIZE){
    button_states[i] = Wire.read();
    for(int i = 0; i < I2C_BUFFER_SIZE; i++){
      if(button_states[i] > 0){
        //Serial.print(pressed[i]);
        //Serial.print("button pressed I2C: ");Serial.println(i);
      }
    }
    i++;
  }
  while(i < I2C_BUFFER_SIZE){
    button_states[i] = 0;
    i++;
  }
}

uint8_t i2cInterface::getVal(uint8_t button, uint8_t address){
  if(button < I2C_BUFFER_SIZE){
    if(current_address != address) this->update(address);
    return button_states[button];
  }
  else return 0;
}

uint8_t i2cInterface::getVal_db(uint8_t button, uint8_t address){
  if(button < I2C_BUFFER_SIZE){
    if(current_address != address) this->update(address);
    return button_states[button];
  }
  else return 0;
}

uint8_t * i2cInterface::getStates(uint8_t address){
  if(current_address != address) this->update(address);
  return button_states;
}

bool i2cInterface::isAddressValid(uint8_t address){
  bool addressIsValid = 0;
  for(uint8_t i = 0; i < address_valid.size(); i++){
    if(address == address_valid.at(i)){
      addressIsValid = 1;
    }
  }
  return addressIsValid;
}
