package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.content.Intent;

public class MenueActivity extends AppCompatActivity {
    Button toArCore;
    Button toTicTacToe;
    Button toJoinGame;
    Button toHostGame;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menue);

        toArCore = findViewById(R.id.button_arCore);
        toTicTacToe = findViewById(R.id.button_TicTacToe);
        toJoinGame = findViewById(R.id.button_joinGame);
        toHostGame = findViewById(R.id.button_hostGame);
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

        toHostGame.setOnClickListener( new View.OnClickListener(){
            @Override
            public void onClick(View view){
                switchToHostGame();
            }
        });

        toJoinGame.setOnClickListener( new View.OnClickListener(){
            @Override
            public void onClick(View view){
                switchToJoinGame();
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

    private void switchToHostGame(){
        Intent switchActivityIntent = new Intent(this,HostGameActivity.class);
        startActivity(switchActivityIntent);
    }

    private void switchToJoinGame(){
        Intent switchActivityIntent = new Intent(this,JoinGameActivity.class);
        startActivity(switchActivityIntent);
    }
}