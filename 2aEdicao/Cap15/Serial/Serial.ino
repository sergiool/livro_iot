#include <Wire.h>

#define ENDERECO 0x20
int numero = 0;

void setup() {
    // Inicia o I2C como escravo
    Wire.begin(ENDERECO);

    // Escravo apenas trata eventos pelas funções callback
    Wire.onReceive(recebeDados);
    Wire.onRequest(enviaDados);
}

void loop() {
    delay(100);
}

// Callback para recepção de dados 
void recebeDados(int cont) {

    while(Wire.available()) {
        numero = Wire.read();
     }
}

// Callback para envio de dados a partir da requisição do mestre
void enviaDados() {
    Wire.write(numero*2);
}
