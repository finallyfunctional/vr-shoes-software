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

import androidx.appcompat.app.AppCompatActivity;

public class MovementDiagnosticsActivity extends AppCompatActivity
{
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
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
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

    private class MovementDiagnosticsForShoe implements ISensorDataObserver
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

            communicator.getObservers().addSensorDataObserver(this);

            setDeviceIdText();
            setDistanceText(0, 0);
            setSpeedText(0, 0);
            resetOrigin();
        }

        public void destroy()
        {
            communicator.getObservers().removeSensorDataObserver(this);
        }

        private void resetOrigin()
        {
            communicator.resetDistance(vrShoe);
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
        public void sensorDataRead(final VrShoe vrShoe)
        {
            if(vrShoe == this.vrShoe)
            {
                activity.runOnUiThread(new Runnable()
                {
                    public void run()
                    {
                        setDeviceIdText();
                        setSpeedText(vrShoe.getForwardSpeed(), vrShoe.getSidewaysSpeed());
                        setDistanceText(vrShoe.getForwardDistance(), vrShoe.getSidewaysDistance());
                    }
                });
            }
        }
    }
}
