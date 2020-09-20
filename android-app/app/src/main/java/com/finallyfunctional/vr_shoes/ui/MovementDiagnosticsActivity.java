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

import java.util.Timer;
import java.util.TimerTask;

public class MovementDiagnosticsActivity extends AppCompatActivity
{
    private Timer timer;
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

        Communicator communicator = CommunicationInitializer.getCommunicator();
        vrShoe1Diagnostics = new MovementDiagnosticsForShoe(vrShoe1Header, vrShoe1ForwardDistance,
                vrShoe1SidewayDistance, vrShoe1ForwardSpeed, vrShoe1SidewaySpeed,
                communicator, this, communicator.getVrShoe1());
        vrShoe2Diagnostics = new MovementDiagnosticsForShoe(vrShoe2Header, vrShoe2ForwardDistance,
                vrShoe2SidewayDistance, vrShoe2ForwardSpeed, vrShoe2SidewaySpeed,
                communicator, this, communicator.getVrShoe2());

        timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask()
        {
            @Override
            public void run()
            {
                vrShoe1Diagnostics.readDistanceFromOrigin();
                vrShoe2Diagnostics.readDistanceFromOrigin();
            }
        }, 0, 100);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        vrShoe1Diagnostics.destroy();
        vrShoe2Diagnostics.destroy();
        timer.cancel();
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
        private VrShoe vrShoe;

        public MovementDiagnosticsForShoe(TextView header, TextView forwardDistance,
                                          TextView sidewayDistance, TextView forwardSpeed,
                                          TextView sidewaySpeed, Communicator communicator,
                                          Activity activity, VrShoe vrShoe)
        {
            this.header = header;
            this.forwardDistance = forwardDistance;
            this.sidewayDistance = sidewayDistance;
            this.forwardSpeed = forwardSpeed;
            this.sidewaySpeed = sidewaySpeed;
            this.communicator = communicator;
            this.activity = activity;
            this.vrShoe = vrShoe;

            communicator.addObserver(this);

            setDeviceIdText();
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
            communicator.readDistanceFromOrigin(vrShoe);
        }

        private void resetOrigin()
        {
            communicator.resetOrigin(vrShoe);
        }

        private void setDeviceIdText()
        {
            header.setText(vrShoe.getDeviceId());
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
        public void messageWritten(VrShoe vrShoe, String message)
        {

        }

        @Override
        public void sensorDataRead(final VrShoe vrShoe)
        {
            if(vrShoe == this.vrShoe)
            {
                activity.runOnUiThread(new Runnable()
                {
                    public void run()
                    {
                        setDeviceIdText();
                        setSpeedText(vrShoe.getForwardSpeed(), vrShoe.getSidewaySpeed());
                    }
                });
            }
        }

        @Override
        public void distanceFromOriginRead(VrShoe vrShoe,  final float forwardDistance, final float sidewayDistance)
        {
            if(vrShoe == this.vrShoe)
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
}
