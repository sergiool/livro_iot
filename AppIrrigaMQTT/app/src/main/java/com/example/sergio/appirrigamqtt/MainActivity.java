package com.example.sergio.appirrigamqtt;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.media.RingtoneManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.app.NotificationCompat;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.UnsupportedEncodingException;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.media.RingtoneManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.app.NotificationCompat;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;


public class MainActivity extends AppCompatActivity {

    // Declarações de Campos EditText
    private EditText UMaxima, UMinima;
    private ProgressBar pB;
    private ImageView iV;
    private TextView uText;
    private TextView nText;


    // Declaração para as mensagens de depuração
    private static final String TAG = MainActivity.class.getSimpleName();

    // Declarações e inicializações para o MQTT
    private String clientId = MqttClient.generateClientId();
    private MqttAndroidClient client;
    boolean assinou = false;

    // Declarações e inicializações para as notificações
    private NotificationCompat.Builder mBuilder = new NotificationCompat.Builder(this);
    private TaskStackBuilder stackBuilder = TaskStackBuilder.create(this);
    private NotificationManager mNotificationManager;
    private int cont = 0;

    private MqttCallback ClientCallBack = new MqttCallback() {
        @Override
        public void connectionLost(Throwable cause) {
            Log.d(TAG,"Perda de conexão... Reconectando...");
            connectMQTT();
            assinou = false;
        }

        @Override
        public void messageArrived(String topic, MqttMessage message) throws Exception {
            Log.d(TAG, topic + ": " + new String(message.getPayload()));
            if (topic.equals("/Umidade")) { // Apresentada graficamente
                int Umidade = Integer.parseInt(message.getPayload().toString());
                pB.setProgress(Umidade);
                uText.setText(message.getPayload().toString());
                Log.d(TAG, "Atualizando progresso: " + Umidade);
            } else if (topic.equals("/NivelAgua")) {
                nText.setText(message.getPayload().toString());
                if (message.getPayload().toString().equals("Alto")) {
                    Log.d(TAG, "Atualizando imagem!");
                    iV.setImageResource(R.drawable.caixacheia);
                } else
                    iV.setImageResource(R.drawable.caixavazia);
            } else {
                Log.d(TAG, "Incoming message: " + new String(message.getPayload()));
                // The message was published
                int mId = 99;
                mBuilder.setSmallIcon(R.drawable.caixacheia)
                        .setContentTitle("Mensagem recebida")
                        .setContentText(topic + ": " + message.toString())
                        .setVibrate(new long[]{150, 300, 150, 600}) // Para Vibrar
                        .setSound(RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION));
                // Para apitar
                // Executa a notificação
                mNotificationManager.notify(mId, mBuilder.build());
            }
        }

        @Override
        public void deliveryComplete(IMqttDeliveryToken token) {
            Log.d(TAG,"Entregue!");

        }
    };

    private IMqttActionListener MqttCallBackApp = new IMqttActionListener() {
        @Override
        public void onSuccess(IMqttToken asyncActionToken) {
            Log.d(TAG, "onSuccess");
            if (!assinou) {
                subscribeMQTT();
                assinou = true;
            }
        }

        @Override
        public void onFailure(IMqttToken asyncActionToken,
                              Throwable exception) {
            // The subscription could not be performed, maybe the user was not
            // authorized to subscribe on the specified topic e.g. using wildcards
            Log.d(TAG, "onFailure");

        }
    };


    // Método de inicialização da Activity
    protected void onCreate(Bundle savedInstanceState) {
        // Inicializa interfaces
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        UMaxima = (EditText) findViewById(R.id.editTextMax);
        UMinima = (EditText) findViewById(R.id.editTextMax);
        pB = (ProgressBar) findViewById(R.id.progressBar);
        iV = (ImageView) findViewById(R.id.imageView);
        uText = (TextView) findViewById(R.id.textView);
        nText = (TextView) findViewById(R.id.textView2);


        initMQTT();
        connectMQTT();
        startNotifications();

    }

    private void initMQTT(){
        client=new MqttAndroidClient(this.getApplicationContext(),
                "tcp://broker.shiftr.io:1883", clientId);
        client.setCallback(ClientCallBack);
    }

    private void connectMQTT(){
        // Inicialização do MQTT e conexão inicial
        try {
            MqttConnectOptions options = new MqttConnectOptions();
            options.setUserName("cap_iot_");
            options.setPassword("iotiot256".toCharArray());
            IMqttToken token = client.connect(options);
            token.setActionCallback(MqttCallBackApp);

        } catch (MqttException e) {
            e.printStackTrace();
        }
    }


    private void startNotifications(){
        // Cria as classes necessárias para notificações: Intent, PendingIntent e acessa o mNotificationManager
        Intent resultIntent = new Intent(this, MainActivity.class);
        stackBuilder.addParentStack(MainActivity.class);
        stackBuilder.addNextIntent(resultIntent);
        PendingIntent resultPendingIntent =
                stackBuilder.getPendingIntent(
                        0,
                        PendingIntent.FLAG_UPDATE_CURRENT
                );
        mBuilder.setContentIntent(resultPendingIntent);
        mNotificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);

    }

    public void subscribeMQTT() {

        int qos = 1;
        try {
            if (!client.isConnected()){
                connectMQTT();
            }
            IMqttToken subTokenU = client.subscribe("/Umidade", qos);
            IMqttToken subTokenB = client.subscribe("/Bomba", qos);
            IMqttToken subTokenS = client.subscribe("/Solenoide", qos);
            IMqttToken subTokenN = client.subscribe("/NivelAgua", qos);
            subTokenU.setActionCallback(MqttCallBackApp);
            subTokenB.setActionCallback(MqttCallBackApp);
            subTokenS.setActionCallback(MqttCallBackApp);
            subTokenN.setActionCallback(MqttCallBackApp);

        } catch (MqttException e) {
            e.printStackTrace();
        }
    }

    public void onClickPub(View v) {
        try {
            if (!client.isConnected()){
                connectMQTT();
            }
            client.publish("/UmidadeMaxima",new MqttMessage(UMaxima.getText().toString().getBytes("UTF-8")));
            client.publish("/UmidadeMinima", new MqttMessage(UMinima.getText().toString().getBytes("UTF-8")));
        } catch (UnsupportedEncodingException | MqttException e) {
            e.printStackTrace();
        }
// Perform action on click
    }
}


