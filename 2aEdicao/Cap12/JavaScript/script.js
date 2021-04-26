$(function(){
  var client = mqtt.connect('mqtt://cap_iot_:iotiot256@broker.shiftr.io', {
    clientId: 'javascript'
  });

  client.on('connect', function(){
    console.log('Cliente conectado!');

    client.subscribe('/Umidade');
    client.subscribe('/Solenoide');
    client.subscribe('/Bomba');
    client.subscribe('/NivelAgua');

  });

  client.on('message', function(topic, message) {
    var msg = message.toString();
    console.log('Nova mensagem', topic, msg);
    var node = document.createElement("LI");
    var textnode = document.createTextNode(topic.concat(": ", msg));
    node.appendChild(textnode);
    document.getElementById("myList").appendChild(node);
  });

  $('#button').click(function(){
   client.publish('/UmidadeMinima', document.getElementById("min").value);
   client.publish('/UmidadeMaxima', document.getElementById("max").value);
   window.alert("Valores enviados");	
  })
})
