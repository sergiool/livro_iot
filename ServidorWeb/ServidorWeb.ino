#include <ESP8266WiFi.h>
const char* ssid = "ssid";
const char* senha = "senha";
WiFiServer server(80); // Define a porta a ser usada
unsigned long MiliAnterior = 0;
int led = LOW;
int val = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Configura a porta D0 do LED
  pinMode(D0, OUTPUT);
  digitalWrite(D0, 0);

  // Iniciando a conexão WiFi

  Serial.print("Conectando para a rede...  ");
  Serial.println(ssid);

  WiFi.begin(ssid, senha);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(“.”);
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (val)// Pisca apenas quando a variável val está habilitada
    if(millis() - MiliAnterior >= 1000) { // 1 segundo
      MiliAnterior = millis();
      if (led == LOW)
        led = HIGH;  // Desliga o LED - Lógica invertida
      else
        led = LOW;   // Liga o LED
      digitalWrite(D0, led);
    }

  // Verifica se algum cliente conectou
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Espera até o cliente enviar sua requisição
  Serial.println("Novo Cliente");
  while(!client.available()){
    delay(1);
  }

  // Lê a primeira linha da requisição
  String req = client.readStringUntil(‘\r’);
  Serial.println(req);
  client.flush();

  // Verifica se um botão foi acionado

  if (req.indexOf("DESLIGA") != -1)
    val = 0;
  else if (req.indexOf("PISCA") != -1)
    val = 1;

  client.flush(); // Limpa o buffer do cliente

  // Envia resposta em formato HTML
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s +="<!DOCTYPE HTML>\r\n<html><h1>Exemplo WEB - Pisca</h1>";
  s +="<p><a href=\”?com=PISCA\”><button><b>PISCA LED</b> </button></a>";
  s +="<p><a href=\”?com=DESLIGA\”><button><b>DESLIGA</b> </button></a>";
  s +="</html>\n";

  client.print(s);
  delay(1);
  Serial.println("Cliente desconectado");

}

