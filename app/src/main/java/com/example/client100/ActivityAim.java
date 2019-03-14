package com.example.client100;

import android.app.Activity;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class ActivityAim extends AppCompatActivity implements View.OnTouchListener {
    TextView tv;
    float x=0;
    float y=0;
    public float cordX = 0,cordY = 0,cordXCur,cordYCur;
    public String out = "first";
    String sDown;
    String sMove;
    String sUp;
    Button buttonLeftClick,buttonWheelUp,buttonWheelDown,buttonRightClick;

    public View.OnClickListener listenerMouseRightClick,listenerMouseLeftClick,listenerWheelUp,
            listenerWheelDown,listenerMyComp,listenerJoystick,listenerNoGba,listenerAimp;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_aim);
        buttonLeftClick = (Button) findViewById(R.id.buttonLeftClick);
        buttonRightClick = (Button) findViewById(R.id.buttonRightClick);
        buttonWheelUp = (Button) findViewById(R.id.buttonWheelUp);
        buttonWheelDown = (Button) findViewById(R.id.buttonWheelDown);

        listenerMouseRightClick = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                out = "mouseRightClick";
                byte[] msgBuffer = out.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        };

        buttonRightClick.setOnClickListener(listenerMouseRightClick);

        listenerMouseLeftClick = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                out = "mouseLeftClick";
                byte[] msgBuffer = out.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        };

        buttonLeftClick.setOnClickListener(listenerMouseLeftClick);

        listenerWheelUp = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                out = "mouseWheelUp";
                byte[] msgBuffer = out.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        };

        buttonWheelUp.setOnClickListener(listenerWheelUp);

        listenerWheelDown = new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                out = "mouseWheelDown";
                byte[] msgBuffer = out.getBytes();
                MainActivity.thisDevices.write(msgBuffer);
            }
        };

        buttonWheelDown.setOnClickListener(listenerWheelDown);

        tv=(TextView)findViewById(R.id.viewTouchpad);
        tv.setOnTouchListener(this);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {

        switch(event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                break;
            case MotionEvent.ACTION_UP:
                return false;
            case MotionEvent.ACTION_MOVE:
                cordXCur = event.getX();
                cordYCur = event.getY();

                if(cordX == cordXCur && cordY == cordYCur) {
                    break;
                }

                if(cordXCur == cordX && cordYCur < cordY) {
                    out = "mouseUp";
                }

                else if (cordXCur == cordX && cordYCur > cordY) {
                    out = "mouseDown";
                }

                else if (cordXCur > cordX && cordYCur == cordY) {
                    out = "mouseRight";
                }

                else if (cordXCur < cordX && cordYCur == cordY) {
                    out = "mouseLeft";
                }

                else if (cordXCur > cordX && cordYCur > cordY) {
                    out = "mouseDownRight";
                    //
                }

                else if (cordXCur > cordX && cordYCur < cordY) {
                    out = "mouseUpRigth";
                    //
                }

                else if (cordXCur < cordX && cordYCur > cordY) {
                    out = "mouseDownLeft";
                    //
                }
                else if (cordXCur < cordX && cordYCur < cordY) {
                    out = "mouseUpLeft";
                    //
                }
                byte[] msgBuffer = out.getBytes();
                MainActivity.thisDevices.write(msgBuffer);


                cordX = cordXCur;
                cordY = cordYCur;
                break;
            default:
                break;
        }


        tv.setText(out);
        return true;


    }
}
