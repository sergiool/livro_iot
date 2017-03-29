cfg =
{
    ip="192.168.1.1",
    netmask="255.255.255.0",
    gateway="192.168.1.1"
    ssid="ssid"
    pwd="senha"

}
 wifi.ap.config(cfg)
 wifi.setmode(wifi.SOFTAP)
 wifi.ap.dhcp.start()
