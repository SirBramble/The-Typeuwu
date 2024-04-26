#include "Wire.h"
#include <Adafruit_NeoPixel.h>

#define MODULE_ADDRESS 0x20
#define I2C_BUFFER_SIZE 32

#define KEY_REMAPPING \
{ 1,  2,  3,  4},\
{ 5,  6,  7,  8},\
{ 9, 10, 11,  0},\
{12, 13, 14, 15},\
{16,  0, 17,  0},\

#define AMMOUNT_ROW 5
#define ROW1 10
#define ROW2 9
#define ROW3 8
#define ROW4 7
#define ROW5 6

#define AMMOUNT_COL 4
#define COL1 17
#define COL2 16
#define COL3 15
#define COL4 14

//--------ANALOG------------

#define PIN_SLIDER_BUTTON_1 0
#define PIN_SLIDER_BUTTON_2 1

#define PIN_SLIDER_1 26
#define PIN_SLIDER_2 25

#define ANALOG_DB 2
#define ADDITIONAL_ANALOG_ZERO_OFFSET 4

uint8_t slider1 = 0;
uint8_t slider2 = 0;

#define AMMOUNT_KEYS 23

int pins_row[AMMOUNT_ROW] = {ROW1, ROW2, ROW3, ROW4, ROW5};
int pins_col[AMMOUNT_COL] = {COL1, COL2, COL3, COL4};
uint8_t pressed[AMMOUNT_KEYS];

int diodeMatrixToKeyIndex[AMMOUNT_ROW][AMMOUNT_COL] = {KEY_REMAPPING};


#define IDLE 0x10
#define SEND_STATES 0x11

uint8_t mode = IDLE;

void receive_handler(int howMany);
void request_handler(void);

//------------------------Lighting-----------------------------
#define LED_PIN    5
#define LED_COUNT 23
#define LED_BRIGHTNESS 100

unsigned long pixelPrevious = 0;        // Previous Pixel Millis
unsigned long patternPrevious = 0;      // Previous Pattern Millis
int           patternCurrent = 0;       // Current Pattern Number
int           patternInterval = 5000;   // Pattern Interval (ms)
int           pixelInterval = 50;       // Pixel Interval (ms)
int           pixelQueue = 0;           // Pattern Pixel Queue
int           pixelCycle = 0;           // Pattern Pixel Cycle
uint16_t      pixelCurrent = 0;         // Pattern Current Pixel Number
uint16_t      pixelNumber = LED_COUNT;  // Total Number of Pixels

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void lightingSetup();
uint32_t Wheel(byte WheelPos);
void rainbow(uint8_t wait);

void setup() {
  delay(1000);
  pinMode(PIN_SLIDER_BUTTON_1, INPUT_PULLUP);
  pinMode(PIN_SLIDER_BUTTON_2, INPUT_PULLUP);

  Wire.begin(MODULE_ADDRESS);
  Wire.onRequest(request_handler);
  Wire.onReceive(receive_handler);
  //pinMode(A4, INPUT_PULLUP);
  //pinMode(A5, INPUT_PULLUP);
  lightingSetup();
  for(int i = 0; i < AMMOUNT_KEYS; i++){
    pressed[i] = 0;
  }
}

//int color = 255;

void loop() {
  update();
  rainbow(10);
  //if(!digitalRead(18)) strip.setPixelColor(1, strip.Color(255, 0, 0));
  //if(digitalRead(18)) strip.setPixelColor(1, strip.Color(0, 0, 0));
  //if(!digitalRead(19)) strip.setPixelColor(2, strip.Color(255, 0, 0));
  //if(digitalRead(19)) strip.setPixelColor(2, strip.Color(0, 0, 0));
  //strip.show();
  //strip.setPixelColor(10, strip.Color(0, map(analogRead(PIN_SLIDER_2), 0, 1023, 0, 255), 0));
  //strip.setPixelColor(1, strip.Color(0, color, 0));
  //color--;
  //if(color == 0) color = 255;
}

