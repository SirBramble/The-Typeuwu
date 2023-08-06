#ifndef FILESYSTEM
#define FILESYSTEM

#include <Regexp.h>
#include <vector>
#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

void filesystemSetup();
void filesystemLoop();
void filesystemClear();
void filesystemTest();

bool check_fs_changed();

typedef struct{
  uint8_t keycode;
  uint8_t modifier;
  uint8_t reportID;
  uint8_t immedieateSend;
}keysycode;

class key{
public:
  key(void);
  void appendKeysycode(uint8_t keycode, uint8_t modifier, uint8_t reportID, uint8_t immediateSend);
  void clear();             //clears Vector and resets to default (size = 1)
  bool isAnalog = 0;        //flag to identefy if "key" stores a analog value
  keysycode getKeysycode(uint16_t position);
private:
  std::vector <keysycode> keycodes;
  uint16_t analogValue;     //use this for potentiometer values and the such
};

class keySet{
public:
  keySet(void);
  void setSize(uint16_t ammountKeys);     //use keys.resize()
  key * getKeyPointer(uint16_t position);
private:
  std::vector <key *> keys;
};

class layerSet{
public:
  void setSize(uint16_t ammountLayers, uint16_t ammountKeys);
  void clearAll();
  key * getKeyPointer(uint16_t layer, uint16_t position);
private:
  std::vector <keySet> layers;
};

#endif // FILESYSTEM
