#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProLight.h"

#define APP_KEY           "46ce709d-66c4-4c94-9c8d-164ca0022270"      
                    // Chave obtida no menu Credentials
#define APP_SECRET        "f4e2b0c1-4021-4c59-a1ab-049550843e4b-50d410a6-4592-41a9-9058-fe9e8a46a4f3"   // Chave obtida no menu Credentials
#define DEVICE_ID          "5f0f9f61b486fd0ff68a7647"    
                  // Identificador obtido na criação do dispositivo no menu "Devices"
#define LAMPADA           D7 // Porta a ser usada pela lâmpada

// Definições WiFi
const String ssid     = "xxxx";
const String password = "xxxxx";
 
bool onPowerState (const String &deviceId, bool &estado) {
  Serial.println("Dispositivo " +  deviceId + estado?" ligado.":" desligado.");
  digitalWrite(LAMPADA, estado); 
  return true; // Indicando execução com sucesso
}

void setupSinric() {
 
  SinricProLight &dispositivo = SinricPro[DEVICE_ID];
  
  // função callback para acionamento do dispositivo
  dispositivo.onPowerState(onPowerState);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.println("Conectado com Sinric"); }); 
  SinricPro.onDisconnected([](){ Serial.println("Desconectado do Sinric"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(115200); 
  Serial.println("\nConectando à rede: " + ssid);
  WiFi.begin(ssid, password);
  Serial.print("Verificando wifi...");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
   }
  
  Serial.print("WiFi conectado. Endereço IP: ");
  Serial.println(WiFi.localIP());
  pinMode(LAMPADA, OUTPUT);
  setupSinric();
}

void loop() {
  SinricPro.handle();
}
