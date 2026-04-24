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

bool LeitorUHF::dataAvailable(){
    // Verifica se há dados disponíveis no serial para leitura
    return _serial->available() > 0;
}

bool LeitorUHF::receiveData(unsigned long timeOut){
    // Processo de leitura dos dados do serial para o buffer
    unsigned long startTime = millis();
    uint8_t bytesRecebidos = 0;
    // Liberar o buffer
    for (int i = 0; i < RX_BUFFER_LENGTH; i++){
        _buffer[i] = 0;
    }
    while ((millis() - startTime) < timeOut){
        while(dataAvailable()){
            uint8_t byteLido = _serial->read();
            if (bytesRecebidos > RX_BUFFER_LENGTH - 1){
                Serial.print("Erro: Buffer de leitura cheio. Retire tags próximas para liberar espaço.");
                return false;
            } else {
                _buffer[bytesRecebidos] = byteLido;
                // buffer[0] deve ser o header do frame (0xAA)
                // A posição do buffer é dada por bytesRecebidos, que é incrementada a cada byte lido.
                // Em teoria, a posição do buffer deve ser igual à posição do byte lido no frame.
                // Ou seja, buffer[0] deve ser frame[0] (header), buffer[1] deve ser frame[1] (tipo do frame), etc.
            } bytesRecebidos++;
            if (byteLido == FrameEnd){
                break; // Frame completo recebido
            }            
        }
    }
    if (bytesRecebidos > 1 && _buffer[0] == FrameHeader && _buffer[bytesRecebidos - 1] == FrameEnd){
        return true; // Frame recebido com sucesso
    } else {
        return false; // Frame não recebido ou frame incompleto
    }
    return false;
}

bool LeitorUHF::dataValid(){
    // Precisa calcular o checksum dos dados recebidos e comparar com o checksum recebido para validar os dados
    uint8_t checkSumCalculado = calculateCheckSum(_buffer);

    uint16_t tamanhoParametros = _buffer[3]; // Como explicado anteriormente, buffer [3] é frame[3], que deve ser o ParamLengthMSB.
    tamanhoParametros<<=8;
    tamanhoParametros += _buffer[4]; // buffer[4] é frame[4], que deve ser o ParamLengthLSB.
    uint8_t posCheckSumRecebido = 5 + tamanhoParametros; 

    return (checkSumCalculado == _buffer[posCheckSumRecebido]); // Retorna true se o checksum calculado for igual ao checksum recebido, indicando que os dados são válidos
}

uint8_t LeitorUHF::calculateCheckSum(uint8_t *buffer){
    // Calcula o checksum dos dados recebidos, somando os bytes desde Type até o último parâmetro e retornando o LSB da soma
    uint16_t tamanhoParametros = buffer[3]; // Como explicado anteriormente, buffer [3] é frame[3], que deve ser o ParamLengthMSB.
    tamanhoParametros<<=8;
    tamanhoParametros += buffer[4]; // buffer[4] é frame[4], que deve ser o ParamLengthLSB.
    
    uint16_t soma = 0;
    for(uint16_t i = 1; i < tamanhoParametros+5; i++){ // Comeca em 1 para pular o header (0), depois vai na sequencia: Type (1), Command (2), ParamLengthMSB (3), ParamLengthLSB (4), Parameters (Até tamanhoParametros)
        soma += buffer[i];
    }
    return (soma & 0xFF); // Retorna o LSB da soma
}

void LeitorUHF::loop(){
    // Processo de loop, integração do processo de leitura do RFID (poll, disponibilidade de dados, leitura pro buffer, etc.)
}