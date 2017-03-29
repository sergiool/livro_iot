-- um servidor http simples
srv = net.createServer(net.TCP)
srv:listen(80, function(conn)
    conn:on("receive", function(sck, payload)
        print(payload)
        sck:send("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1> Olá, mundo! </h1>")
    end)
    conn:on("sent", function(sck) sck:close() end)
end)
