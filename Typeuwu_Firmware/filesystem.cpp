#include "filesystem.h"

#include "SPI.h"
#include <SdFat.h>
#include "Adafruit_SPIFlash.h"
#include "Adafruit_TinyUSB.h"

#include "flash_config.h"

Adafruit_SPIFlash flash(&flashTransport);

// file system object from SdFat
FatVolume fatfs;

FatFile root;
FatFile file;

// USB Mass Storage object
Adafruit_USBD_MSC usb_msc;

// Check if flash is formatted
bool fs_formatted;

// Set to true when PC write to flash
bool fs_changed;

// Callback invoked when received READ10 command.
// Copy disk's data to buffer (up to bufsize) and 
// return number of copied bytes (must be multiple of block size) 
int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize)
{
  // Note: SPIFLash Block API: readBlocks/writeBlocks/syncBlocks
  // already include 4K sector caching internally. We don't need to cache it, yahhhh!!
  return flash.readBlocks(lba, (uint8_t*) buffer, bufsize/512) ? bufsize : -1;
}

// Callback invoked when received WRITE10 command.
// Process data in buffer to disk's storage and 
// return number of written bytes (must be multiple of block size)
int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize)
{
  //digitalWrite(LED_BUILTIN, HIGH);

  // Note: SPIFLash Block API: readBlocks/writeBlocks/syncBlocks
  // already include 4K sector caching internally. We don't need to cache it, yahhhh!!
  return flash.writeBlocks(lba, buffer, bufsize/512) ? bufsize : -1;
}

// Callback invoked when WRITE10 command is completed (status received and accepted by host).
// used to flush any pending cache.
void msc_flush_cb (void)
{
  // sync with flash
  flash.syncBlocks();

  // clear file system's cache to force refresh
  fatfs.cacheClear();

  fs_changed = true;

  //digitalWrite(LED_BUILTIN, LOW);
}


// the setup function runs once when you press reset or power the board
void filesystemSetup()
{
  //pinMode(LED_BUILTIN, OUTPUT);

  flash.begin();

  // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
  usb_msc.setID("Typeuwu", "BIG DICK FLASH", "6.9");

  // Set callback
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

  // Set disk size, block size should be 512 regardless of spi flash page size
  usb_msc.setCapacity(flash.size()/512, 512);

  // MSC is ready for read/write
  usb_msc.setUnitReady(true);

  usb_msc.begin();

  // Init file system on the flash
  fs_formatted = fatfs.begin(&flash);

  //Serial.begin(115200);
  //while ( !Serial ) delay(10);   // wait for native usb

  if ( !fs_formatted )
  {
    #ifdef SERIAL_DEBUG
    Serial.println("Failed to init files system, flash may not be formatted");
    #endif
  }
  #ifdef SERIAL_DEBUG
  Serial.print("JEDEC ID: 0x"); Serial.println(flash.getJEDECID(), HEX);
  Serial.print("Flash size: "); Serial.print(flash.size() / 1024); Serial.println(" KB");
  #endif
  fs_changed = true; // to print contents initially
}

void filesystemLoop()
{
  if ( fs_changed )
  {
    fs_changed = false;

    // check if host formatted disk
    if (!fs_formatted)
    {
      fs_formatted = fatfs.begin(&flash);
    }

    // skip if still not formatted
    if (!fs_formatted) return;

    Serial.println("Opening root");

    if ( !root.open("/") )
    {
      Serial.println("open root failed");
      return;
    }

    Serial.println("Flash contents:");

    // Open next file in root.
    // Warning, openNext starts at the current directory position
    // so a rewind of the directory may be required.

    while ( file.openNext(&root, O_RDONLY) )
    {
      file.printFileSize(&Serial);
      Serial.write(' ');
      file.printName(&Serial);
      if ( file.isDir() )
      {
        // Indicate a directory.
        Serial.write('/');
      }
      Serial.println();
      file.close();
    }

    root.close();

    Serial.println();
    delay(1000); // refresh every 1 second
  }
}

bool check_fs_changed(){
  return fs_changed;
}

void filesystemClear(){
  if (!flash.eraseChip()) {
    Serial.println("Failed to erase chip!");
  }

  flash.waitUntilReady();
  Serial.println("Successfully erased chip!");
  
  String filename_to_open = "Layout.txt";
  if(!fatfs.exists(filename_to_open)){
    Serial.println("Layout.txt does not exist");
    Serial.println("Trying to create Layout.txt...");
    File32 writeFile = fatfs.open("Layout.txt", FILE_WRITE);
    if (!writeFile) {
      Serial.println("Error, failed to open Layout.txt for writing!");
      while(1) yield();
    }
    Serial.println("Created Layout.txt succesfully");
    writeFile.println("This is a test line, please ignore...");             //perhaps implement default config file...
    writeFile.close();
  }
}

