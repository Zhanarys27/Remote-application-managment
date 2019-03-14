package com.example.client100;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MouseActivity extends AppCompatActivity {
    String code = "first";
    Button prevTrack, nextTrack, pause, play,next,prev;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mouse);
        play = (Button) findViewById(R.id.buttonPlayer);
        play.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                code = "play";
                byte[] msgBuffer = code.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        });
        pause = (Button) findViewById(R.id.buttonPlayerStop);
        pause.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                code = "stop";
                byte[] msgBuffer = code.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        });

        nextTrack = (Button) findViewById(R.id.buttonPlayerRev);
        nextTrack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                code = "next";
                byte[] msgBuffer = code.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        });
        prevTrack = (Button) findViewById(R.id.buttonBack5);
        prevTrack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                code = "prev";
                byte[] msgBuffer = code.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        })
        next = (Button) findViewById(R.id.buttonPlayerPlay);
        next.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                code = "nextS";
                byte[] msgBuffer = code.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        });
        prev = (Button) findViewById(R.id.buttonPlayerPause);
        prev.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                code = "prevS";
                byte[] msgBuffer = code.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        });


    }
}
