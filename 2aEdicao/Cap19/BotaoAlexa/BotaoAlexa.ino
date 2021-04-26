#include <ESP8266WiFi.h>
#include "SinricPro.h"
#include "SinricProSwitch.h"

#define APP_KEY           "46ce709d-66c4-4c94-9c8d-164ca0022270"      // Chave obtida no menu Credentials
#define APP_SECRET        "f4e2b0c1-4021-4c59-a1ab-049550843e4b-50d410a6-4592-41a9-9058-fe9e8a46a4f3"   // Chave obtida no menu Credentials
#define BOTAO           D7
const String DEVICE_ID ="5f0f9f61b486fd0ff68a7647" ;   // Identificador obtido na criação do dispositivo no menu "Devices"

// Definições WiFi
const String ssid     = "xxxx";
const String password = "xxxxx";

void setupSinric() {
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.println("Conectado com Sinric"); }); 
  SinricPro.onDisconnected([](){ Serial.println("Desconectado do Sinric"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
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
  pinMode(BOTAO, INPUT_PULLUP); // Define como entrada
  setupSinric();
}

void verificaBotao(){
  unsigned long atualMillis = millis(); // get actual millis() and keep it in variable actualMillis
  static unsigned long ultimoMillis = 0;
  bool estado =  false;
  if (digitalRead(BOTAO) == LOW && atualMillis - ultimoMillis > 1000)  { // Se botão foi pressionado há mais de um segundo
    estado = !estado; // Inverte o estado
    
    SinricProSwitch& dispositivo = SinricPro[DEVICE_ID];
    dispositivo.sendPowerStateEvent(estado); // Envia o nodo estado para o Sinric 
    
    Serial.printf("Dispositivo " + DEVICE_ID + estado ?" ligado.":" desligado.");

    ultimoMillis = atualMillis;  // atualiza a variável ultimoMillis
  } 
}

void loop() {
  verificaBotao();
  SinricPro.handle();
}