void filesystemCreateConfig(){
  String filename_to_open = "Layout.txt";
  if(!fatfs.exists(filename_to_open)){
    Serial.println("Layout.txt does not exist");
    Serial.println("Trying to create Layout.txt...");
    File32 writeFile = fatfs.open("Layout.txt", FILE_WRITE);
    if (!writeFile) {
      Serial.println("Error, failed to open Layout.txt for writing!");
      while(1) yield();
    }
    Serial.println("Created Layout.txt succesfully");
    writeFile.println("This is a test line, please ignore...");             //perhaps implement default config file...
    writeFile.close();
  }
  return;
}


//-----------------------key------------------------------
key::key(){
    keycodes.push_back({0,0,0,0});
}

void key::appendKeysycode(uint8_t keycode, uint8_t modifier, uint8_t reportID, uint8_t immediateSend){
    keycodes.push_back({keycode, modifier, reportID, immediateSend});
    return;
}

void key::clear(){
    keycodes.resize(0);
    keycodes.push_back({0,0,0,0});
    return;
}

void key::clearToZero(){
    keycodes.resize(0);
    return;
}

keysycode key::getKeysycode(uint16_t position){
    if(position < keycodes.size()) return keycodes.at(position);
    else return {0,0,0,0};
}

uint16_t key::getKeycodesSize(){
  return (uint16_t)keycodes.size();
}

void key::setAnalog(uint16_t value){
    this->analogValue = value;
    return;
}

uint16_t key::getAnalog(){
    return this->analogValue;
}

//------------------------keySet--------------------------

keySet::keySet(){
    keys.push_back(key());
}

void keySet::setSize(uint16_t ammountKeys){
    keys.resize(ammountKeys, key());
    return;
}

key * keySet::getKeyPointer(uint16_t position){
    if(position < keys.size()) return &keys.at(position);
    else return nullptr;
}

void keySet::clear(){
    keys.resize(0);
    keys.push_back(key());
    return;
}

//-----------------------module-------------------------

module::module(String moduleName){
    this->moduleName = moduleName;
    layers.push_back(keySet());
}

void module::setSize(uint16_t ammountLayers, uint16_t ammountKeys){
    layers.resize(ammountLayers, keySet());
    for(uint16_t i = 0; i < ammountLayers; i++){
        layers.at(i).setSize(ammountKeys);
    }
    return;
}

void module::clearAll(){
    layers.resize(0);
    layers.push_back(keySet());
    return;
}

key * module::getKeyPointer(uint16_t layer, uint16_t position){
    if(layer < layers.size()) return layers.at(layer).getKeyPointer(position);
    else return nullptr;
}

