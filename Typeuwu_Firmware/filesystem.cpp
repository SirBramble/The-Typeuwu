#include "filesystem.h"

#include "SPI.h"
#include <SdFat.h>
#include "Adafruit_SPIFlash.h"

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

  digitalWrite(LED_BUILTIN, LOW);
}


// the setup function runs once when you press reset or power the board
void filesystemSetup()
{
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
  }
}

bool check_fs_changed(){
  return fs_changed;
}

void filesystenClear(){
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


//-----------------------key------------------------------
key::key(void){
  if(keycodes.empty()) keycodes.push_back({0,0,0,0});
  else keycodes.at(0) = {0,0,0,0};
}

void key::appendKeysycode(uint8_t keycode, uint8_t modifier, uint8_t reportID, uint8_t immediateSend){
  keysycode tmp = {keycode, modifier, reportID, immediateSend};
  keycodes.push_back(tmp);
  return;
}

void key::clear(){
  keycodes.resize(1);
  keycodes.at(0) = {0,0,0,0};
  return;
}

keysycode key::getKeysycode(uint16_t position){
  return keycodes.at(position);
}

//------------------------keySet--------------------------

keySet::keySet(void){
  keys.at(0) = nullptr;
}

void keySet::setSize(uint16_t ammountKeys){
  while(keys.size() > ammountKeys){
    delete keys.at(keys.size() - 1);
    keys.pop_back();
  }
  
  if(keys.size == ammountKeys) return;

  keys.resize(ammountKeys, nullptr);
  for(uint16_t i = 0; i < ammountKeys; i++){
    if(keys.at(i) == nullptr){
      keys.at(i) = new key();       //may cause problems, keep in mind
    }
  }
  return;
}

key * keySet::getKeyPointer(uint16_t position){
  return keys.at(position);
}

//-----------------------layerSet-------------------------

void layerSet::setSize(uint16_t ammountLayers, uint16_t ammountKeys){
  layers.resize(ammountLayers);
  for(uint16_t i = 0; i < ammountLayers; i++){
    layers.at(i).setSize(ammountKeys);
  }
  return;
}







