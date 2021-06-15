package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.text.format.Formatter;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

import java.net.InetAddress;
import java.util.Timer;
import java.util.TimerTask;

public class TicTacToeActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("full-lib");
    }

    Button startButton;
    Button[][] board = new Button[3][3];

    EditText ipEdit;

    Timer timer;

    boolean myTurn = false;
    boolean gameRunning = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_tictactoe);



        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try{
                    WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
                    String ipAddress = Formatter.formatIpAddress(wm.getConnectionInfo().getIpAddress());
                    TextView a = (TextView) findViewById(R.id.textViewTitle);
                    a.setText("Your IP-Adress is: "+ipAddress);
                }catch(Exception e){
                    System.out.println("Exception : "+e.toString());
                }
            }
        });

        Button btn = (Button) findViewById(R.id.QRCodeButton);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                IntentIntegrator integrator = new IntentIntegrator(TicTacToeActivity.this);
                integrator.setDesiredBarcodeFormats(IntentIntegrator.ALL_CODE_TYPES);
                integrator.setPrompt("Scan");
                integrator.setCameraId(0);
                integrator.setBeepEnabled(false);
                integrator.setBarcodeImageEnabled(false);
                integrator.initiateScan();
            }
        });

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_WIFI_STATE)
                != PackageManager.PERMISSION_GRANTED) {
            System.out.println("INTERNET PERMISSION NOT GRANTED");
            ActivityCompat.requestPermissions(
                    this, new String[]{Manifest.permission.ACCESS_WIFI_STATE}, 0);
            return;
        }
        thread.start();


        startButton = (Button) findViewById(R.id.start_button);
        ipEdit = (EditText) findViewById(R.id.editTextIP);

        board[0][0] = (Button) findViewById(R.id.button_1_1);
        board[1][0] = (Button) findViewById(R.id.button_2_1);
        board[2][0] = (Button) findViewById(R.id.button_3_1);
        board[0][1] = (Button) findViewById(R.id.button_1_2);
        board[1][1] = (Button) findViewById(R.id.button_2_2);
        board[2][1] = (Button) findViewById(R.id.button_3_2);
        board[0][2] = (Button) findViewById(R.id.button_1_3);
        board[1][2] = (Button) findViewById(R.id.button_2_3);
        board[2][2] = (Button) findViewById(R.id.button_3_3);

        resetBoard();
        setBoardEnabled(false);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j].setOnClickListener(getCustomClickListener(i, j));
            }
        }

        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //startButtonPressed();
                startButton.setEnabled(false);
                switchToArCore(ipEdit.getText().toString());
            }
        });

        timer = new Timer("Update");

        TimerTask updateTask = new TimerTask() {
            @Override
            public void run() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        myTurn = getMyTurn();
                        gameRunning = getGameRunning();

                        startButton.setEnabled(!gameRunning);

                        if (gameRunning) {
                            for (int i = 0; i < 3; i++) {
                                for (int j = 0; j < 3; j++) {
                                    switch (getStatusAt(i, j)) {
                                        case 0: //FIELD_EMPTY
                                            board[i][j].setText("");
                                            board[i][j].setEnabled(myTurn);
                                            break;
                                        case 1: //PLAYER O
                                            board[i][j].setText("O");
                                            board[i][j].setEnabled(false);
                                            break;
                                        case 2: //PLAYER 1
                                            board[i][j].setText("X");
                                            board[i][j].setEnabled(false);
                                            break;
                                    }
                                }
                            }
                        }
                    }
                });
            }
        };

        timer.schedule(updateTask, 1000, 1000);
    }

    public void resetBoard() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j].setText("");
            }
        }
    }

    public void setBoardEnabled(boolean enabled) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j].setEnabled(enabled);
            }
        }
    }

    public View.OnClickListener getCustomClickListener(final int x, final int y) {
        return new View.OnClickListener() {
            final int pos_x = x;
            final int pos_y = y;

            @Override
            public void onClick(View view) {
                boardPressedAt(pos_x, pos_y);
                setBoardEnabled(false);
            }
        };
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        IntentResult result = IntentIntegrator.parseActivityResult(requestCode, resultCode, data);
        if(result != null) {
            if(result.getContents() == null) {
                System.out.println("Cancelled scan");

            } else {
                System.out.println("Scanned");

                EditText ipInput = (EditText) findViewById(R.id.editTextIP);
                ipInput.setText(result.getContents());
                //Toast.makeText(this, "Scanned: " + result.getContents(), Toast.LENGTH_LONG).show();
            }
        } else {
            // This is important, otherwise the result will not be passed to the fragment
            super.onActivityResult(requestCode, resultCode, data);
        }
    }

    private void switchToArCore(String ipAddress){
        Intent switchActivityIntent = new Intent(this,ARCoreActivity.class);
        switchActivityIntent.putExtra("ipAddress",ipAddress);
        startActivity(switchActivityIntent);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void boardPressedAt(int x, int y);

    public native void startButtonPressed(String ip);

    public native int getStatusAt(int x, int y);

    public native boolean getMyTurn();

    public native boolean getGameRunning();
}
