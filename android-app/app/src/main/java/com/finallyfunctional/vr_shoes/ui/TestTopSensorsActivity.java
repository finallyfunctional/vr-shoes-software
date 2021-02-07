package com.finallyfunctional.vr_shoes.ui;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.observers.ISensorDataObserver;

import java.util.Timer;
import java.util.TimerTask;

import androidx.appcompat.app.AppCompatActivity;

public class TestTopSensorsActivity extends AppCompatActivity
{
    private Communicator communicator;
    private Timer dataTimer;
    private ButtonDiagnosticsForShoe diagnosticsForShoe1, diagnosticsForShoe2;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test_top_sensors);
        TextView vrShoe1Header = findViewById(R.id.testTopSensorsVrShoe1Header);
        TextView vrShoe1FrontButtonPressed = findViewById(R.id.testTopSensorsVrShoe1FrontSensorPressedLabel);
        TextView vrShoe1RearButtonPressed = findViewById(R.id.testTopSensorsVrShoe1RearSensorPressedLabel);

        TextView vrShoe2Header = findViewById(R.id.testTopSensorsVrShoe2Header);
        TextView vrShoe2FrontButtonPressed = findViewById(R.id.testTopSensorsVrShoe2FrontSensorPressedLabel);
        TextView vrShoe2RearButtonPressed = findViewById(R.id.testTopSensorsVrShoe2RearSensorPressedLabel);

        communicator = CommunicationInitializer.getCommunicator();
        diagnosticsForShoe1 = new ButtonDiagnosticsForShoe(this,
                communicator, vrShoe1Header, vrShoe1FrontButtonPressed,
                vrShoe1RearButtonPressed, communicator.getVrShoe1());
        diagnosticsForShoe2 = new ButtonDiagnosticsForShoe(this,
                communicator, vrShoe2Header, vrShoe2FrontButtonPressed,
                vrShoe2RearButtonPressed, communicator.getVrShoe2());

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
        communicator.getTopSensorValues(communicator.getVrShoe1());
        communicator.getTopSensorValues(communicator.getVrShoe2());
    }

    public void BackBtnClicked(View view)
    {
        finish();
    }

    private class ButtonDiagnosticsForShoe implements ISensorDataObserver
    {
        private TextView header, frontButtonPressed, rearButtonPressed;
        private Activity activity;
        private Communicator communicator;
        private String vrShoeId;

        public ButtonDiagnosticsForShoe(Activity activity, Communicator communicator,
                                        TextView header, TextView frontButton, TextView rearButton,
                                        VrShoe vrShoe)
        {
            this.activity = activity;
            this.communicator = communicator;
            this.header = header;
            this.frontButtonPressed = frontButton;
            this.rearButtonPressed = rearButton;
            this.vrShoeId = vrShoe.getDeviceId();
            communicator.getObservers().addSensorDataObserver(this);
            setBooleanValues(vrShoe);
            header.setText(vrShoe.getDeviceId());
        }

        public void destroy()
        {
            communicator.getObservers().removeSensorDataObserver(this);
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

        private void setBooleanValues(VrShoe vrShoe)
        {
            header.setText(vrShoe.getDeviceId());
            frontButtonPressed.setText(getString(R.string.front_button_pressed) + " : " + vrShoe.isFrontButtonPressed());
            rearButtonPressed.setText(getString(R.string.rear_button_pressed) + " : " + vrShoe.isRearButtonPressed());
        }
    }
}
