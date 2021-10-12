package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

import java.util.Timer;
import java.util.TimerTask;

public class JoinGameActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("full-lib");
    }

    Button connectBtn;
    Button qrCodeBtn;
    EditText ipEdit;
    Timer timer;
    GameModes gamemode;
    String gamemodeStr;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_join_game);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setHomeButtonEnabled(true);
        connectBtn = findViewById(R.id.button_connect);
        qrCodeBtn = findViewById(R.id.button_qrCode);
        ipEdit = findViewById(R.id.editText_ip);
        gamemodeStr = getIntent().getStringExtra("GameMode");
        gamemode = GameModes.valueOf(gamemodeStr);



        timer = new Timer("Update");

        TimerTask updateTask = new TimerTask() {
            @Override
            public void run() {
                if(hasSlaveConnectToMasterSucceeded()==true){
                    cancel();
                    switchToArCore();
                }
            }
        };
        timer.schedule(updateTask, 1000, 1000);


        connectBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //startButtonPressed();
                slaveConnectToMaster(ipEdit.getText().toString(),gamemodeStr);
            }
        });

        qrCodeBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                IntentIntegrator integrator = new IntentIntegrator(JoinGameActivity.this);
                integrator.setDesiredBarcodeFormats(IntentIntegrator.QR_CODE_TYPES);
                integrator.setPrompt("Scan");
                integrator.setCameraId(0);
                integrator.setBeepEnabled(false);
                integrator.setBarcodeImageEnabled(false);
                integrator.initiateScan();
            }
        });
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        if(item.getItemId() == android.R.id.home)
        {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
        if(result != null) {
            if(result.getContents() == null) {
                System.out.println("Cancelled scan");

            } else {
                System.out.println("Scanned");
                ipEdit.setText(result.getContents());
                //Toast.makeText(this, "Scanned: " + result.getContents(), Toast.LENGTH_LONG).show();
            }
        } else {
            // This is important, otherwise the result will not be passed to the fragment
            super.onActivityResult(requestCode, resultCode, data);
        }
    }

    private void switchToArCore(){
        System.out.println("Switching to ARCore");
        Intent switchActivityIntent = new Intent(this,ARCoreActivity.class);
        switchActivityIntent.putExtra("Type","slave");
        if(gamemode==GameModes.TICTACTOE){
            switchActivityIntent.putExtra("TTTGamePointer",getTTTGame());
        }else if(gamemode==GameModes.FOURINAROW) {
            switchActivityIntent.putExtra("FourGamePointer",getFourGame());
        }
        switchActivityIntent.putExtra("GameMode",gamemodeStr);
        startActivity(switchActivityIntent);
    }

    public native void slaveConnectToMaster(String ip,String gamemode);
    public native boolean hasSlaveConnectToMasterSucceeded();
    public native long getTTTGame();
    public native long getFourGame();

}