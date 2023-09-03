#include "filesystem.h"

////////////////////////////KEY////////////////////////
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

void key::setAnalog(uint16_t value){
    this->analogValue = value;
    return;
}

uint16_t key::getAnalog(){
    return this->analogValue;
}

/////////////////////////KEYSET////////////////////////////
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

////////////////////////MODULE/////////////////////
module::module(std::string moduleName){
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