package com.finallyfunctional.vr_shoes.ui;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.ICommunicatorObserver;

import java.util.Timer;
import java.util.TimerTask;

import androidx.appcompat.app.AppCompatActivity;

public class ConfigureButtonsActivity extends AppCompatActivity
{
    private Communicator communicator;
    private Timer dataTimer;
    private ButtonDiagnosticsForShoe diagnosticsForShoe1, diagnosticsForShoe2;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_configure_buttons);
        TextView vrShoe1Header = findViewById(R.id.configureButtonsVrShoe1Header);
        TextView vrShoe1FrontButtonPressed = findViewById(R.id.configureButtonsVrShoe1FrontBtnPressedLabel);
        TextView vrShoe1RearButtonPressed = findViewById(R.id.configureButtonsVrShoe1RearBtnPressedLabel);
        TextView vrShoe1FrontButtonValue = findViewById(R.id.configButtonsVrShoe1FrontBtnValueLabel);
        TextView vrShoe1RearButtonValue = findViewById(R.id.configButtonsVrShoe1RearBtnValueLabel);
        EditText vrShoe1MaxDifferenceValue = findViewById(R.id.configButtonsVrShoe1MaxDifferenceValue);

        TextView vrShoe2Header = findViewById(R.id.configureButtonsVrShoe2Header);
        TextView vrShoe2FrontButtonPressed = findViewById(R.id.configureButtonsVrShoe2FrontBtnPressedLabel);
        TextView vrShoe2RearButtonPressed = findViewById(R.id.configureButtonsVrShoe2RearBtnPressedLabel);
        TextView vrShoe2FrontButtonValue = findViewById(R.id.configButtonsVrShoe2FrontBtnValueLabel);
        TextView vrShoe2RearButtonValue = findViewById(R.id.configButtonsVrShoe2RearBtnValueLabel);
        EditText vrShoe2MaxDifferenceValue = findViewById(R.id.configButtonsVrShoe2MaxDifferenceValue);

        communicator = CommunicationInitializer.getCommunicator();
        diagnosticsForShoe1 = new ButtonDiagnosticsForShoe(this,
                communicator, vrShoe1Header, vrShoe1FrontButtonPressed,
                vrShoe1RearButtonPressed, vrShoe1FrontButtonValue, vrShoe1RearButtonValue,
                vrShoe1MaxDifferenceValue, communicator.getVrShoe1());
        diagnosticsForShoe2 = new ButtonDiagnosticsForShoe(this,
                communicator, vrShoe2Header, vrShoe2FrontButtonPressed,
                vrShoe2RearButtonPressed, vrShoe2FrontButtonValue,
                vrShoe2RearButtonValue, vrShoe2MaxDifferenceValue, communicator.getVrShoe2());

        dataTimer = new Timer();
        dataTimer.scheduleAtFixedRate(new TimerTask()
        {
            @Override
            public void run()
            {
                updateButtonValues();
            }
        }, 0, 100);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        diagnosticsForShoe1.destroy();
        diagnosticsForShoe2.destroy();
        dataTimer.cancel();
    }

    private void updateButtonValues()
    {
        communicator.getButtonValues(communicator.getVrShoe1());
        communicator.getButtonValues(communicator.getVrShoe2());
    }

    public void BackBtnClicked(View view)
    {
        finish();
    }

    public void CalibrateBtnClicked(View view)
    {
        int vrShoe1MaxDiff = Integer.parseInt(diagnosticsForShoe1.maxDifference.getText().toString());
        communicator.configureButtons(communicator.getVrShoe1(), vrShoe1MaxDiff);

        int vrShoe2MaxDiff = Integer.parseInt(diagnosticsForShoe2.maxDifference.getText().toString());
        communicator.configureButtons(communicator.getVrShoe2(), vrShoe2MaxDiff);
    }

    private class ButtonDiagnosticsForShoe implements ICommunicatorObserver
    {
        private TextView header, frontButtonPressed, rearButtonPressed, frontButtonValue, rearButtonValue;
        private EditText maxDifference;
        private Activity activity;
        private Communicator communicator;
        private String vrShoeId;

        public ButtonDiagnosticsForShoe(Activity activity, Communicator communicator,
                                        TextView header, TextView frontButton, TextView rearButton,
                                        TextView frontButtonValue, TextView rearButtonValue,
                                        EditText maxDifference, VrShoe vrShoe)
        {
            this.activity = activity;
            this.communicator = communicator;
            this.header = header;
            this.frontButtonPressed = frontButton;
            this.rearButtonPressed = rearButton;
            this.frontButtonValue = frontButtonValue;
            this.rearButtonValue = rearButtonValue;
            this.maxDifference = maxDifference;
            this.vrShoeId = vrShoe.getDeviceId();

            communicator.addObserver(this);
            setBooleanValues(vrShoe);
            setIntValues(vrShoe, 0, 0, 0);
        }

        public void destroy()
        {
            communicator.removeObserver(this);
        }

        @Override
        public void messageRead(String message)
        {

        }

        @Override
        public void messageWritten(VrShoe vrShoe, String message)
        {

        }

        @Override
        public void sensorDataRead(final VrShoe vrShoe)
        {
            if(vrShoe.getDeviceId().equals(vrShoeId))
            {
                activity.runOnUiThread(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        setBooleanValues(vrShoe);
                    }
                });
            }
        }

        @Override
        public void buttonValuesRead(final VrShoe vrShoe, final int frontButtonValue,
                                     final int rearButtonValue, final int maxDifference)
        {
            if(vrShoe.getDeviceId().equals(vrShoeId))
            {
                activity.runOnUiThread(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        setIntValues(vrShoe, frontButtonValue, rearButtonValue, maxDifference);
                    }
                });
            }
        }

        private void setBooleanValues(VrShoe vrShoe)
        {
            header.setText(vrShoe.getDeviceId());
            frontButtonPressed.setText(getString(R.string.front_button_pressed) + " : " + vrShoe.isFrontButtonPressed());
            rearButtonPressed.setText(getString(R.string.rear_button_pressed) + " : " + vrShoe.isRearButtonPressed());
        }

        private void setIntValues(VrShoe vrShoe, int frontButtonValue, int rearButtonValue,
                                  int maxDifference)
        {
            header.setText(vrShoe.getDeviceId());
            this.frontButtonValue.setText(getString(R.string.front_button_value) + " : " + frontButtonValue);
            this.rearButtonValue.setText(getString(R.string.rear_button_value) + " : " + rearButtonValue);
            this.maxDifference.setText(getString(R.string.button_max_difference) + " : " + maxDifference);
        }
    }
}
