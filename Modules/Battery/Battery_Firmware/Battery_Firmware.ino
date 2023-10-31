/**
 * if Stuff no work, check this: https://github.com/T-vK/ESP32-BLE-Keyboard/issues/169
 */
#include "BleKeyboard.h"
#include <Wire.h>
#include <memory>

#define BAT_V_PIN 2

#define ADDR_I2C 0x69

BleKeyboard bleKeyboard("Macruwu", "FG Labs", 69);

uint8_t delayTime = 60;

String sendString;
bool messageAvailable = 0;
//KeyReport reportBuffer;
std::vector <KeyReport *> reportBuffer;

void Keyboard_print(String text);
void Keyboard_send_buffer();
void clear_send_buffer();
void receiveEvent(int howMany);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  bleKeyboard.setBatteryLevel(get_battery_level());
  Wire.begin(ADDR_I2C);
  Wire.onReceive(receiveEvent);
}



void loop() {
  
  if(bleKeyboard.isConnected() && messageAvailable) {
    messageAvailable = 0;
    Keyboard_print(sendString);
    Keyboard_send_buffer();
  }
  else if(sendString.length() > 0){
    sendString.remove(0, sendString.length());
    sendString.trim();
    clear_send_buffer();
  }



}

void receiveEvent(int howMany){
  while(Wire.available()){
    // Read in format ReportID,modifier,keycodes[6]
  }
}

void Keyboard_send_buffer(){
  Serial.println("Sending Buffer");
  for(int i = 0; i < reportBuffer.size(); i++){
    if((i%4)==0){
      delay(delayTime);
    }
    bleKeyboard.sendReport(reportBuffer.at(i));
    bleKeyboard.releaseAll();
    for(int n = 0; n < 6; n++){
      Serial.print(reportBuffer.at(i)->keys[n]);
    }
    Serial.println(reportBuffer.at(i)->modifiers);
  }
  clear_send_buffer();
}

uint8_t get_battery_level(){
  return (uint8_t)map(analogRead(BAT_V_PIN), 0, 1023, 0, 100);
}



