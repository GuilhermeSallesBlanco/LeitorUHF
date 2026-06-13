#include "leitorUHF.h"
#include "usuario.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <WebServer.h>
#include <LittleFS.h>

const char* ssid = "TCCRede";
const char* password = "12345678";

unsigned long lastPollTime = 0;
LeitorUHF leitorUHF;
WebServer server(80);

String ultimoUIDLido = "";

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

void handleRoot(){
  File file = LittleFS.open("/index.html", "r");

  if(!file){
    server.send(404, "text/plain", "index.html nao encontrado");
    return;
  }

  server.streamFile(file, "text/html");
  file.close();
}

void handleCSS(){
  File file = LittleFS.open("/style.css", "r");
  server.streamFile(file, "text/css");
  file.close();
}

void handleScript(){
  File file = LittleFS.open("/script.js", "r");
  server.streamFile(file, "application/javascript");
  file.close();
}

void handleDatabase(){
  File file = LittleFS.open("/database.js", "r");
  server.streamFile(file, "application/javascript");
  file.close();
}

void handleRFIDAPI(){
  File file = LittleFS.open("/rfid-api.js", "r");
  server.streamFile(file, "application/javascript");
  file.close();
}

void handleRFID() {
  server.send(200, "text/plain", ultimoUIDLido);

  // limpa após enviar
  ultimoUIDLido = "";
}

String uidParaString(const uint8_t* uid) {
  String resultado = "";

  for (int i = 0; i < 12; i++) {
    if (uid[i] < 16) {
      resultado += "0";
    }

    resultado += String(uid[i], HEX);
  }

  resultado.toUpperCase();

  return resultado;
}

void setup() {
  Serial.begin(115200); // Inicia o monitor serial
  leitorUHF.begin(&Serial2, 115200, 16, 17); // Inicia o leitor

  if(!LittleFS.begin(true)){
  Serial.println("Erro ao montar LittleFS");
  return;
  }

  Serial.println("LittleFS montado");

  File root = LittleFS.open("/");
  File file = root.openNextFile();

  Serial.println("Arquivos encontrados:");

  while(file){
  Serial.println(file.name());
  file = root.openNextFile();
  }

  Serial.println();
  Serial.println("Conectando ao Wifi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Conectado!");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/script.js", handleScript);
  server.on("/database.js", handleDatabase);
  server.on("/rfid-api.js", handleRFIDAPI);
  server.on("/rfid", handleRFID);

  server.begin();

  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
  leitorUHF.loop();
  if(leitorUHF.novaLeitura){
    leitorUHF.novaLeitura = false;
    Serial.println("Nova leitura detectada, comparando com usuários cadastrados...");
    for(int i = 0; i < 6; i++){
      if(compararUID(usuarios[i].uid, leitorUHF.uid)){
        ultimoUIDLido = uidParaString(leitorUHF.uid);
        Serial.print("Usuário identificado: ");
        Serial.print(usuarios[i].nome);
        Serial.print(" - Cargo: ");
        Serial.print(usuarios[i].cargo);
        Serial.println();
        break;
      } else if (i == 5){
        Serial.print("Usuário desconhecido. UID lido: ");
        leitorUHF.dumpUIDToSerial();
        // Manda um sinal pro front end indicando que o usuário é desconhecido, junto com o UID lido.
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
