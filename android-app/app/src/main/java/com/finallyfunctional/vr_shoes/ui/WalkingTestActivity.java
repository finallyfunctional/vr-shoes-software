package com.finallyfunctional.vr_shoes.ui;

import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.StoredSettings;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.ICommunicatorObserver;

import java.util.Timer;
import java.util.TimerTask;

import androidx.appcompat.app.AppCompatActivity;

public class WalkingTestActivity extends AppCompatActivity implements ICommunicatorObserver
{
    private ToggleButton startWalkingBtn;
    private EditText dutyCycleBoostText;
    private Spinner vrShoeSpinner;
    private TextView forwardDutyCycleText, sidewayDutyCycleText;
    private TextView forwardDistanceText, sidewayDistanceText;

    private Communicator communicator;
    private VrShoe vrShoe;
    private StoredSettings settings;
    private Timer extraSensorDataTimer;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_walking_test);
        startWalkingBtn = findViewById(R.id.walkingTestStartBtn);
        dutyCycleBoostText = findViewById(R.id.walkingTestDutyCycleBoostText);
        vrShoeSpinner = findViewById(R.id.walkingTestVrShoeSpinner);
        forwardDutyCycleText = findViewById(R.id.walkingTestCurrentForwardDutyCycleText);
        sidewayDutyCycleText = findViewById(R.id.walkingTestCurrentSidewayDutyCycleText);
        forwardDistanceText = findViewById(R.id.walkingTestForwardDistanceText);
        sidewayDistanceText = findViewById(R.id.walkingTestSidewayDistanceText);
        extraSensorDataTimer = new Timer();
        communicator = CommunicationInitializer.getCommunicator();
        settings = new StoredSettings(this);

        startWalkingBtn.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                startWalkingBtnClicked();
            }
        });

        dutyCycleBoostText.setText(String.valueOf(settings.getDutyCycleBoost()));
        dutyCycleBoostText.addTextChangedListener(new TextWatcher()
        {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after)
            {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count)
            {

            }

            @Override
            public void afterTextChanged(Editable s)
            {
                dutyCycleBoostTextChanged();
            }
        });

        String[] vrShoesIds = new String[] {communicator.getVrShoe1().getDeviceId(), communicator.getVrShoe2().getDeviceId()};
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, vrShoesIds);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        vrShoeSpinner.setAdapter(adapter);
        vrShoeSpinner.setSelection(0);
        vrShoe = communicator.getVrShoe1();
        vrShoeSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
        {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id)
            {
                vrShoeSelected((String)parent.getItemAtPosition(position));
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent)
            {

            }
        });

        setDutyCycleText();
        setDistanceText(0, 0);

        extraSensorDataTimer.scheduleAtFixedRate(new TimerTask()
        {
            @Override
            public void run()
            {
                update();
            }
        }, 0, 100);

        communicator.addObserver(this);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        extraSensorDataTimer.cancel();
        stopAlgorithm();
        communicator.removeObserver(this);
    }

    public void backBtnClicked(View view)
    {
        stopAlgorithm();
        finish();
    }

    public void resetOriginBtnClicked(View view)
    {
        communicator.resetOrigin(communicator.getVrShoe1());
        communicator.resetOrigin(communicator.getVrShoe2());
    }

    private void update()
    {
        communicator.getExtraSensorData(vrShoe);
        communicator.readDistanceFromOrigin(vrShoe);
        setDutyCycleText();
    }

    private void vrShoeSelected(String shoeId)
    {
        VrShoe vrShoe1 = communicator.getVrShoe1();
        VrShoe vrShoe2 = communicator.getVrShoe2();
        if(shoeId.equals(vrShoe1.getDeviceId()))
        {
            vrShoe = vrShoe1;
        }
        else
        {
            vrShoe = vrShoe2;
        }
    }

    private void setDutyCycleText()
    {
        forwardDutyCycleText.setText(getString(R.string.current_forward_duty_cycle) + vrShoe.getForwardDutyCycle());
        sidewayDutyCycleText.setText(getString(R.string.current_sideway_duty_cycle) + vrShoe.getSidewayDutyCycle());
    }

    private void setDistanceText(float forwardDistance, float sidewayDistance)
    {
        forwardDistanceText.setText(getString(R.string.forward_distance) + forwardDistance);
        sidewayDistanceText.setText(getString(R.string.sideway_distance) + sidewayDistance);
    }

    private void startWalkingBtnClicked()
    {
        if(startWalkingBtn.isChecked())
        {
            communicator.startAlgorithm(communicator.getVrShoe1());
            communicator.startAlgorithm(communicator.getVrShoe2());
        }
        else
        {
            stopAlgorithm();
        }
    }

    private void stopAlgorithm()
    {
        communicator.stopAlgorithm(communicator.getVrShoe1());
        communicator.stopAlgorithm(communicator.getVrShoe2());
    }

    private void dutyCycleBoostTextChanged()
    {
        float boost = Float.parseFloat(dutyCycleBoostText.getText().toString());
        settings.saveDutyCycleBoost(boost);
        communicator.setDutyCycleBoost(communicator.getVrShoe1(), boost);
        communicator.setDutyCycleBoost(communicator.getVrShoe2(), boost);
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
    public void sensorDataRead(VrShoe vrShoe1)
    {

    }

    @Override
    public void distanceFromOriginRead(VrShoe vrShoe, final float forwardDistance, final float sidewayDistance)
    {
        if(vrShoe == this.vrShoe)
        {
            this.runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    setDistanceText(forwardDistance, sidewayDistance);
                }
            });
        }
    }
}
