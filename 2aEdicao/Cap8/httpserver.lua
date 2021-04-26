-- um servidor http simples
wifi.setmode(wifi.STATION)
print(wifi.sta.getip())
--nil ou IP previamente configurado
wifi.sta.config("ssid","senha")
wifi.sta.connect()
while (wifi.sta.getip() == nil) do
    print("Conectando...")
    tmr.delay(1000000)
end
print("Conectado, IP é "..wifi.sta.getip())
srv = net.createServer(net.TCP)
srv:listen(80, function(conn)
    conn:on("receive", function(sck, payload)
        print(payload)
        sck:send("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1> Olá, mundo! </h1>")
    end)
    conn:on("sent", function(sck) sck:close() end)
    end)
