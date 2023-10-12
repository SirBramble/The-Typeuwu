#include <stdint.h>
#ifndef MODULES
#define MODULES

#include "filesystem.h"

#define AMMOUNT_KEYS 88
#define AMMOUNT_KEYS_NUMPAD 27      // actually 23, but for testing adding buffer


#define KEY_REMAPPING \
{73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 0 },\
{ 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 63, 64, 65},\
{15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,  0, 66, 67, 68},\
{29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 28,  0,  0,  0},\
{42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,  0,  0, 69,  0},\
{55, 56, 57,  0,  0,  0, 58,  0,  0, 59, 60, 61, 62,  0, 70, 71, 72}\

#define AMMOUNT_ROW 6
#define ROW1 6
#define ROW2 7
#define ROW3 8
#define ROW4 9
#define ROW5 14
#define ROW6 13

#define AMMOUNT_COL 17
#define COL1 10
#define COL2 11
#define COL3 16
#define COL4 19
#define COL5 20
#define COL6 21
#define COL7 22
#define COL8 23
#define COL9 24
#define COL10 25
#define COL11 26
#define COL12 27
#define COL13 28
#define COL14 29
#define COL15 15
#define COL16 17
#define COL17 12

typedef enum{
  IDLE = 0,
  TRANSITION,
  PRESSED,
  RELEASED
} _state;

class _keyboard : public module{
public:
  _keyboard(String moduleName);
  void init();
  void testKeys();
  bool isPressed(int position);
  void update();
  bool isPressed_hold(int position);
  bool isReleased_hold(int position);      //true if released
  bool isPressed_single(int position);
  bool isReleased_single(int position);    //true if released
private:
  int pins_row[AMMOUNT_ROW] = {ROW1, ROW2, ROW3, ROW4, ROW5, ROW6};
  int pins_col[AMMOUNT_COL] = {COL1, COL2, COL3, COL4, COL5, COL6, COL7, COL8, COL9, COL10, COL11, COL12, COL13, COL14, COL15, COL16, COL17};
  bool pressed[AMMOUNT_KEYS];     //remember -1 for start count at 0
  //SM
  _state states[AMMOUNT_KEYS];
  void updateSM();
};

class _numpad : public module{
public:
  _numpad(String moduleName, uint8_t address);
  void init();
  uint8_t address();
  void update(uint8_t * input);
  bool isPressed_hold(int position);
  bool isReleased_hold(int position);      //true if released
  bool isPressed_single(int position);
  bool isReleased_single(int position);    //true if released
private:
  uint8_t _address;
  uint8_t pressed[AMMOUNT_KEYS_NUMPAD];     //remember -1 for start count at 0
  //SM
  _state states[AMMOUNT_KEYS_NUMPAD];
};

#endif // MODULES