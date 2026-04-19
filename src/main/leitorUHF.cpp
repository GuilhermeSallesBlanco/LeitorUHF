// Codigo de implementacao da classe do leitor RFID. Deve, principalmente, conter a funcao de leitura do RFID.

#include "leitorUHF.h"
#include <Arduino.h>

// Construtor
LeitorUHF::LeitorUHF() {};

bool LeitorUHF::begin(HardwareSerial *serial, int baud, uint8_t rxPin, uint8_t txPin){
    // Processo de inicializacao
    _serial = serial;
    _serial->begin(baud, SERIAL_8N1, rxPin, txPin);
    return true;
}

void LeitorUHF::loop(){
    // Processo de loop
}

void LeitorUHF::poll(){
    // Processo de leitura do RFID
}