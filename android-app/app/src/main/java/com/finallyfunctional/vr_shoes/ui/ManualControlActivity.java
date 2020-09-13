package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;

import java.util.Timer;
import java.util.TimerTask;

public class ManualControlActivity extends AppCompatActivity
{
    private float rpm;
    private Communicator communicator;

    private Button forwardBtn, backwardBtn, leftBtn, rightBtn;
    private Timer timer;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manual_control);
        communicator = CommunicationInitializer.getCommunicator();
        forwardBtn = findViewById(R.id.manualControlForwardBtn);
        backwardBtn = findViewById(R.id.manualControlBackwardBtn);
        leftBtn = findViewById(R.id.manualControlLeftBtn);
        rightBtn = findViewById(R.id.manualControlRightBtn);
        EditText rpmEdit = findViewById(R.id.manualControlRpmEditText);
        rpmEdit.setText("2000");
        rpm = 2000;
        rpmEdit.addTextChangedListener(new TextWatcher()
        {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after)
            {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count)
            {
                String rpmAsStr = s.toString();
                if(!rpmAsStr.equals(""))
                {
                    rpm = Float.parseFloat(s.toString());
                }
            }

            @Override
            public void afterTextChanged(Editable s)
            {

            }
        });
        timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask()
        {
            @Override
            public void run()
            {
                loop();
            }
        }, 0, 50);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        timer.cancel();
    }

    private void loop()
    {
        float forwardRpm = 0;
        float sidewaysRpm = 0;
        if(forwardBtn.isPressed())
        {
            forwardRpm = rpm * -1;
        }
        else if(backwardBtn.isPressed())
        {
            forwardRpm = rpm;
        }
        if(leftBtn.isPressed())
        {
            sidewaysRpm = rpm;
        }
        else if(rightBtn.isPressed())
        {
            sidewaysRpm = rpm * -1;
        }
        communicator.setRpm(forwardRpm, sidewaysRpm);
    }

    public void backBtnClicked(View view)
    {
        finish();
    }
}
