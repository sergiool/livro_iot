package com.sergiool.portaoiotfirebase;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.widget.Toast;

import com.ebanx.swipebtn.OnActiveListener;
import com.ebanx.swipebtn.SwipeButton;
import com.google.firebase.messaging.FirebaseMessaging;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {

    // Declaração para as mensagens de depuração
    private static final String TAG = MainActivity.class.getSimpleName();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        FirebaseMessaging.getInstance().subscribeToTopic("Portao");
        SwipeButton swipeButtonNoState = (SwipeButton) findViewById(R.id.swipe_btn);
        SwipeButton swipeButtonNoState2 = (SwipeButton) findViewById(R.id.swipe_btn2);
        SwipeButton swipeButtonNoState3 = (SwipeButton) findViewById(R.id.swipe_btn3);
        swipeButtonNoState.setOnActiveListener(new OnActiveListener() {
            @Override
            public void onActive() {
                sendMessage("Abre");
            }
        });

        swipeButtonNoState2.setOnActiveListener(new OnActiveListener() {
            @Override
            public void onActive() {
                sendMessage("Fecha");
            }
        });

        swipeButtonNoState3.setOnActiveListener(new OnActiveListener() {
            @Override
            public void onActive() {
                sendMessage("Para");
            }
        });
    }

    private void sendMessage(final String command) {
        // Do something in response to button click
        final OkHttpClient client = new OkHttpClient();
        final String basicAuth = "Basic " + Base64.encodeToString("sergiool:7597a5305b78b3ef".getBytes(), Base64.NO_WRAP);

        RequestBody formBody = new FormBody.Builder()
                .addEncoded(command, "")
                .build();
        Request request = new Request.Builder()
                .url("https://broker.shiftr.io/Portao")
                .addHeader("Content-Type", "application/x-www-form-urlencoded")
                .addHeader("Authorization", basicAuth)
                .post(formBody)
                .build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                Log.d(TAG, "error: " + e.getMessage());
                try {
                    final Response response = client.newCall(new Request.Builder()
                            .url("http://192.168.4.1/" + command)
                            .get()
                            .build()).execute();
                    final String resposta = response.body().string();
                    Log.i(TAG, resposta);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            if(resposta.compareTo("Ok!") == 0)
                                Toast.makeText(MainActivity.this, command, Toast.LENGTH_SHORT).show();
                            else
                                Toast.makeText(MainActivity.this, "Falha no envio", Toast.LENGTH_SHORT).show();
                        }
                    });
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
            @Override
            public void onResponse(Call call, final Response response) throws IOException {
                Log.i(TAG, response.body().string());
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, command, Toast.LENGTH_SHORT).show();
                    }
                });
            }
        });
    }
}