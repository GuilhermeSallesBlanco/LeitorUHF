#ifndef LEITORUHF_H
#define LEITORUHF_H

// Codigo de definicao da classe do leitor RFID. Deve, principalmente, conter a funcao de leitura do RFID.
// No caso, esta sendo tratado como uma biblioteca.

// Inclusao de bibliotecas
#include <stdint.h>
#include <Arduino.h>

#define RX_BUFFER_LENGTH 64 // Tamanho do buffer de leitura serial

// Definicao da classe 
class LeitorUHF {

    private:
    // Variaveis internas, como ponteiro de comunicacao serial, buffer de leitura, etc.
        
        HardwareSerial *_serial;
        // Ponteiro para comunicacao serial

        uint8_t _buffer[RX_BUFFER_LENGTH] = {0};
        // Buffer para armazenar dados recebidos do leitor UHF

    public:
    // O que o usuario pode usar, como a funcao de leitura do RFID
        
        LeitorUHF();

        bool begin(HardwareSerial *serial = &Serial2, int baud = 115200, uint8_t rxPin = 16, uint8_t txPin = 17); 
        // Inicializacao da comunicacao serial, com parametros padrao para o Serial2 do ESP32

        void loop();
        // Funcao a ser chamada no loop principal, para verificar se ha dados disponiveis e processar leituras

        void poll();
        // Funcao para solicitar uma leitura do RFID, enviando o comando apropriado para o leitor UHF


};

#endif