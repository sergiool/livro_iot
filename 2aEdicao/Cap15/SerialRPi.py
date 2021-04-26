import smbus
import time
# Inicializa a comunicação
bus = smbus.SMBus(0)

# Define o endereço no qual se encontra o ESP8266 – escravo
escravo = 0x20

while True:
    numero = input("Digite um número: ")
    if numero:
    # Função para enviar dado para o escravo
        bus.write_byte(endereco, numero)
        print ("Foi enviado: ", numero)
        # Espera um segundo
        time.sleep(1)
        # Função para requisitar e receber dado do escravo
        dobro = bus.read_byte(endereco)
        print ("ESP respondeu com ", dobro)

