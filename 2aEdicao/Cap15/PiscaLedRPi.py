import RPi.GPIO as GPIO
import time
led = 10
GPIO.setmode(GPIO.BOARD)
GPIO.setup(led, GPIO.OUT)
while True:
    GPIO.output(led, GPIO.LOW)
    time.sleep(1)
    GPIO.output(led, GPIO.HIGH)
    time.sleep(1)

