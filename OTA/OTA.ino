#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>

void setup() {
int tentativas = 0;
Serial.begin(115200);
  delay(10);
    // Conecta à rede WiFi
  WiFi.begin("dd-wrt", "kedma256");

  while (WiFi.status() != WL_CONNECTED && tentativas++ < 10) {
    delay(500);
    Serial.println(".");
  }
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Falha na conexão! Reiniciando...");
    ESP.restart();
  }


  ArduinoOTA.onStart([]() {
    Serial.println("Inicio");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("Fim");
  });
  ArduinoOTA.onProgress([](unsigned int progresso, unsigned int total) {
    Serial.printf("Progresso: %u%%\r", (progresso / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t erro) {
    Serial.printf("Erro[%u]: ", erro);
    if (erro == OTA_AUTH_ERROR) Serial.println("Falha de Autenticação");
    else if (erro == OTA_BEGIN_ERROR) Serial.println("Falha na inicialização");
    else if (erro == OTA_CONNECT_ERROR) Serial.println("Falha de conexão");
    else if (erro == OTA_RECEIVE_ERROR) Serial.println("Falha no recebimento");
    else if (erro == OTA_END_ERROR) Serial.println("Falha na finalização");
  });
  ArduinoOTA.begin();
  Serial.println("Pronto!");
  Serial.print("Endereço IP: ");
  Serial.print(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
}
