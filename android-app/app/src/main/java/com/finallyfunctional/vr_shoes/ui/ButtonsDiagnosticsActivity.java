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

import org.w3c.dom.Text;

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

        diagnosticsForShoe1 = new ButtonDiagnosticsForShoe(this,
                CommunicationInitializer.getCommunicator1(), vrShoe1Header,
                vrShoe1FrontButtonPressed, vrShoe1RearButtonPressed);
        diagnosticsForShoe2 = new ButtonDiagnosticsForShoe(this,
                CommunicationInitializer.getCommunicator2(), vrShoe2Header,
                vrShoe2FrontButtonPressed, vrShoe2RearButtonPressed);
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
        private VrShoe vrShoe;
        private Activity activity;
        private Communicator communicator;

        public ButtonDiagnosticsForShoe(Activity activity, Communicator communicator,
                                        TextView header, TextView frontButton, TextView rearButton)
        {
            this.activity = activity;
            this.communicator = communicator;
            this.vrShoe = communicator.getVrShoe();
            this.header = header;
            this.frontButton = frontButton;
            this.rearButton = rearButton;

            communicator.addObserver(this);
            setLabelsText();
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
        public void messageWritten(String message)
        {

        }

        @Override
        public void sensorDataRead(VrShoe vrShoe1)
        {
            activity.runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    setLabelsText();
                }
            });
        }

        private void setLabelsText()
        {
            header.setText(vrShoe.getDeviceId());
            frontButton.setText(getString(R.string.front_button_pressed) + " : " + vrShoe.isFrontButtonPressed());
            rearButton.setText(getString(R.string.rear_button_pressed) + " : " + vrShoe.isRearButtonPressed());
        }

        @Override
        public void distanceFromOriginRead(String deviceId, float forwardDistance, float sidewayDistance)
        {

        }
    }
}
