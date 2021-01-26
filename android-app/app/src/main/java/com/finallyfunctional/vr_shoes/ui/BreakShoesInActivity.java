package com.finallyfunctional.vr_shoes.ui;

import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;

import java.util.Timer;
import java.util.TimerTask;

import androidx.appcompat.app.AppCompatActivity;

public class BreakShoesInActivity extends AppCompatActivity
{
    private Button startStopBtn, forwardBackwardBtn;
    private TextView countdownLabel;
    private Communicator communicator;
    private Timer sendCommandsTimer;
    private CountDownTimer countDownTimer;
    private boolean forward;
    private boolean sendCommands;

    private static int COUNT_DOWN_TIMER_LENGTH = 600000;
    private static int COUNT_DOWN_TIMER_INTERVAL = 1000;
    private static int RPM = 250;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_break_shoes_in);

        forward = true;
        sendCommands = false;

        startStopBtn = findViewById(R.id.breakShoesInStartStopBtn);
        forwardBackwardBtn = findViewById(R.id.breakShoesInForwardBackwardBtn);
        countdownLabel = findViewById(R.id.breakShoesInCountdownLabel);

        communicator = CommunicationInitializer.getCommunicator();

        sendCommandsTimer = new Timer();
        sendCommandsTimer.scheduleAtFixedRate(new TimerTask()
        {
            @Override
            public void run()
            {
                sendCommands();
            }
        }, 0, 500);

        countDownTimer = new CountDownTimer(COUNT_DOWN_TIMER_LENGTH, COUNT_DOWN_TIMER_INTERVAL)
        {
            @Override
            public void onTick(long millisUntilFinished)
            {
                countDownTimerTick(millisUntilFinished);
            }

            @Override
            public void onFinish()
            {

            }
        };
    }

    public void destroy()
    {
        sendCommandsTimer.cancel();
        countDownTimer.cancel();
    }

    private void countDownTimerTick(long millisUntilFinished)
    {
        countdownLabel.setText(millisUntilFinished / COUNT_DOWN_TIMER_INTERVAL + " s");
    }

    private void sendCommands()
    {
        if(!sendCommands)
        {
            return;
        }
        int rpm = RPM;
        if(!forward)
        {
            rpm *= -1;
        }
        communicator.setRpm(rpm, rpm, communicator.getVrShoe1());
        communicator.setRpm(rpm, rpm, communicator.getVrShoe2());
    }

    public void startStopBtnClicked(View view)
    {
        if(startStopBtn.getText().toString().equals(getString(R.string.start)))
        {
            startStopBtn.setText(R.string.stop);
            countDownTimer.start();
            sendCommands = true;
        }
        else
        {
            startStopBtn.setText(R.string.start);
            countDownTimer.cancel();
            sendCommands = false;
            countdownLabel.setText(COUNT_DOWN_TIMER_LENGTH / COUNT_DOWN_TIMER_INTERVAL + " s");
        }
    }

    public void forwardBackwardBtnClicked(View view)
    {
        if(forwardBackwardBtn.getText().toString().equals(getString(R.string.forward)))
        {
            forwardBackwardBtn.setText(R.string.backward);
            forward = false;
        }
        else
        {
            forwardBackwardBtn.setText(R.string.forward);
            forward = true;
        }
    }

    public void backBtnClicked(View view)
    {
        finish();
    }
}
