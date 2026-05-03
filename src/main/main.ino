// Codigo principal do programa. Ele deve:
  // Inicializar o leitor RFID
  // Ficar rodando em um loop
  // Pedir leituras periodicas
  // Chamar uma funcao para carregar o codigo HTML (verificar)

// TODO:
  // Criar um array de structs para armazenar os UIDs lidos
    // Struct de UID deve ter nome e permissão, pelo menos
  // Fazer uma seleção de cenário (botão físico?) para escolher qual cenário usar (isso deve ser refletido no site)
  // Definir permissões para cada cenário

#include "leitorUHF.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <WebServer.h>

unsigned long lastPollTime = 0;
LeitorUHF leitorUHF;

void setup() {
  Serial.begin(115200); // Inicia o monitor serial
  leitorUHF.begin(&Serial2, 115200, 16, 17); // Inicia o leitor
}

void loop() {
  leitorUHF.loop();
  if(millis() - lastPollTime > 1000){
    leitorUHF.poll(); // Envia um comando de leitura a cada 1 segundo
    lastPollTime = millis();
  }
  delay(1000);
}
