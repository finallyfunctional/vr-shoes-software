package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;

import java.util.Timer;
import java.util.TimerTask;

public class ManualControlActivity extends AppCompatActivity
{
    private ManualControlForShoe vrShoe1Control, vrShoe2Control;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_manual_control);
        Button forwardVrShoe1Btn = findViewById(R.id.manualControlVrShoe1ForwardBtn);
        Button backwardVrShoe1Btn = findViewById(R.id.manualControlVrShoe1BackwardBtn);
        Button leftVrShoe1Btn = findViewById(R.id.manualControlVrShoe1LeftBtn);
        Button rightVrShoe1Btn = findViewById(R.id.manualControlVrShoe1RightBtn);
        EditText rpmVrShoe1Edit = findViewById(R.id.manualControlVrShoe1RpmEditText);
        TextView deviceIdVrShoe1Text = findViewById(R.id.manualControlVrShoe1DeviceIdLabel);
        Button forwardVrShoe2Btn = findViewById(R.id.manualControlVrShoe2ForwardBtn);
        Button backwardVrShoe2Btn = findViewById(R.id.manualControlVrShoe2BackwardBtn);
        Button leftVrShoe2Btn = findViewById(R.id.manualControlVrShoe2LeftBtn);
        Button rightVrShoe2Btn = findViewById(R.id.manualControlVrShoe2RightBtn);
        EditText rpmVrShoe2Edit = findViewById(R.id.manualControlVrShoe2RpmEditText);
        TextView deviceIdVrShoe2Text = findViewById(R.id.manualControlVrShoe2DeviceIdLabel);

        vrShoe1Control = new ManualControlForShoe(CommunicationInitializer.getCommunicator1(),
                forwardVrShoe1Btn, backwardVrShoe1Btn, rightVrShoe1Btn, leftVrShoe1Btn, rpmVrShoe1Edit,
                deviceIdVrShoe1Text);
        vrShoe2Control = new ManualControlForShoe(CommunicationInitializer.getCommunicator2(),
                forwardVrShoe2Btn, backwardVrShoe2Btn, rightVrShoe2Btn, leftVrShoe2Btn, rpmVrShoe2Edit,
                deviceIdVrShoe2Text);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        vrShoe1Control.destroy();
        vrShoe2Control.destroy();
    }

    public void backBtnClicked(View view)
    {
        finish();
    }

    private class ManualControlForShoe
    {
        private Communicator communicator;
        private Button forwardBtn, backwardBtn, leftBtn, rightBtn;

        private float rpm;
        private Timer timer;

        public ManualControlForShoe(Communicator communicator, Button forwardBtn,
                                    Button backwardBtn, Button leftBtn, Button rightBtn,
                                    EditText rpmEdit, TextView deviceIdText)
        {
            this.communicator = communicator;
            this.forwardBtn = forwardBtn;
            this.backwardBtn = backwardBtn;
            this.leftBtn = leftBtn;
            this.rightBtn = rightBtn;

            rpmEdit.setText("2000");
            rpm = 2000;
            deviceIdText.setText(communicator.getVrShoe().getDeviceId());

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

        public void destroy()
        {
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
    }
}
