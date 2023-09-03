#include "filesystem.h"
#include <iostream>

module typeuwu("typeuwu");

int main(void){
    typeuwu.setSize((uint16_t)5,(uint16_t)104);
    std::cout << "done" << std::endl;
    key * tmp = typeuwu.getKeyPointer(2,50);
    char charToInterpret = 'U';
    tmp->appendKeysycode(ASCII_conv_table_german[charToInterpret][1], ASCII_conv_table_german[charToInterpret][0], 1, 1);
    for(int i = 0; i < 2; i++){
        keysycode tmpk = tmp->getKeysycode(i);
        std::cout << "Keycode: " << (int)tmpk.keycode << std::endl;
        std::cout << "Modifier: " << (int)tmpk.modifier << std::endl;
        std::cout << "ReportID: " << (int)tmpk.reportID << std::endl;
        std::cout << "immedieateSend: " << (int)tmpk.immedieateSend << std::endl << std::endl;
    }
    typeuwu.setSize((uint16_t)1,(uint16_t)2);
    tmp = typeuwu.getKeyPointer(0,1);
    charToInterpret = 'U';
    tmp->appendKeysycode(ASCII_conv_table_german[charToInterpret][1], ASCII_conv_table_german[charToInterpret][0], 1, 1);
    for(int i = 0; i < 2; i++){
        keysycode tmpk = tmp->getKeysycode(i);
        std::cout << "Keycode: " << (int)tmpk.keycode << std::endl;
        std::cout << "Modifier: " << (int)tmpk.modifier << std::endl;
        std::cout << "ReportID: " << (int)tmpk.reportID << std::endl;
        std::cout << "immedieateSend: " << (int)tmpk.immedieateSend << std::endl << std::endl;
    }
    while(1);
}