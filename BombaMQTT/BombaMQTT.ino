#include <ESP8266WiFi.h>
#include <MQTTClient.h>

#define BOIA D0
#define BOMBA D1

WiFiClient net;
MQTTClient cliente;

unsigned long lastMillis = 0;

bool solenoideLigada = false, bombaLigada = false;

void conectar() {
  Serial.print("Verificando wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nConectando...");
  while (!cliente.connect("esp8266", "cap_iot_", "iotiot256")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Conectado!");

  cliente.subscribe("/Solenoide");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin("ssid", "senha");
  pinMode(BOMBA, OUTPUT);
  pinMode(BOIA, INPUT);
  cliente.begin("broker.shiftr.io", net);

  conectar();

}


void loop() {
  cliente.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if(!cliente.connected()) {
    conectar();
  }

  // Publica uma mensagem periodicamente a cada segundo.
  if(millis() - lastMillis > 1000) {
    lastMillis = millis();
    int nivel = digitalRead(BOIA);
    if (nivel == LOW)
      cliente.publish("/NivelAgua", "Baixo");
    else if (nivel == HIGH)
          cliente.publish("/NivelAgua", "Alto");

    if (bombaLigada && nivel == LOW){
        digitalWrite(BOMBA, LOW);
        cliente.publish("/Bomba", "Desliga");
        bombaLigada = false;
    }  else if (solenoideLigada && nivel == HIGH && !bombaLigada){
        digitalWrite(BOMBA, HIGH);
        cliente.publish("/Bomba", "Liga");
        bombaLigada = true;
    }
  }
}

// Função requerida e chamada pela biblioteca MQTT, exatamente nesse formato
void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("Mensagem recebida: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
  if (topic == "/Solenoide"){
    if (payload == "Liga")
      solenoideLigada = true;
    else if (payload == "Desliga")
      solenoideLigada = false;
  }
}
