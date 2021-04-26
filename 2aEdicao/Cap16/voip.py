# -*- coding: utf-8 -*-
import time
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(17,GPIO.IN)
#Váriável de controle do botão. 
entradaAnterior = False

def chamada_voip():
   saida = subprocess.check_output("linphonecsh status register", \
                            stderr=subprocess.STDOUT, shell=True)
   if saida.find('ERROR') != -1:
      saida2 = subprocess.check_output("linphonecsh init", stderr=subprocess.STDOUT, shell=True)
      if saida2 != '':
         print ("Erro na execuçãoo do linphonecsh init")
         exit(1)

   if saida == "registered=0" OR  saida.find('ERROR') != -1:
     saida = subprocess.check_output("linphonecsh register \
                --host sip.linphone.org \
                --username sergiool \
                --password 7819049455955895", stderr=subprocess.STDOUT, shell=True)
     if saida != '':
       print ("Erro na execução do linphonecsh register")
       exit(1) 

   saida = subprocess.check_output("linphonecsh dial sergiool5@sip.linphone.org")

while True:
  # Lê o botão do interfone
  entrada = GPIO.input(17)
  # Verifica se já tratou o acionamento, ou seja, se o botão foi apertado e solto
  if ((not entradaAnterior) and entrada):
    print("Botao pressionado")
    chamada_voip()

  #atualiza a entrada anterior
  entradaAnterior = entrada
  #pequena pausa não sobrecarregar o processador
  time.sleep(0.05)
