package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.ICommunicatorObserver;

public class MovementDiagnosticsActivity extends AppCompatActivity
{
    private Thread updateDistanceLoop;
    private boolean updateDistance;
    private MovementDiagnosticsForShoe vrShoe1Diagnostics, vrShoe2Diagnostics;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_movement_diagnostics);
        TextView vrShoe1Header = findViewById(R.id.movementDiagnosticVrShoe1Header);
        TextView vrShoe1ForwardDistance = findViewById(R.id.movementDiagnosticVrShoe1ForwardDistanceLabel);
        TextView vrShoe1SidewayDistance = findViewById(R.id.movementDiagnosticVrShoe1SidewaysDistanceLabel);
        TextView vrShoe1ForwardSpeed = findViewById(R.id.movementDiagnosticVrShoe1ForwardSpeedLabel);
        TextView vrShoe1SidewaySpeed = findViewById(R.id.movementDiagnosticVrShoe1SidewaySpeedLabel);
        TextView vrShoe2Header = findViewById(R.id.movementDiagnosticVrShoe2Header);
        TextView vrShoe2ForwardDistance = findViewById(R.id.movementDiagnosticVrShoe2ForwardDistanceLabel);
        TextView vrShoe2SidewayDistance = findViewById(R.id.movementDiagnosticVrShoe2SidewaysDistanceLabel);
        TextView vrShoe2ForwardSpeed = findViewById(R.id.movementDiagnosticVrShoe2ForwardSpeedLabel);
        TextView vrShoe2SidewaySpeed = findViewById(R.id.movementDiagnosticVrShoe2SidewaySpeedLabel);

        vrShoe1Diagnostics = new MovementDiagnosticsForShoe(vrShoe1Header, vrShoe1ForwardDistance,
                vrShoe1SidewayDistance, vrShoe1ForwardSpeed, vrShoe1SidewaySpeed,
                CommunicationInitializer.getCommunicator1(), this);
        vrShoe2Diagnostics = new MovementDiagnosticsForShoe(vrShoe2Header, vrShoe2ForwardDistance,
                vrShoe2SidewayDistance, vrShoe2ForwardSpeed, vrShoe2SidewaySpeed,
                CommunicationInitializer.getCommunicator2(), this);

        updateDistance = true;
        updateDistanceLoop = new Thread(new Runnable()
        {
            @Override
            public void run()
            {
                while(updateDistance)
                {
                    try
                    {
                        vrShoe1Diagnostics.readDistanceFromOrigin();
                        vrShoe2Diagnostics.readDistanceFromOrigin();
                        Thread.sleep(100);
                    }
                    catch (InterruptedException e)
                    {
                        e.printStackTrace();
                        //TODO handle error
                    }
                }
            }
        });
        updateDistanceLoop.start();
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        updateDistance = false;
        vrShoe1Diagnostics.destroy();
        vrShoe2Diagnostics.destroy();
    }

    public void resetOriginVrShoe1BtnClicked(View view)
    {
        vrShoe1Diagnostics.resetOrigin();
    }

    public void resetOriginVrShoe2BtnClicked(View view)
    {
        vrShoe2Diagnostics.resetOrigin();
    }

    public void backBtnClicked(View view)
    {
        finish();
    }

    private class MovementDiagnosticsForShoe implements ICommunicatorObserver
    {
        private TextView header;
        private TextView forwardDistance;
        private TextView sidewayDistance;
        private TextView forwardSpeed;
        private TextView sidewaySpeed;
        private Communicator communicator;
        private Activity activity;

        public MovementDiagnosticsForShoe(TextView header, TextView forwardDistance,
                                          TextView sidewayDistance, TextView forwardSpeed,
                                          TextView sidewaySpeed, Communicator communicator,
                                          Activity activity)
        {
            this.header = header;
            this.forwardDistance = forwardDistance;
            this.sidewayDistance = sidewayDistance;
            this.forwardSpeed = forwardSpeed;
            this.sidewaySpeed = sidewaySpeed;
            this.communicator = communicator;
            this.activity = activity;

            communicator.addObserver(this);

            setDeviceIdText(communicator.getVrShoe().getDeviceId());
            setDistanceText(0, 0);
            setSpeedText(0, 0);
            resetOrigin();
        }

        public void destroy()
        {
            communicator.removeObserver(this);
        }

        public void readDistanceFromOrigin()
        {
            communicator.readDistanceFromOrigin();
        }

        private void resetOrigin()
        {
            communicator.resetOrigin();
        }

        private void setDeviceIdText(String deviceId)
        {
            header.setText(deviceId);
        }

        private void setDistanceText(float forwardDistance, float sidewayDistance)
        {
            this.forwardDistance.setText(getString(R.string.forward_distance) + forwardDistance);
            this.sidewayDistance.setText(getString(R.string.sideway_distance) + sidewayDistance);
        }

        private void setSpeedText(float forwardSpeed, float sidewaySpeed)
        {
            this.forwardSpeed.setText(getString(R.string.forward_speed) + forwardSpeed);
            this.sidewaySpeed.setText(getString(R.string.sideway_speed) + sidewaySpeed);
        }

        @Override
        public void messageRead(String message)
        {

        }

        @Override
        public void messageWritten(String message)
        {

        }

        @Override
        public void sensorDataRead(final VrShoe vrShoe)
        {
            activity.runOnUiThread(new Runnable()
            {
                public void run()
                {
                    setDeviceIdText(vrShoe.getDeviceId());
                    setSpeedText(vrShoe.getForwardSpeed(), vrShoe.getSidewaySpeed());
                }
            });
        }

        @Override
        public void distanceFromOriginRead(String deviceId, final float forwardDistance, final float sidewayDistance)
        {
            activity.runOnUiThread(new Runnable()
            {
                public void run()
                {
                    setDistanceText(forwardDistance, sidewayDistance);
                }
            });
        }
    }
}
