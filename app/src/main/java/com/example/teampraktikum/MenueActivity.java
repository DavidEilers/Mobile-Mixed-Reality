package com.example.teampraktikum;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.content.Intent;

public class MenueActivity extends AppCompatActivity {
    Button toArCore;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menue);

        toArCore = findViewById(R.id.button_arCore);
        toArCore.setOnClickListener(
                new View.OnClickListener(){
                    @Override
                    public void onClick(View view){
                        switchToArCore();
                    }
                }
        );
    }

    private void switchToArCore(){
        Intent switchActivityIntent = new Intent(this,ARCoreActivity.class);
        startActivity(switchActivityIntent);
    }
}