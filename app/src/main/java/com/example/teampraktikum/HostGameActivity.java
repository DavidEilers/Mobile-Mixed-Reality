package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Bitmap;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.text.format.Formatter;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Switch;
import android.widget.TextView;

import androidmads.library.qrgenearator.QRGContents;
import androidmads.library.qrgenearator.QRGEncoder;

import org.w3c.dom.Text;

import java.util.Timer;
import java.util.TimerTask;

public class HostGameActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("full-lib");
    }

    ImageView imageView;
    String ipString="";
    Bitmap qrCode;
    LinearLayout main;
    Timer timer;
    String gamemodeStr;
    public final static int QRcodeWidth = 350 ;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_host_game);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setHomeButtonEnabled(true);
        imageView = findViewById(R.id.imageView);
        main = findViewById(R.id.main);
        gamemodeStr = getIntent().getStringExtra("GameMode");

        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try{
                    WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
                    String ipAddress = Formatter.formatIpAddress(wm.getConnectionInfo().getIpAddress());
                    TextView ipView = (TextView) findViewById(R.id.ipView);
                    ipView.setText("Your IP-Adress is: "+ipAddress);
                    ipView.setVisibility(View.VISIBLE);
                    ipString = ipAddress;
                }catch(Exception e){
                    System.out.println("Exception : "+e.toString());
                }
            }
        });
        thread.start();
        try{
            thread.join();
        }catch (Exception e){
            System.out.println("Join thread: "+e.toString());
        }

        timer = new Timer("Update");

        TimerTask updateTask = new TimerTask() {
            @Override
            public void run() {
                if(isSlaveConnected(gamemodeStr)==true){
                    cancel();
                    switchToArCore();
                }
            }
        };

        timer.schedule(updateTask, 1000, 1000);

    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        if(!ipString.equals("")){
            try{
                QRGEncoder qrEnc = new QRGEncoder(ipString, null, QRGContents.Type.TEXT, (int) (main.getWidth()));
                qrCode = qrEnc.encodeAsBitmap();
                imageView.setImageBitmap(qrCode);
                imageView.setVisibility(View.VISIBLE);
            }catch (Exception e){
                System.out.println("BitMapEncoding : "+e.toString());
            }
        }else{
            TextView ipView = findViewById(R.id.ipView);
            ipView.setText("ERROR: Couldn't detect IP");
            ipView.setVisibility(View.VISIBLE);
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        if(item.getItemId() == android.R.id.home)
        {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }

    private void switchToArCore(){
        Intent switchActivityIntent = new Intent(this,ARCoreActivity.class);
        switchActivityIntent.putExtra("Type","master");
        if(gamemodeStr.equals("TicTacToe")) {
            switchActivityIntent.putExtra("TTTGamePointer", getTTTGame());
        }else{
            switchActivityIntent.putExtra("FourGamePointer",getFourGame());
        }
        switchActivityIntent.putExtra("GameMode",gamemodeStr);
        startActivity(switchActivityIntent);
    }

    public native boolean isSlaveConnected(String gamemode);
    public native long getMaster();
    public native long getTTTGame();
    public native long getFourGame();
}