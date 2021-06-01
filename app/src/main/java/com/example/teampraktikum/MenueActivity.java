package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.content.Intent;

public class MenueActivity extends AppCompatActivity {
    Button toArCore;
    Button toTicTacToe;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menue);

        toArCore = findViewById(R.id.button_arCore);
        toTicTacToe = findViewById(R.id.button_TicTacToe);
        toArCore.setOnClickListener(
                new View.OnClickListener(){
                    @Override
                    public void onClick(View view){
                        switchToArCore();
                    }
                }
        );
        toTicTacToe.setOnClickListener( new View.OnClickListener(){
            @Override
            public void onClick(View view){
                switchToTicTacToe();
            }
        });
    }

    private void switchToArCore(){
        Intent switchActivityIntent = new Intent(this,ARCoreActivity.class);
        startActivity(switchActivityIntent);
    }


    private void switchToTicTacToe(){
        Intent switchActivityIntent = new Intent(this,TicTacToeActivity.class);
        startActivity(switchActivityIntent);
    }
}