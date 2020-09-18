package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.ICommunicatorObserver;

public class MovementDiagnosticsActivity extends AppCompatActivity implements ICommunicatorObserver
{
    private TextView vrShoe1Header;
    private TextView vrShoe1ForwardDistance;
    private TextView vrShoe1SidewayDistance;
    private TextView vrShoe1ForwardSpeed;
    private TextView vrShoe1SidewaySpeed;
    private Thread updateDistanceLoop;
    private boolean updateDistance;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_movement_diagnostics);
        vrShoe1Header = findViewById(R.id.movementDiagnosticVrShoe1Header);
        vrShoe1ForwardDistance = findViewById(R.id.movementDiagnosticVrShoe1ForwardDistanceLabel);
        vrShoe1SidewayDistance = findViewById(R.id.movementDiagnosticVrShoe1SidewaysDistanceLabel);
        vrShoe1ForwardSpeed = findViewById(R.id.movementDiagnosticVrShoe1ForwardSpeedLabel);
        vrShoe1SidewaySpeed = findViewById(R.id.movementDiagnosticVrShoe1SidewaySpeedLabel);

        final Communicator communicator = CommunicationInitializer.getCommunicator1();
        communicator.addObserver(this);
        VrShoe vrShoe1 = communicator.getVrShoe();
        setDeviceIdText(vrShoe1.getDeviceId());
        setDistanceText(0, 0);
        setSpeedText(vrShoe1.getForwardSpeed(), vrShoe1.getSidewaySpeed());

        communicator.resetOrigin();
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
                        communicator.readDistanceFromOrigin();
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
        CommunicationInitializer.getCommunicator1().removeObserver(this);
    }

    private void setDeviceIdText(String deviceId)
    {
        vrShoe1Header.setText(deviceId);
    }

    private void setDistanceText(float forwardDistance, float sidewayDistance)
    {
        vrShoe1ForwardDistance.setText(getString(R.string.forward_distance) + forwardDistance);
        vrShoe1SidewayDistance.setText(getString(R.string.sideway_distance) + sidewayDistance);
    }

    private void setSpeedText(float forwardSpeed, float sidewaySpeed)
    {
        vrShoe1ForwardSpeed.setText(getString(R.string.forward_speed) + forwardSpeed);
        vrShoe1SidewaySpeed.setText(getString(R.string.sideway_speed) + sidewaySpeed);
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
    public void sensorDataRead(final VrShoe vrShoe1)
    {
        this.runOnUiThread(new Runnable()
        {
            public void run()
            {
                setDeviceIdText(vrShoe1.getDeviceId());
                setSpeedText(vrShoe1.getForwardSpeed(), vrShoe1.getSidewaySpeed());
            }
        });
    }

    @Override
    public void distanceFromOriginRead(String deviceId, final float forwardDistance, final float sidewayDistance)
    {
        this.runOnUiThread(new Runnable()
        {
            public void run()
            {
                setDistanceText(forwardDistance, sidewayDistance);
            }
        });
    }

    public void resetOriginBtnClicked(View view)
    {
        CommunicationInitializer.getCommunicator1().resetOrigin();
    }

    public void backBtnClicked(View view)
    {
        finish();
    }
}
