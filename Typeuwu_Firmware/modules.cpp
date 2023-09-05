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
  }
}

void _keyboard::update(){
  for(int row = 0; row < AMMOUNT_ROW; row++){
    pinMode(this->pins_row[row], OUTPUT);
    digitalWrite(this->pins_row[row], HIGH);

    for(int col = AMMOUNT_COL; col >= 0; col--){
      if(digitalRead(this->pins_col[col])){
        Serial.print("matrix: "); Serial.print(diodeMatrixToKeyIndex[row][col]);
        if(diodeMatrixToKeyIndex[row][col] != 0){
          pressed[diodeMatrixToKeyIndex[row][col] - 1] = 1;
        }
        Serial.print(" row: ");Serial.print(row + 1);Serial.print(" col: "); Serial.println(col + 1);
      }
      else{
        pressed[diodeMatrixToKeyIndex[row][col] - 1] = 0;
      }
    }
    delay(1);
    digitalWrite(this->pins_row[row], LOW);
    pinMode(this->pins_row[row], INPUT);
  }
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