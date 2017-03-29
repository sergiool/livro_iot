#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup(void){

  Serial.begin(115200);
  Serial.println();
  Serial.println("Iniciando...");
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin("ssid", "password");

  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    WiFi.begin("ssid", "senha");
    Serial.println("Falha no Wifi, tentando novamente...");
  }

  httpUpdater.setup(&httpServer);
  httpServer.begin();

  Serial.print("HTTPUpdateServer pronto! Vá para http://");
  Serial.print(WiFi.localIP());
  Serial.println("/update no seu navegador\n");
}

void loop(void){
  httpServer.handleClient();
}