void module::updateKeymapsFromFile(){
  Serial.println("update Keymaps from file");
  File32 Layout = fatfs.open(CONFIG_FILENAME, FILE_READ);
  if(!Layout){
    Layout.close();
    filesystemCreateConfig();
    Layout = fatfs.open(CONFIG_FILENAME, FILE_READ);
    if(!Layout) while(1){
      Serial.println("failed to open Config!");
      delay(500);
    }
  }

  String fileAll;
  while (Layout.available()) {
    fileAll.concat((char)Layout.read());
  }

  Layout.close();

  MatchState fileAllSearch;
  int fileAllLength = fileAll.length() + 1;      // Safety Margin because of '\0' and all that
  char *buf = new char[fileAllLength];           // may be a waste of space, but we got 16MB... I mean come on...
  for(uint16_t i = 0; i < fileAll.length()-1; i++) buf[i] = '\0';   //clear buffer
  
  char *fileAllBuffer = new char[fileAllLength];
  fileAll.toCharArray(fileAllBuffer, fileAllLength);

  fileAllSearch.Target(fileAllBuffer);

  // search it
  String targetModule = "begin%(" + this->moduleName + "%)(.*)end%(" + this->moduleName + "%)";     //perhaps change (.*) to .* or something
  char result = fileAllSearch.Match (targetModule.c_str(), 0);


  String moduleString;
  // check results

  switch (result)
  {
    case REGEXP_MATCHED:

      Serial.println ("-----");
      Serial.print ("Captured: ");
      
      moduleString += fileAllSearch.GetCapture(buf, 0);
      Serial.println (moduleString.c_str());

      // matching offsets in ms.capture

      Serial.print ("Captures: ");
      Serial.println (fileAllSearch.level);
      break;

    case REGEXP_NOMATCH:
      Serial.println ("No match.");
      break;

    default:
      Serial.print ("Regexp error: ");
      Serial.println (result, DEC);
      break;

  }  // end of switch

  delete[] fileAllBuffer;
  delete[] buf;

  // return;          //worked till here!
  
  String moduleStringSecond = moduleString.substring(0);

  uint16_t layerMax = 1;
  uint16_t positionMax = 1;

  uint16_t layerCurrent;
  uint16_t positionCurrent;
  
  // First run, to get max vals
  while(moduleString.length() > 1){       //perhaps make > 1
    Serial.println("running first while");
    String line = moduleString.substring(0, moduleString.indexOf("\n"));
    Serial.print("line: ");Serial.println(line.c_str());
    moduleString.remove(0, moduleString.indexOf("\n") + 1);     //perhaps add +1 to end index...

    int lineLength = line.length() + 1;     // because of '\0' and all that
    char *lineBuffer = new char[lineLength];
    buf = new char[lineLength];
    line.toCharArray(lineBuffer, lineLength);

    MatchState lineSearch;
    lineSearch.Target(lineBuffer);
    if(lineSearch.Match("Layer (%d+)", 0) == REGEXP_MATCHED){
      layerCurrent = atoi(lineSearch.GetCapture(buf, 0));
      Serial.print("Layer current: ");Serial.println(layerCurrent);
      if(layerCurrent > layerMax) layerMax = layerCurrent;
    }
    if(lineSearch.Match("Button (%d+)", 0) == REGEXP_MATCHED){
      positionCurrent = atoi(lineSearch.GetCapture(buf, 0));
      Serial.print("Position Current: ");Serial.println(positionCurrent);
      if(positionCurrent > positionMax) positionMax = positionCurrent;
    }
    delete[] buf;
    if(moduleString.indexOf("\n") == -1) break;
  }
  Serial.print("layerMax: ");Serial.println(layerMax);
  Serial.print("positionMax: "); Serial.println(positionMax);
  //return;     //worked till here
  this->setSize(layerMax + (uint16_t)1, positionMax + (uint16_t)1);     // + 1 because start counting at 1 not 0
  //return;     //worked till here
  //Second Run to get Strings
  while(moduleStringSecond.length() > 0){       //perhaps make > 1
  Serial.println("running second while");
    String stringToInterpret;
    String line = moduleStringSecond.substring(0, moduleStringSecond.indexOf("\n"));
    Serial.print("moduleStringSecond: ");Serial.println(moduleStringSecond.c_str());
    Serial.print("line: ");Serial.println(line.c_str());
    //return;
    moduleStringSecond.remove(0, moduleStringSecond.indexOf("\n") + 1);     //perhaps add +1 to end index...

    int lineLength = line.length() + 1;     // because of '\0' and all that
    char *lineBuffer = new char[lineLength];
    buf = new char[lineLength];
    line.toCharArray(lineBuffer, lineLength);

    MatchState lineSearch;
    lineSearch.Target(lineBuffer);
    if(lineSearch.Match("Layer (%d+)", 0) == REGEXP_MATCHED){
      layerCurrent = atoi(lineSearch.GetCapture(buf, 0));
    }
    if(lineSearch.Match("Button (%d+): (.*)", 0) == REGEXP_MATCHED){
      positionCurrent = atoi(lineSearch.GetCapture(buf, 0));
      Serial.print("positionCurrent: ");Serial.println(positionCurrent);
      stringToInterpret += lineSearch.GetCapture(buf, 1);
      interpret(this->getKeyPointer(layerCurrent, positionCurrent), stringToInterpret);       //CRASHED somewere in here!!!!
      stringToInterpret.remove(0);
    }
    delete[] buf;
    if(moduleStringSecond.indexOf("\n") == -1) break;
  }
  return;
}

//------------------------------interpreter-------------------------------------
void interpreter::interpret(key * inputKey, String inputString){
  if(inputKey == nullptr){
    Serial.println("Nullus Mullus!!");
    return;
  }

  int itterations = 0;

  while(inputString.length() > 0 && itterations < MAX_WHILE_ITTERATIONS){
    int inputLength = inputString.length() + 1;
    char * inputBuffer = new char[inputLength];
    inputString.toCharArray(inputBuffer, inputLength);
    char * buf = new char[inputLength];
    String keycodeBufferString;

    MatchState inputSearch;
    inputSearch.Target(inputBuffer);
    if(inputSearch.Match("^([%a%d_]+)") == REGEXP_MATCHED){
      keycodeBufferString += inputSearch.GetCapture(buf, 0);
      stringToKeycodes(inputKey, keycodeBufferString);
      inputString.remove(0, keycodeBufferString.length());
    }
    else break;

    keycodeBufferString.remove(0);
    delete[] inputBuffer;
    delete[] buf;
    itterations++;
  }
  if(inputKey->getKeycodesSize() == 1) inputKey->isSingleKey == 1;
}

void interpreter::stringToKeycodes(key * inputKey, String inputString){
  // clear keysycode vector with key.clearToZero()

  inputKey->clearToZero();
  for(uint16_t i = 0; i < inputString.length(); i++){           //may need +1
    uint8_t sendimediate = 0;
    if(inputString.charAt(i+1) != -1){
      if(ASCII_conv_table_german[inputString.charAt(i)][0] != ASCII_conv_table_german[inputString.charAt(i + 1)][0]){
        sendimediate = 1;
      }
    }
    inputKey->appendKeysycode(ASCII_conv_table_german[inputString.charAt(i)][1], ASCII_conv_table_german[inputString.charAt(i)][0], RID_KEYBOARD, sendimediate);
  }
  return;
}

// make seperate Funktion to check for immediate Send!!!! Will make Life a lot easier


