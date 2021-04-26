# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import time
portao = 11
GPIO.setmode(GPIO.BOARD)
GPIO.setup(portao, GPIO.OUT)
def na_conexao(cliente, dados, retorno):
   print('Conectado. Codigo de retorno:' + str(retorno))
# Tópico assinado na conexão. Caso desconecte e seja necessário reconectar, o tópico é assinado novamente
   client.subscribe('/PorteiroEletronico')
# Função callback que trata a chegada de um comando para abrir o portão

def na_publicacao(cliente, dados, msg):
   print ('Topico: '+  msg.topic+ '\nMensagem:' + str(msg.payload))
   if (str(msg.payload) == 'Abre'):
      GPIO.output(led, GPIO.HIGH)
      time.sleep(0.05)
      GPIO.output(led, GPIO.LOW)

client = mqtt.Client()
client.on_connect = na_conexao
client.on_message = na_publicacao
client.username_pw_set('cap_iot_', 'iotiot256')
client.connect('broker.shiftr.io', 1883, 60)
# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()

