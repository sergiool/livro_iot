#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#define LM35  A0 // Define o pino que irá ler a saída do LM35

WiFiClient client;
MySQL_Connection conn(&client);

void setup() {
  IPAddress server_addr;
  Serial.begin(115200);
  delay(10);
    // Conecta à rede WiFi
  WiFi.begin("ssid", "senha");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  // Imprime o endereço IP
  Serial.println(WiFi.localIP());

  // Obtém o IP do servidor, pois a conexão não aceita nomes...
  WiFi.hostByName("db4free.net", server_addr);

  Serial.println(server_addr);
  Serial.println("Conectando ...");
  // Ip do Servidor, porta, usuário e senha
  if (conn.connect(server_addr, 3306, "sergiool_iot", "iotiot256")) {
    delay(1000);
  }
  else
    Serial.println("Falha na conexão");
}

void loop() {
  double temperatura = analogRead(LM35)*0.322;
  char toStr[10];
  // SQL Exemplo
  MySQL_Cursor cur_mem(&conn);
  String INSERT_SQL = "INSERT INTO iotiot.registro_temp (valor) VALUES (";
  INSERT_SQL += dtostrf(temperatura, 3, 1, toStr);
  INSERT_SQL += ")";

  Serial.println("Gravando dados ...");

  // Executa a consulta
  cur_mem.execute(INSERT_SQL.c_str());
  // Libera memória. Importante devido às limitações dessa plataforma
  cur_mem.close();
  delay(2000);
}
