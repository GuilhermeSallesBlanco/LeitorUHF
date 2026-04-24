// Codigo de implementacao da classe do leitor RFID. Deve, principalmente, conter a funcao de leitura do RFID.
// Funções do serial: _serial->available(), _serial->read(), _serial->write(), etc.

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
    // Processo de loop, integração do processo de leitura do RFID (poll, disponibilidade de dados, leitura pro buffer, etc.)
}

void LeitorUHF::poll(){
    // Processo de envio de frame para solicitar uma única leitura do RFID
    uint8_t frame[7] = {0};
    frame[0] = FrameHeader;
    frame[1] = TipoFrame_Comando; // 0x00
    frame[2] = CMD_SinglePollInstruction; // 0x22
    frame[3] = 0x00; // ParamLengthMSB
    frame[4] = 0x00; // ParamLengthLSB
    // Como não existe parametro para o comando de single poll, pula direto pro checksum.
    frame[5] = 0x22; // Checksum é o LSB da soma dos bytes desde Type (0x00) até o último parâmetro (0x00), ou seja, 0x22.
    frame[6] = FrameEnd;
    _serial->write(frame, 7); // Envia o frame completo para o leitor UHF
}

// Função para verificar se há dados disponíveis e processar leituras

// Função para enviar dados pro buffer