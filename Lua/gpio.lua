pinw=0
gpio.mode(pinw,gpio.OUTPUT)
gpio.write(pinw,gpio.HIGH)
pinr=1
gpio.mode(pinr,gpio.INPUT)
print(gpio.read(pinr))

