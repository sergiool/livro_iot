$(function(){
  var client = mqtt.connect(‘mqtt://cap_iot_:iotiot256@broker.shiftr.io’, {
    clientId: ‘javascript’
  });

  client.on(‘connect’, function(){
    console.log(‘Cliente conectado!’);
  });

  $(‘#button’).click(function(){
   client.publish(‘/PorteiroEletronico’, 'Abre');
      window.alert('Comando enviado');	
  })
})
