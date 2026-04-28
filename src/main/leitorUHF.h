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
        uint8_t _buffer[RX_BUFFER_LENGTH] = {0};
        bool dataAvailable();
        bool dataValid();
        bool receiveData(unsigned long timeOut = 500); // Timeout de 500ms para leitura dos dados do serial
        uint8_t calculateCheckSum(uint8_t *buffer);

    public:
    // O que o usuario pode usar, como a funcao de leitura do RFID
        LeitorUHF();
        uint8_t uid[12] = {0}; // Buffer para armazenar o UID da tag lida
        bool begin(HardwareSerial *serial = &Serial2, int baud = 115200, uint8_t rxPin = 16, uint8_t txPin = 17); 
        // Inicializacao da comunicacao serial, com parametros padrao para o Serial2 do ESP32
        void loop();
        void poll();
        void dumpUIDToSerial();

    // Estrutura do frame de comunicação    
    enum EstruturaFrame : byte {
        HeaderPos = 0x00,
        TypePos = 0x01,
        CommandPos = 0x02, 
        ParamLengthMSBPos = 0x03,
        ParamLengthLSBPos = 0x04,
        ParamPos = 0x05, // Parâmetros sempre começam na posição 5, se existirem
        // O frame completo terá o formato: [Header][Type][Command][ParamLengthMSB+ParamLengthLSB][Parameters][Checksum][End]
        // Checksum é o LSB da soma dos bytes desde Type até o último parâmetro.
    };

    // Constantes para header e end do frame
    enum ControleFrame : byte {
        FrameHeader = 0xAA,
        FrameEnd = 0xDD,
    };

    // Constantes para tipos de frame
    enum TipoFrame : byte {
        TipoFrame_Comando = 0x00,
        TipoFrame_Resposta = 0x01,
        TipoFrame_Notif = 0x02,
    };

    // Constantes para comandos 
    enum Comandos : byte {
        CMD_GetModuleInfo = 0x03,
        CMD_SinglePollInstruction = 0x22,
        CMD_MultiplePollInstruction = 0x27,
        CMD_StopMultiplePoll = 0x28,
        CMD_SetSelectParameter = 0x0C,
        CMD_GetSelectParameter = 0x0B,
        CMD_SetSendSelectInstruction = 0x12,
        CMD_ReadLabel = 0x39,
        CMD_WriteLabel = 0x49,
        CMD_LockLabel = 0x82,
        CMD_KillTag = 0x65,
        CMD_GetQueryParameters = 0x0D,
        CMD_SetQueryParameters= 0x0E,
        CMD_SetWorkArea = 0x07,
        CMD_SetWorkingChannel = 0xAB,
        CMD_GetWorkingChannel = 0xAA,
        CMD_SetAutoFrequencyHopping = 0xAD,
        CMD_AcquireTransmitPower = 0xB7,
        CMD_SetTransmitPower = 0xB6,
        CMD_SetTransmitContinuousCarrier = 0xB0,
        CMD_GetReceiverDemodulatorParameters = 0xF1,
        CMD_SetReceiverDemodulatorParameters = 0xF0,
        CMD_TestRFInputBlockingSignal = 0xF2,
        CMD_TestChannelRSSI = 0xF3,
        CMD_ControlIOPort = 0x1A,
        CMD_ModuleSleep = 0x17,
        CMD_SetModuleIdleSleepTime = 0x1D,
        CMD_ExecutionFailure = 0xFF,
    };

    // Constantes para erros
    enum Erros : byte {
        ERR_CommandError = 0x17,
        ERR_FHSSFail = 0x20,
        ERR_InventoryFail = 0x15,
        ERR_AccessFail = 0x16,
        ERR_ReadFail = 0x09,
        ERR_WriteFail = 0x10,
        ERR_LockFail = 0x13,
        ERR_KillFail = 0x12,
    };
};

#endif