-- Conexao na rede Wifi
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","senha")
wifi.sta.connect()
tmr.alarm(1, 1000, tmr.ALARM_AUTO, function()
     if wifi.sta.getip() == nil then
         print("Conectando...")
     else
         tmr.stop(1)
         print("Conectado! IP é "..wifi.sta.getip())
     end
end)

pin=0
led=0
-- Função para piscar o LED
function pisca()
     if led==0 then
         led=1
         gpio.write(pin,gpio.LOW)
     else
         led=0
         gpio.write(pin,gpio.HIGH)
     end
end

-- Temporizador
tmr.alarm(0,1000, tmr.ALARM_AUTO, function() pisca() end )

-- Servidor Web
srv=net.createServer(net.TCP)
srv:listen(80,function(conn)
    conn:on("receive", function(client,request)
        page = "<h1>Exemplo WEB - Pisca</h1>"
     page = page.."<p><a href=\"?com=PISCA\"><button><b>PISCA LED</b> </button></a>"
        page = page.."<p><a href=\"?com=DESLIGA\"><button><b>DESLIGA</b> </button></a>"
        client:send(page)
        client:close()
        if(string.find(request, "PISCA")) then
              tmr.start(0)
        elseif(string.find(request, "DESLIGA")) then
              tmr.stop(0)
        end
    end)
end)