void update(){
  //Matrix
  for(int row = 0; row < AMMOUNT_ROW; row++){
    pinMode(pins_row[row], OUTPUT);
    delay(1);
    digitalWrite(pins_row[row], HIGH);
    delay(1);

    for(int col = AMMOUNT_COL - 1; col >= 0; col--){
      if(digitalRead(pins_col[col])){
        if(diodeMatrixToKeyIndex[row][col] != 0){
          pressed[diodeMatrixToKeyIndex[row][col] - 1] = 1;
        }
      }
      else{
        pressed[diodeMatrixToKeyIndex[row][col] - 1] = 0;
      }
    }
    delay(1);
    digitalWrite(pins_row[row], LOW);
    delay(1);
    pinMode(pins_row[row], INPUT);
    delay(1);
  }
  //Buttons slider
  pressed[17] = !digitalRead(PIN_SLIDER_BUTTON_1);
  pressed[18] = !digitalRead(PIN_SLIDER_BUTTON_2);

  //Slider
  if((map(analogRead(PIN_SLIDER_1), 0, 1023, 255, 0) >= (slider1 + ANALOG_DB)) || (map(analogRead(PIN_SLIDER_1), 0, 1023, 255, 0) <= (slider1 - ANALOG_DB))){
    slider1 = map(analogRead(PIN_SLIDER_1), 0, 1023, 255, 0);
  }
  if((map(analogRead(PIN_SLIDER_2), 0, 1023, 255, 0) >= (slider2 + ANALOG_DB)) || (map(analogRead(PIN_SLIDER_2), 0, 1023, 255, 0) <= (slider2 - ANALOG_DB))){
    slider2 = map(analogRead(PIN_SLIDER_2), 0, 1023, 255, 0);
  }
  
  if(map(analogRead(PIN_SLIDER_1), 0, 1023, 255, 0) < (ADDITIONAL_ANALOG_ZERO_OFFSET + ANALOG_DB)){
    slider1 = 0;
  }
  if(map(analogRead(PIN_SLIDER_2), 0, 1023, 255, 0) < (ADDITIONAL_ANALOG_ZERO_OFFSET + ANALOG_DB)){
    slider2 = 0;
  }

  if(map(analogRead(PIN_SLIDER_1), 0, 1023, 255, 0) > (255 - ADDITIONAL_ANALOG_ZERO_OFFSET + ANALOG_DB)){
    slider1 = 255;
  }
  if(map(analogRead(PIN_SLIDER_2), 0, 1023, 255, 0) > (255 - ADDITIONAL_ANALOG_ZERO_OFFSET + ANALOG_DB)){
    slider2 = 255;
  }
  pressed[19] = slider1;
  pressed[20] = slider2;
}

void receive_handler(int howMany){
  if(Wire.available()){
    mode = Wire.read();
  }
  if(mode == SEND_STATES) strip.setPixelColor(6, strip.Color(0, 255, 0));
  while(Wire.available()) Wire.read();
}

void request_handler(void){
  uint8_t i = 0;
  switch (mode) {
    case IDLE:
      //strip.setPixelColor(1, strip.Color(0, 255, 0));
      //strip.setPixelColor(2, strip.Color(0, 0, 0));
      //strip.setPixelColor(3, strip.Color(0, 0, 0));
      while(i < I2C_BUFFER_SIZE){
        Wire.write(0);
        i++;
      }
      break;
    case SEND_STATES:
      //strip.setPixelColor(1, strip.Color(0, 0, 0));
      //strip.setPixelColor(2, strip.Color(255, 0, 0));
      //strip.setPixelColor(3, strip.Color(0, 0, 0));
      while(i < AMMOUNT_KEYS){
        Wire.write(pressed[i]);
        //Wire.write(0);
        i++;
      }
      while(i < I2C_BUFFER_SIZE){
        Wire.write(0);
        i++;
      }
      //mode = IDLE;
      break;
    default:
      //strip.setPixelColor(1, strip.Color(0, 0, 0));
      //strip.setPixelColor(2, strip.Color(0, 0, 0));
      //strip.setPixelColor(3, strip.Color(0, 0, 255));
      while(i < I2C_BUFFER_SIZE){
        Wire.write(0);
        i++;
      }
      //mode = IDLE;
      break;
  }
}

void rainbow(uint8_t wait) {
  if(pixelInterval != wait)
    pixelInterval = wait;                   
  for(uint16_t i=0; i < pixelNumber; i++) {
    strip.setPixelColor(i, Wheel((i + pixelCycle) & 255)); //  Update delay time  
  }
  strip.show();                             //  Update strip to match
  pixelCycle++;                             //  Advance current cycle
  if(pixelCycle >= 256)
    pixelCycle = 0;                         //  Loop the cycle back to the begining
}

void lightingSetup(){
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(LED_BRIGHTNESS); // Set BRIGHTNESS to about 1/5 (max = 255)
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}