cfg={ssid="teste", pwd="senha123"}
wifi.setmode(wifi.SOFTAP)
wifi.ap.config(cfg)
wifi.ap.dhcp.start()
