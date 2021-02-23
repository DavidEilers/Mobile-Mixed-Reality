package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    Button button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        button = (Button) findViewById(R.id.button);

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d("network", "button pressed");
                String[] msgs = getMessagesAsString();

                TextView tv = findViewById(R.id.sample_text);

                String text = "";

                for (String msg : msgs) {
                    text += msg + "\n";
                }

                tv.setText(text);
            }
        });

        startListening();

        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        Log.d("onCreate, stringFromJNI", stringFromJNI());
    }

    @Override
    protected void onPause() {
        super.onPause();
        stopListening();
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        Log.d("onPause, stringFromJNI", stringFromJNI());
    }

    @Override
    protected void onResume() {
        super.onResume();
        startListening();
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        Log.d("onResume, stringFromJNI", stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void startListening();

    public native void stopListening();

    public native void sendString();

    public native String[] getMessagesAsString();
}
