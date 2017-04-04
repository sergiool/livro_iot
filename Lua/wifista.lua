wifi.setmode(wifi.STATION)
﻿print(wifi.sta.getip())
--nil ou IP previamente configurado
wifi.sta.config("SSID","password")
wifi.sta.connect()
while (wifi.sta.getip() == nil) do
    print("Conectando...")
    tmr.delay(1000000)
end
print("Conectado, IP é "..wifi.sta.getip())
