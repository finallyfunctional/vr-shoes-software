package com.finallyfunctional.vr_shoes.ui;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.ICommunicatorObserver;

import androidx.appcompat.app.AppCompatActivity;

public class ButtonsDiagnosticsActivity extends AppCompatActivity
{
    private ButtonDiagnosticsForShoe diagnosticsForShoe1, diagnosticsForShoe2;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_buttons_diagnostics);
        TextView vrShoe1Header = findViewById(R.id.buttonsDiagnosticVrShoe1Header);
        TextView vrShoe1FrontButtonPressed = findViewById(R.id.buttonsDiagnosticVrShoe1FrontBtnPressedLabel);
        TextView vrShoe1RearButtonPressed = findViewById(R.id.buttonsDiagnosticVrShoe1RearBtnPressedLabel);
        TextView vrShoe2Header = findViewById(R.id.buttonsDiagnosticVrShoe2Header);
        TextView vrShoe2FrontButtonPressed = findViewById(R.id.buttonsDiagnosticVrShoe2FrontBtnPressedLabel);
        TextView vrShoe2RearButtonPressed = findViewById(R.id.buttonsDiagnosticVrShoe2RearBtnPressedLabel);

        Communicator communicator = CommunicationInitializer.getCommunicator();
        diagnosticsForShoe1 = new ButtonDiagnosticsForShoe(this,
                communicator, vrShoe1Header, vrShoe1FrontButtonPressed,
                vrShoe1RearButtonPressed, communicator.getVrShoe1());
        diagnosticsForShoe2 = new ButtonDiagnosticsForShoe(this,
                communicator, vrShoe2Header, vrShoe2FrontButtonPressed,
                vrShoe2RearButtonPressed, communicator.getVrShoe2());
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        diagnosticsForShoe1.destroy();
        diagnosticsForShoe2.destroy();
    }

    public void BackBtnClicked(View view)
    {
        finish();
    }

    private class ButtonDiagnosticsForShoe implements ICommunicatorObserver
    {
        private TextView header, frontButton, rearButton;
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
            this.frontButton = frontButton;
            this.rearButton = rearButton;
            this.vrShoeId = vrShoe.getDeviceId();

            communicator.addObserver(this);
            setLabelsText(vrShoe);
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
                        setLabelsText(vrShoe);
                    }
                });
            }
        }

        private void setLabelsText(VrShoe vrShoe)
        {
            header.setText(vrShoe.getDeviceId());
            frontButton.setText(getString(R.string.front_button_pressed) + " : " + vrShoe.isFrontButtonPressed());
            rearButton.setText(getString(R.string.rear_button_pressed) + " : " + vrShoe.isRearButtonPressed());
        }
    }
}
