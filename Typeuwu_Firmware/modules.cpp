#include "modules.h"

int diodeMatrixToKeyIndex[AMMOUNT_ROW][AMMOUNT_COL] = {KEY_REMAPPING};

_keyboard::_keyboard(String moduleName) : module(moduleName){}

void _keyboard::init(){
  for(int i = 0; i < 6; i++){
    pinMode(this->pins_row[i], INPUT);
  }

  for(int i = 0; i < 17; i++){
    pinMode(this->pins_col[i], INPUT_PULLDOWN);
  }

  for(int i = 0; i < AMMOUNT_KEYS; i++){
    pressed[i] = 0;
    states[i] = IDLE;
  }
}

void _keyboard::update(){
  for(int row = 0; row < AMMOUNT_ROW; row++){
    pinMode(this->pins_row[row], OUTPUT);
    digitalWrite(this->pins_row[row], HIGH);

    for(int col = AMMOUNT_COL - 1; col >= 0; col--){
      if(digitalRead(this->pins_col[col])){
        Serial.print("matrix: "); Serial.print(diodeMatrixToKeyIndex[row][col]);
        if(diodeMatrixToKeyIndex[row][col] != 0){
          pressed[diodeMatrixToKeyIndex[row][col] - 1] = true;
        }
        Serial.print(" row: ");Serial.print(row + 1);Serial.print(" col: "); Serial.print(col + 1);Serial.print(" pressed: ");Serial.println(pressed[diodeMatrixToKeyIndex[row][col] - 1]);
      }
      else{
        pressed[diodeMatrixToKeyIndex[row][col] - 1] = false;
      }
    }
    digitalWrite(this->pins_row[row], LOW);
    pinMode(this->pins_row[row], INPUT);
    delay(1);
  }
  this->updateSM();
}

bool _keyboard::isPressed(int position){
  return pressed[position];
}

void _keyboard::testKeys(){
  this->update();
  for(int i = 0; i < AMMOUNT_KEYS; i++){
    if(pressed[i] > 0){
      Serial.print("Key Pressed: ");
      Serial.println(i + 1);
    }
  }
}

void _keyboard::updateSM(){
  for(int i = 0; i < AMMOUNT_KEYS; i++){
    switch(states[i]){
      case IDLE:
        if(pressed[i]){
          states[i] = TRANSITION;
        }
        break;
      case TRANSITION:
        if(pressed[i]){
          states[i] = PRESSED;
        }
        else {
          states[i] = IDLE;
        }
        break;
      case PRESSED:
        if(pressed[i] == 0){
          states[i] = RELEASED;
        }
        break;
      case RELEASED:
        if(pressed[i] == 0){
          states[i] = IDLE;
        }
    }
  }
}

bool _keyboard::isPressed_hold(int position){
  if(states[position] == TRANSITION){
    return true;
  }
  else{
    return false;
  }
}

bool _keyboard::isReleased_hold(int position){
  if(states[position] == RELEASED){
    return true;
  }
  else{
    return false;
  }
}

bool _keyboard::isPressed_single(int position){
  if(states[position] == TRANSITION){
    return true;
  }
  else{
    return false;
  }
}

bool _keyboard::isReleased_single(int position){
  if(states[position] == PRESSED){
    return true;
  }
  else{
    return false;
  }
}
//----------------------------numpad--------------------------
_numpad::_numpad(String moduleName, uint8_t address) : module(moduleName){
  this->_address = address;
}

uint8_t _numpad::address(){
  return _address;
}

void _numpad::update(uint8_t * input){
  for(int i = 0; i < AMMOUNT_KEYS_NUMPAD; i++){
    pressed[i] = input[i];
  }

  for(int i = 0; i < AMMOUNT_KEYS_NUMPAD; i++){
    switch(states[i]){
      case IDLE:
        if(pressed[i]){
          states[i] = TRANSITION;
          //Serial.print("Transition numpad: ");Serial.print(pressed[i]);Serial.print(",");Serial.println(input[i]);
        }
        break;
      case TRANSITION:
        if(pressed[i]){
          states[i] = PRESSED;
        }
        else {
          states[i] = IDLE;
        }
        break;
      case PRESSED:
        if(pressed[i] == 0){
          states[i] = RELEASED;
        }
        break;
      case RELEASED:
        states[i] = IDLE;
        break;
    }
  }
}

bool _numpad::isPressed_hold(int position){
  if(states[position] == TRANSITION){
    return true;
  }
  else{
    return false;
  }
}

bool _numpad::isReleased_hold(int position){
  if(states[position] == RELEASED){
    return true;
  }
  else{
    return false;
  }
}

bool _numpad::isPressed_single(int position){
  if(states[position] == TRANSITION){
    return true;
  }
  else{
    return false;
  }
}

bool _numpad::isReleased_single(int position){
  if(states[position] == PRESSED){
    return true;
  }
  else{
    return false;
  }
}
