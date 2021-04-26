wifi.setmode(wifi.STATION)
print(wifi.sta.getip())
--nil ou IP previamente configurado
wifi.sta.config("AP 801","bc6967bec")
wifi.sta.connect()
while (wifi.sta.getip() == nil) do
    print("Conectando...")
    tmr.delay(1000000)
end
print("Conectado, IP é "..wifi.sta.getip())
http.get("http://httpbin.org/ip", nil, function(code, data)
    if (code < 0) then 
      print("HTTP request failed")
    else
      print(code, data)
    end
end)