// Codigo principal do programa. Ele deve:
  // Inicializar o leitor RFID
  // Ficar rodando em um loop
  // Pedir leituras periodicas
  // Chamar uma funcao para carregar o codigo HTML (verificar)

#include "leitorUHF.h"

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
