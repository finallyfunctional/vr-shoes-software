package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.ICommunicatorObserver;

public class ButtonsDiagnosticsActivity extends AppCompatActivity implements ICommunicatorObserver
{
    private TextView vrShoe1Header;
    private TextView vrShoe1FrontButtonPressed;
    private TextView vrShoe1RearButtonPressed;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_buttons_diagnostics);
        vrShoe1Header = findViewById(R.id.buttonsDiagnosticVrShoe1Header);
        vrShoe1FrontButtonPressed = findViewById(R.id.buttonsDiagnosticVrShoe1FrontBtnPressedLabel);
        vrShoe1RearButtonPressed = findViewById(R.id.buttonsDiagnosticVrShoe1RearBtnPressedLabel);

        Communicator communicator = CommunicationInitializer.getCommunicator();
        communicator.addObserver(this);
        setLabelsText(communicator.getVrShoe1());
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        CommunicationInitializer.getCommunicator().removeObserver(this);
    }

    public void BackBtnClicked(View view)
    {
        finish();
    }

    @Override
    public void sensorDataRead(final VrShoe vrShoe1)
    {
        this.runOnUiThread(new Runnable()
        {
            public void run()
            {
                setLabelsText(vrShoe1);
            }
        });
    }

    @Override
    public void distanceFromOriginRead(String deviceId, float xDistance, float yDistance)
    {

    }

    private void setLabelsText(VrShoe vrShoe1)
    {
        vrShoe1Header.setText(vrShoe1.getDeviceId());
        vrShoe1FrontButtonPressed.setText(getString(R.string.front_button_pressed) + " : " + vrShoe1.isFrontButtonPressed());
        vrShoe1RearButtonPressed.setText(getString(R.string.rear_button_pressed) + " : " + vrShoe1.isRearButtonPressed());
    }

    @Override
    public void messageRead(String message)
    {

    }

    @Override
    public void messageWritten(String message)
    {

    }
}
