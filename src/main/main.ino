#include "leitorUHF.h"
#include "usuario.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <WebServer.h>
#include <LittleFS.h>

#define MODO_TESTE 0 // 1 para ativar modo de teste, 0 para desativar

const char* ssid = "TCCRede";
const char* password = "12345678";

unsigned long lastPollTime = 0;
LeitorUHF leitorUHF;
WebServer server(80);

unsigned long inicioTeste = 0;
const unsigned long DURACAO_TESTE = 60000; // 60 segundos

bool testeIniciado = false;
bool resultadoImpresso = false;

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
  String uid = ultimoUIDLido;
  ultimoUIDLido = "";

  server.send(200, "text/plain", uid);
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

void reiniciarTeste() {
    inicioTeste = millis();
    resultadoImpresso = false;
    testeIniciado = true;

    for (int i = 0; i < 6; i++) {
        usuarios[i].contadorLeituras = 0;
    }

    Serial.println();
    Serial.println("==================================");
    Serial.println("NOVO TESTE INICIADO");
    Serial.println("Duracao: 60 segundos");
    Serial.println("==================================");
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

  reiniciarTeste();
}

void loop() {
  server.handleClient();
  leitorUHF.loop();
  if(leitorUHF.novaLeitura){
    leitorUHF.novaLeitura = false;
    //Serial.println("Nova leitura detectada, comparando com usuários cadastrados...");
    for(int i = 0; i < 6; i++){
      if(compararUID(usuarios[i].uid, leitorUHF.uid)){
        usuarios[i].contadorLeituras++;
        ultimoUIDLido = uidParaString(leitorUHF.uid);
        #if !MODO_TESTE
        Serial.print("Usuário identificado: ");
        Serial.print(usuarios[i].nome);
        Serial.print(" - Cargo: ");
        Serial.print(usuarios[i].cargo);
        Serial.println();
        #endif
        break;
      } else if (i == 5){
        #if !MODO_TESTE
        Serial.print("Usuário desconhecido. UID lido: ");
        leitorUHF.dumpUIDToSerial();
        #endif
        break;
      } 
    }
  }
  if(millis() - lastPollTime > 100){
    leitorUHF.poll(); // Envia um comando de leitura a cada 0.1 segundo
    lastPollTime = millis();
  }
  
  #if MODO_TESTE
  if(testeIniciado && !resultadoImpresso && (millis() - inicioTeste >= DURACAO_TESTE)){
    resultadoImpresso = true;
    Serial.println();
    Serial.println("========== RESULTADO ==========");

    uint32_t total = 0;

    for(int i = 0; i < 6; i++)
        total += usuarios[i].contadorLeituras;

    for(int i = 0; i < 6; i++)
    {
        Serial.print(usuarios[i].nome);
        Serial.print(": ");

        Serial.print(usuarios[i].contadorLeituras);

        if(total > 0)
        {
            float porcentagem =
                100.0 * usuarios[i].contadorLeituras / total;

            Serial.print(" (");
            Serial.print(porcentagem,1);
            Serial.print("%)");
        }

        Serial.println();
    }

    Serial.print("Total de leituras: ");
    Serial.println(total);

    Serial.println("==============================");

    delay(2000);

    reiniciarTeste();
  }
  #endif 

  //delay(1000); 
  // Delay removido para evitar conflitos. O comando de leitura já é controlado por um timer, então não é necessário um delay adicional aqui.
}
