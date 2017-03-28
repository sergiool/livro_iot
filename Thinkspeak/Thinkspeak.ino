#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define VOLTAGEM_MAX 1.0
#define VOLTAGEM_MAXCOUNTS 1023.0
WiFiClient  cliente;

const unsigned long NumeroCanal = 169736;
const char * ChaveEscrita = "42F62ANNENZUXZOM";

void setup()
{

   Serial.begin(115200);
   delay(250);
   Serial.println();
   Serial.println("Iniciando....");

   WiFi.begin("ssid", "senha");
   int tentativas = 0;

   while ((WiFi.status() != WL_CONNECTED) && tentativas++ < 20) {
      delay(500);
      Serial.print(".");
   }
   ThingSpeak.begin(cliente);
}


void loop()
{

    float sensor, tensao, corrente;
    // Ler a corrente D0 = 1 e D1 = 0
    digitalWrite(D0, 1);
    digitalWrite(D1, 0);
    sensor = analogRead(A0);
    tensao = sensor / 1023.0 * 3300.0;
    corrente = (tensao - 1830.0) / 48.4;
    ThingSpeak.writeField(NumeroCanal, 1, corrente, ChaveEscrita);

    // Ler a tensão da bateria D0 = 0 e D1 = 1
    digitalWrite(D0, 0);
    digitalWrite(D1, 1);
    sensor = analogRead(A0);
    tensao = sensor * 18.3 / 1023.0;
    ThingSpeak.writeField(NumeroCanal, 2, tensao, ChaveEscrita);

    delay(20000); // ThingSpeak aceita atualizações a partir de 15s
}
