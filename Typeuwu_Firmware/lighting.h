#ifndef LIGHTING
#define LIGHTING

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    1
#define LED_COUNT 88
#define LED_BRIGHTNESS 100


void lightingSetup();
void lightingLoop();

void colorWipe(uint32_t color, int wait);
void theaterChase(uint32_t color, int wait);
void rainbow(uint8_t wait);
void theaterChaseRainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos);

#endif //LIGHTING