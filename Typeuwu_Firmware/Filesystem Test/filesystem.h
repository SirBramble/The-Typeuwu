#include <cstdint>
#include <vector>
#include <string>

#define CONFIG_FILENAME "Layout.txt"

#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LMETA  0x08
#define KEY_MOD_RCTRL  0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT   0x40
#define KEY_MOD_RMETA  0x80

#define HID_ASCII_TO_KEYCODE_GERMAN \
    {0              , 0                     }, /* 0x00 Null      */ \
    {0              , 0                     }, /* 0x01           */ \
    {0              , 0                     }, /* 0x02           */ \
    {0              , 0                     }, /* 0x03           */ \
    {0              , 0                     }, /* 0x04           */ \
    {0              , 0                     }, /* 0x05           */ \
    {0              , 0                     }, /* 0x06           */ \
    {0              , 0                     }, /* 0x07           */ \
    {0              , HID_KEY_BACKSPACE     }, /* 0x08 Backspace */ \
    {0              , HID_KEY_TAB           }, /* 0x09 Tab       */ \
    {0              , HID_KEY_ENTER         }, /* 0x0A Line Feed */ \
    {0              , 0                     }, /* 0x0B           */ \
    {0              , 0                     }, /* 0x0C           */ \
    {0              , HID_KEY_ENTER         }, /* 0x0D CR        */ \
    {0              , 0                     }, /* 0x0E           */ \
    {0              , 0                     }, /* 0x0F           */ \
    {0              , 0                     }, /* 0x10           */ \
    {0              , 0                     }, /* 0x11           */ \
    {0              , 0                     }, /* 0x12           */ \
    {0              , 0                     }, /* 0x13           */ \
    {0              , 0                     }, /* 0x14           */ \
    {0              , 0                     }, /* 0x15           */ \
    {0              , 0                     }, /* 0x16           */ \
    {0              , 0                     }, /* 0x17           */ \
    {0              , 0                     }, /* 0x18           */ \
    {0              , 0                     }, /* 0x19           */ \
    {0              , 0                     }, /* 0x1A           */ \
    {0              , HID_KEY_ESCAPE        }, /* 0x1B Escape    */ \
    {0              , 0                     }, /* 0x1C           */ \
    {0              , 0                     }, /* 0x1D           */ \
    {0              , 0                     }, /* 0x1E           */ \
    {0              , 0                     }, /* 0x1F           */ \
                                                                    \
    {0              , HID_KEY_SPACE         }, /* 0x20           */ \
    {KEY_MOD_LSHIFT , HID_KEY_1             }, /* 0x21 !         */ \
    {KEY_MOD_LSHIFT , HID_KEY_2             }, /* 0x22 "         */ \
    {0              , 0x32                  }, /* 0x23 #         */ \
    {KEY_MOD_LSHIFT , HID_KEY_4             }, /* 0x24 $         */ \
    {KEY_MOD_LSHIFT , HID_KEY_5             }, /* 0x25 %         */ \
    {KEY_MOD_LSHIFT , HID_KEY_6             }, /* 0x26 &         */ \
    {KEY_MOD_LSHIFT , 0x32                  }, /* 0x27 '         */ \
    {KEY_MOD_LSHIFT , HID_KEY_8             }, /* 0x28 (         */ \
    {KEY_MOD_LSHIFT , HID_KEY_9             }, /* 0x29 )         */ \
    {KEY_MOD_LSHIFT , 0x30                  }, /* 0x2A *         */ \
    {0              , 0x30                  }, /* 0x2B +         */ \
    {0              , HID_KEY_COMMA         }, /* 0x2C ,         */ \
    {0              , 0x38                  }, /* 0x2D -         */ \
    {0              , HID_KEY_PERIOD        }, /* 0x2E .         */ \
    {KEY_MOD_LSHIFT , HID_KEY_7             }, /* 0x2F /         */ \
    {0              , HID_KEY_0             }, /* 0x30 0         */ \
    {0              , HID_KEY_1             }, /* 0x31 1         */ \
    {0              , HID_KEY_2             }, /* 0x32 2         */ \
    {0              , HID_KEY_3             }, /* 0x33 3         */ \
    {0              , HID_KEY_4             }, /* 0x34 4         */ \
    {0              , HID_KEY_5             }, /* 0x35 5         */ \
    {0              , HID_KEY_6             }, /* 0x36 6         */ \
    {0              , HID_KEY_7             }, /* 0x37 7         */ \
    {0              , HID_KEY_8             }, /* 0x38 8         */ \
    {0              , HID_KEY_9             }, /* 0x39 9         */ \
    {KEY_MOD_LSHIFT , 0x36                  }, /* 0x3A :         */ \
    {0              , HID_KEY_SEMICOLON     }, /* 0x3B ;         */ \
    {0              , HID_KEY_PERIOD        }, /* 0x3C <         */ \
    {KEY_MOD_LSHIFT , HID_KEY_0             }, /* 0x3D =         */ \
    {KEY_MOD_LSHIFT , HID_KEY_PERIOD        }, /* 0x3E >         */ \
    {KEY_MOD_LSHIFT , HID_KEY_SLASH         }, /* 0x3F ?         */ \
                                                                    \
    {KEY_MOD_LSHIFT , HID_KEY_2             }, /* 0x40 @         */ \
    {KEY_MOD_LSHIFT , HID_KEY_A             }, /* 0x41 A         */ \
    {KEY_MOD_LSHIFT , HID_KEY_B             }, /* 0x42 B         */ \
    {KEY_MOD_LSHIFT , HID_KEY_C             }, /* 0x43 C         */ \
    {KEY_MOD_LSHIFT , HID_KEY_D             }, /* 0x44 D         */ \
    {KEY_MOD_LSHIFT , HID_KEY_E             }, /* 0x45 E         */ \
    {KEY_MOD_LSHIFT , HID_KEY_F             }, /* 0x46 F         */ \
    {KEY_MOD_LSHIFT , HID_KEY_G             }, /* 0x47 G         */ \
    {KEY_MOD_LSHIFT , HID_KEY_H             }, /* 0x48 H         */ \
    {KEY_MOD_LSHIFT , HID_KEY_I             }, /* 0x49 I         */ \
    {KEY_MOD_LSHIFT , HID_KEY_J             }, /* 0x4A J         */ \
    {KEY_MOD_LSHIFT , HID_KEY_K             }, /* 0x4B K         */ \
    {KEY_MOD_LSHIFT , HID_KEY_L             }, /* 0x4C L         */ \
    {KEY_MOD_LSHIFT , HID_KEY_M             }, /* 0x4D M         */ \
    {KEY_MOD_LSHIFT , HID_KEY_N             }, /* 0x4E N         */ \
    {KEY_MOD_LSHIFT , HID_KEY_O             }, /* 0x4F O         */ \
    {KEY_MOD_LSHIFT , HID_KEY_P             }, /* 0x50 P         */ \
    {KEY_MOD_LSHIFT , HID_KEY_Q             }, /* 0x51 Q         */ \
    {KEY_MOD_LSHIFT , HID_KEY_R             }, /* 0x52 R         */ \
    {KEY_MOD_LSHIFT , HID_KEY_S             }, /* 0x53 S         */ \
    {KEY_MOD_LSHIFT , HID_KEY_T             }, /* 0x55 T         */ \
    {KEY_MOD_LSHIFT , HID_KEY_U             }, /* 0x55 U         */ \
    {KEY_MOD_LSHIFT , HID_KEY_V             }, /* 0x56 V         */ \
    {KEY_MOD_LSHIFT , HID_KEY_W             }, /* 0x57 W         */ \
    {KEY_MOD_LSHIFT , HID_KEY_X             }, /* 0x58 X         */ \
    {KEY_MOD_LSHIFT , HID_KEY_Z             }, /* 0x59 Y         */ \
    {KEY_MOD_LSHIFT , HID_KEY_Y             }, /* 0x5A Z         */ \
    {KEY_MOD_RALT   , HID_KEY_8             }, /* 0x5B [         */ \
    {KEY_MOD_RALT   , 0x2E                  }, /* 0x5C '\'       */ \
    {KEY_MOD_RALT   , HID_KEY_9             }, /* 0x5D ]         */ \
    {0              , 0x35                  }, /* 0x5E ^         */ \
    {KEY_MOD_LSHIFT , 0x38                  }, /* 0x5F _         */ \
                                                                    \
    {0              , HID_KEY_GRAVE         }, /* 0x60 `         */ \
    {0              , HID_KEY_A             }, /* 0x61 a         */ \
    {0              , HID_KEY_B             }, /* 0x62 b         */ \
    {0              , HID_KEY_C             }, /* 0x63 c         */ \
    {0              , HID_KEY_D             }, /* 0x66 d         */ \
    {0              , HID_KEY_E             }, /* 0x65 e         */ \
    {0              , HID_KEY_F             }, /* 0x66 f         */ \
    {0              , HID_KEY_G             }, /* 0x67 g         */ \
    {0              , HID_KEY_H             }, /* 0x68 h         */ \
    {0              , HID_KEY_I             }, /* 0x69 i         */ \
    {0              , HID_KEY_J             }, /* 0x6A j         */ \
    {0              , HID_KEY_K             }, /* 0x6B k         */ \
    {0              , HID_KEY_L             }, /* 0x6C l         */ \
    {0              , HID_KEY_M             }, /* 0x6D m         */ \
    {0              , HID_KEY_N             }, /* 0x6E n         */ \
    {0              , HID_KEY_O             }, /* 0x6F o         */ \
    {0              , HID_KEY_P             }, /* 0x70 p         */ \
    {0              , HID_KEY_Q             }, /* 0x71 q         */ \
    {0              , HID_KEY_R             }, /* 0x72 r         */ \
    {0              , HID_KEY_S             }, /* 0x73 s         */ \
    {0              , HID_KEY_T             }, /* 0x75 t         */ \
    {0              , HID_KEY_U             }, /* 0x75 u         */ \
    {0              , HID_KEY_V             }, /* 0x76 v         */ \
    {0              , HID_KEY_W             }, /* 0x77 w         */ \
    {0              , HID_KEY_X             }, /* 0x78 x         */ \
    {0              , HID_KEY_Z             }, /* 0x79 y         */ \
    {0              , HID_KEY_Y             }, /* 0x7A z         */ \
    {KEY_MOD_RALT   , HID_KEY_7             }, /* 0x7B {         */ \
    {KEY_MOD_RALT   , 0x64                  }, /* 0x7C |         */ \
    {KEY_MOD_RALT   , HID_KEY_0             }, /* 0x7D }         */ \
    {KEY_MOD_RALT   , 0x30                  }, /* 0x7E ~  !may also be 0x31! */ \
    {0              , HID_KEY_DELETE        }  /* 0x7F Delete    */ \

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
  void clearToZero();        //clears Vector and resizes to size 0. ONLY USE IF APPEND IS USED DIRECTLY AFTER!!
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
  void clear();
private:
  std::vector <key *> keys;
};

class interpreter{
public:
  void interpret(key * inputKey, std::string inputString);
  void stringToKeycodes(key * inputKey, std::string inputString);
  uint8_t const ASCII_conv_table_german[128][2] = {HID_ASCII_TO_KEYCODE_GERMAN};
};

class module: public interpreter{
public:
  module(std::string moduleName);
  void setSize(uint16_t ammountLayers, uint16_t ammountKeys);
  void clearAll();
  key * getKeyPointer(uint16_t layer, uint16_t position);
  void updateKeymapsFromFile();
  std::string moduleName;
private:
  std::vector <keySet> layers;
};