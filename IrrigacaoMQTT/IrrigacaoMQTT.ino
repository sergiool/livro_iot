#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <Arduino.h>

#define SOLENOIDE D0

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

double umidadeMaxima = 60.0, umidadeMinima = 40.0;
bool solenoideLigada = false, caixaCheia = false;

void connect(); // <- predefine connect() for setup()

void setup() {
  Serial.begin(115200);
  WiFi.begin("ssid", "pass");
  pinMode(D0, OUTPUT);
  client.begin("broker.shiftr.io", net);

  connect();

}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("esp8266", "cap_iot_", "iotiot256")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/UmidadeMaxima");
  client.subscribe("/UmidadeMinima");
  client.subscribe("/NivelAgua");
}

void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if(!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if(millis() - lastMillis > 1000) {
    lastMillis = millis();
    double umidade = double(analogRead(A0))/10.23;
    client.publish("/Umidade", String(umidade, 2));
    if (solenoideLigada){
      if (umidade > umidadeMaxima){
        digitalWrite(D0, LOW);
        client.publish("/Solenoide", "Desliga");
        solenoideLigada = false;
      }
    } else {
      if (umidade < umidadeMinima && caixaCheia){
        digitalWrite(D0, HIGH);
        client.publish("/Solenoide", "Liga");
        solenoideLigada = true;
      }
    }
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("Mensagem recebida: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
  if (topic == "/UmidadeMaxima")
    umidadeMaxima = payload.toFloat();
  if (topic == "/UmidadeMinima")
    umidadeMinima = payload.toFloat();
  if (topic == "/NivelAgua"){
    if (payload == "Baixo"){
      caixaCheia = false;
        digitalWrite(D0, LOW);
        client.publish("/Solenoide", "Desliga");
        solenoideLigada = false;
    }  else if (payload == "Alto")
      caixaCheia = true;
  }
}
