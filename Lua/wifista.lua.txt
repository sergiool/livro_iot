print(wifi.sta.getip())
--nil
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","password")
wifi.sta.connect()
while (wifi.sta.getip() == nil) do
    print("Conectando...")
    tmr.delay(1000000)
end
print("Conectado, IP é "..wifi.sta.getip())
