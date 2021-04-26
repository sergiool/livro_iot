// Definição das bibliotecas 
#include <MQTT.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

// Definição das portas de I/O utilizadas 
#define PORTAO_ABERTO D7
#define PORTAO_FECHADO D4
#define ABRE_PORTAO D5
#define FECHA_PORTAO D6

// Definições para o Firebase
#define NOME_PROJETO "portaoiotfirebase.firebaseio.com"
#define CHAVE_API "AIzaSyA3BIJLE3jb_TvOyTjEOczPVJgdSWSgH6k"
#define CHAVE_SERVIDOR "AAAAXyuy7hw:APA91bHaY-bEFwGni99BYxaQJUJgup65OBfCeucqyM-3P0dHoelkKsSVNLPKjsOBOB0YoL6TMrUNdsxncIJnSncUKTso1jE_JpSCiBknspxEoicimSpyhNdQr_asvlq9RMqREryfqs-l"

// Definições MQTT 
const char* broker = "broker.shiftr.io";
const char * mqtt_user = "sergiool";
const char * mqtt_pass = "7597a5305b78b3ef";

// Definições WiFi
const String ssid     = "SSID";
const String password = "xxxxxxx";

// Para o MQTT
WiFiClient net;
MQTTClient cliente;

// Para conexão ao Firebase
FirebaseData firebaseData;

// Definição dos estados 
enum estado {ABRINDO, FECHANDO, ABERTO, FECHADO} est;

void setup() {
  Serial.begin(115200);

// Configura as portas 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ABRE_PORTAO, OUTPUT);
  pinMode(FECHA_PORTAO, OUTPUT);
  pinMode(PORTAO_ABERTO, INPUT_PULLUP);
  pinMode(PORTAO_FECHADO, INPUT_PULLUP);
  digitalWrite(ABRE_PORTAO, HIGH);
  digitalWrite(FECHA_PORTAO, HIGH);      

  Serial.println("Conectando à rede: " + ssid);
  WiFi.mode(WIFI_STA);
  if (conectar()){
     Serial.print("WiFi conectado. Endereço IP: ");
     Serial.println(WiFi.localIP());  
     iniciaFirebase();
     enviaNotificacao("Iniciando o portão");
  }
  est = (digitalRead(PORTAO_FECHADO) == HIGH) ? FECHADO : ABERTO;
}

void iniciaFirebase(){
    
  // Configuração do projeto Firebase
  Firebase.begin(NOME_PROJETO, CHAVE_API);
  
    // Informar a chave do servidor
  firebaseData.fcm.begin(CHAVE_SERVIDOR);
  
  // Informar o tópico que deve receber a notificação
  firebaseData.fcm.setTopic("Portao");
}

void enviaNotificacao(String mensagem){
    // Define título e mensagem da notificção
  firebaseData.fcm.setNotifyMessage("Portão IoT", mensagem);
  
  // Define o formato da mensagem
  firebaseData.fcm.setDataMessage("{\"myData\":\"myValue\"}");
  
  // Envia mensagem para o dispositivo cujo token foi cadastrado
  if (Firebase.sendTopic(firebaseData)) {
    // Sucesso
    Serial.println(firebaseData.fcm.getSendResult());
  } else {
    // Falha
    Serial.println(firebaseData.errorReason());
  }
}

boolean conectar() {

  WiFi.begin(ssid, password);
  Serial.print("Verificando wifi...");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries++ < 20) {
    Serial.print(".");
    delay(1000);
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Falha ao conectar");
    return false;
  }

// Configuração do MQTT
  cliente.begin(broker, net);
// Associa o método que vai tratar as mensagens recebidas pelo MQTT
  cliente.onMessage(messageReceived);
  
  Serial.print("\nConectando MQTT...");
  tries = 0;
  while (!cliente.connect("esp8266", mqtt_user, mqtt_pass) && tries++ < 20) {
    Serial.print(".");
    delay(1000);
  }
  if (tries >= 20){
    Serial.print("Falha na conexão MQTT");
    return false;
  }
  
  Serial.println("Conectado!:");
  cliente.subscribe("/Portao");
  return true;
}

void loop() {
  
  cliente.loop(); // Verifica requisições MQTT
  
  static int led = LOW;
  static unsigned long lastMillis = 0;

  delay(10); 
   
  if(!cliente.connected()) {
// Se houve queda de conexão, tenta conectar novamente
    conectar();
  }

// A depender do estado em que se encontra o portão, as ações são executadas
  switch(est){
    case ABRINDO:
      if (digitalRead(PORTAO_ABERTO) == HIGH){
        digitalWrite(ABRE_PORTAO, HIGH);
        est = ABERTO;
        enviaNotificacao("Portão aberto!");
        Serial.println("Portão aberto");
      }
      break;
    case FECHANDO:
      if (digitalRead(PORTAO_FECHADO)== HIGH){
        digitalWrite(FECHA_PORTAO, HIGH);
        est = FECHADO;
        enviaNotificacao("Portão fechado!");
        Serial.println("Portão fechado");
      }
      break;
   default:
      break;
  }
  if(millis() - lastMillis > 2000) { // Pisca LED para indicar que está funcionando
    lastMillis = millis();
    led = (led == LOW) ? HIGH : LOW;
    digitalWrite(LED_BUILTIN, led);   
  }
}

// Método para tratamento das mensagens recebidas
void messageReceived(String &topic, String &payload) {
  Serial.println("Mensagem recebida: " + topic + " - " + payload);
  payload.remove(payload.length()-1); // Retira lixo ao final do comando

  Serial.print("Portão aberto:"); // Mostra situação dos sensores
  Serial.println((digitalRead(PORTAO_ABERTO) == LOW) ? "LOW":"HIGH");

  Serial.print("Portão fechado:");
  Serial.println((digitalRead(PORTAO_FECHADO) == LOW) ? "LOW":"HIGH");
  
  if (payload == "Abre"){
    if (digitalRead(PORTAO_ABERTO) == LOW){
      digitalWrite(FECHA_PORTAO, HIGH);
      digitalWrite(ABRE_PORTAO, LOW);
      est = ABRINDO;
      Serial.println("Abrindo portão");
    }
  }
  if (payload == "Fecha"){
    if (digitalRead(PORTAO_FECHADO) == LOW) {
      digitalWrite(ABRE_PORTAO, HIGH);
      digitalWrite(FECHA_PORTAO, LOW);
      est = FECHANDO;
      Serial.println("Fechando portão.");
    }    
  }
  if (payload == "Para"){
      digitalWrite(ABRE_PORTAO, HIGH);
      digitalWrite(FECHA_PORTAO, HIGH);
  }
}
