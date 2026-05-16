// Codigo principal do programa. Ele deve:
  // Inicializar o leitor RFID
  // Ficar rodando em um loop
  // Pedir leituras periodicas
  // Comparar tag lida com os usuários cadastrados
  // Chamar uma funcao para carregar o codigo HTML (verificar)

// TODO:
  // Definir permissões para cada cenário
  // Interação back end e front end

#include "leitorUHF.h"
#include "usuario.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <WebServer.h>

unsigned long lastPollTime = 0;
LeitorUHF leitorUHF;

Usuario usuarios[6] = {
  {
    // TAG 1
    .uid = {0x80, 0x69, 0x15, 0x00, 0x00, 0x50, 0x1D, 0x63, 0xE6, 0x6C, 0x47, 0x22},
    .nome = "Carlos Silva",
    .cargo = "Administrador",
    .permissao = PERM_ADMIN
  },

  {
    // TAG 2
    .uid = {0x80, 0x69, 0x15, 0x00, 0x00, 0x40, 0x1D, 0x63, 0xE6, 0x70, 0x47, 0x7E},
    .nome = "Fernanda Rocha",
    .cargo = "Supervisor",
    .permissao = PERM_SUPERVISOR
  },

  {
    // TAG 3
    .uid = {0x80, 0x69, 0x15, 0x00, 0x00, 0x50, 0x1D, 0x63, 0xE6, 0x74, 0x47, 0xA8},
    .nome = "Julio Mendes",
    .cargo = "Supervisor",
    .permissao = PERM_SUPERVISOR
  },

  {
    // TAG 4
    .uid = {0x80, 0x69, 0x15, 0x00, 0x00, 0x50, 0x1D, 0x63, 0xE6, 0x78, 0x47, 0xED},
    .nome = "Ana Costa",
    .cargo = "Funcionário",
    .permissao = PERM_FUNCIONARIO
  },

  {
    // TAG 5
    .uid = {0x80, 0x69, 0x15, 0x00, 0x00, 0x40, 0x1D, 0x63, 0xE6, 0x7C, 0x47, 0x3B},
    .nome = "Rafael Oliveira",
    .cargo = "Funcionário",
    .permissao = PERM_FUNCIONARIO
  },

  {
    // TAG 6
    .uid = {0x80, 0x69, 0x15, 0x00, 0x00, 0x40, 0x1D, 0x63, 0xE6, 0x80, 0x47, 0x6D},
    .nome = "Visitante",
    .cargo = "Visitante",
    .permissao = PERM_VISITANTE
  }
};

void setup() {
  Serial.begin(115200); // Inicia o monitor serial
  leitorUHF.begin(&Serial2, 115200, 16, 17); // Inicia o leitor
}

void loop() {
  leitorUHF.loop();
  if(leitorUHF.novaLeitura){
    leitorUHF.novaLeitura = false;
    Serial.println("Nova leitura detectada, comparando com usuários cadastrados...");
    for(int i = 0; i < 6; i++){
      if(compararUID(usuarios[i].uid, leitorUHF.uid)){
        Serial.print("Usuário identificado: ");
        Serial.print(usuarios[i].nome);
        Serial.print(" - Cargo: ");
        Serial.print(usuarios[i].cargo);
        Serial.println();
        // Manda um sinal pro front end indicando que o usuário foi identificado, junto com o nome, cargo e permissão do usuário.
        break;
      }
    }
  }
  if(millis() - lastPollTime > 1000){
    leitorUHF.poll(); // Envia um comando de leitura a cada 1 segundo
    lastPollTime = millis();
  }
  //delay(1000); 
  // Delay removido para evitar conflitos. O comando de leitura já é controlado por um timer, então não é necessário um delay adicional aqui.
}
