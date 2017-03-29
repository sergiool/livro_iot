enduser_setup.start(
  function()
    print("Conectado ao WiFi como:" .. wifi.sta.getip())
    enduser_setup.stop()
  end,
  function(err, str)
    print("enduser_setup: Erro #" .. err .. ": " .. str)
  end);
