package com.finallyfunctional.vr_shoes.ui;

import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.commands.ShoeConfiguration;
import com.finallyfunctional.vr_shoes.communication.observers.ISensorDataObserver;
import com.finallyfunctional.vr_shoes.communication.observers.IShoeConfigurationsObserver;

import java.util.Timer;
import java.util.TimerTask;

import androidx.appcompat.app.AppCompatActivity;

public class WalkingTestActivity extends AppCompatActivity implements IShoeConfigurationsObserver, ISensorDataObserver
{
    private ToggleButton startWalkingBtn;
    private EditText dutyCycleBoostEdit;
    private EditText speedMultiplierEdit;
    private Spinner vrShoeSpinner;
    private TextView forwardDistanceText, sidewayDistanceText;
    private TextView peakCurrentText, averageCurrentText, ampHoursText, ampHoursChargedText;
    private EditText centerRadiusEdit, centerOffsetEdit;
    private TextView positionText, strideLengthText;
    private CheckBox calculateStrideLengthCheckBox;

    private Communicator communicator;
    private VrShoe vrShoe;
    private Timer dataTimer;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_walking_test);
        dataTimer = new Timer();
        communicator = CommunicationInitializer.getCommunicator();
        communicator.getObservers().addShoeConfigurationObserver(this);
        communicator.getObservers().addSensorDataObserver(this);
        communicator.stopNegatingMotion(communicator.getVrShoe1());
        communicator.stopNegatingMotion(communicator.getVrShoe2());
        communicator.getShoeConfigurations(communicator.getVrShoe1());
        communicator.getShoeConfigurations(communicator.getVrShoe2());
        communicator.resetDistance(communicator.getVrShoe1());
        communicator.resetDistance(communicator.getVrShoe2());
        communicator.readSensorDataFromShoes();
        vrShoe = communicator.getVrShoe1();
        assignUiElements();
        initializeVrShoeSpinner();
        initializeStartWalkingBtn();
        initializeDutyCycleBoost();
        initializeSpeedMultiplier();

        setDistanceText();
        setPowerStatisticsText();
        dataTimer.scheduleAtFixedRate(new TimerTask()
        {
            @Override
            public void run()
            {
                updatePowerStatistics();
            }
        }, 0, 1000 * 10);
    }

    @Override
    public void shoeConfigurationsRead(ShoeConfiguration message, VrShoe vrShoe)
    {
        if(this.vrShoe == vrShoe)
        {
            initializeDutyCycleBoost();
            initializeSpeedMultiplier();
            initializeCenterRadius();
            initializeCenterOffset();
            initializeCalculateStrideLength();
        }
    }

    @Override
    public void sensorDataRead(VrShoe vrShoe)
    {
        if(this.vrShoe == vrShoe)
        {
            setDistanceText();
            setStrideProperties();
        }
    }

    private void assignUiElements()
    {
        startWalkingBtn = findViewById(R.id.walkingTestStartBtn);
        dutyCycleBoostEdit = findViewById(R.id.walkingTestDutyCycleBoostText);
        speedMultiplierEdit = findViewById(R.id.walkingTestSpeedMultiplierText);
        vrShoeSpinner = findViewById(R.id.walkingTestVrShoeSpinner);
        forwardDistanceText = findViewById(R.id.walkingTestForwardDistanceText);
        sidewayDistanceText = findViewById(R.id.walkingTestSidewayDistanceText);

        peakCurrentText = findViewById(R.id.walkingTestPeakCurrentLabel);
        averageCurrentText = findViewById(R.id.walkingTestAverageCurrentLabel);
        ampHoursText = findViewById(R.id.walkingTestAmpHoursLabel);
        ampHoursChargedText = findViewById(R.id.walkingTestAmpHoursChargedLabel);

        strideLengthText = findViewById(R.id.walkingTestStrideLengthLabel);
        centerRadiusEdit = findViewById(R.id.walkingTestCenterRadiusText);
        centerOffsetEdit = findViewById(R.id.walkingTestCenterOffsetText);
        positionText = findViewById(R.id.walkingTestPositionLabel);
        calculateStrideLengthCheckBox = findViewById(R.id.walkingTestCalculateStrideLengthCheckBox);
    }

    private void initializeVrShoeSpinner()
    {
        String[] vrShoesIds = new String[] {communicator.getVrShoe1().getDeviceId(), communicator.getVrShoe2().getDeviceId()};
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, vrShoesIds);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        vrShoeSpinner.setAdapter(adapter);
        vrShoeSpinner.setSelection(0);
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
    }

    private void initializeStartWalkingBtn()
    {
        startWalkingBtn.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                startWalkingBtnClicked();
            }
        });
    }

    private void initializeDutyCycleBoost()
    {
        dutyCycleBoostEdit.setText(String.valueOf(vrShoe.getDutyCycleBoost()));
        dutyCycleBoostEdit.addTextChangedListener(new TextWatcher()
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
    }

    private void initializeSpeedMultiplier()
    {
        speedMultiplierEdit.setText(String.valueOf(vrShoe.getSpeedMultiplier()));
        speedMultiplierEdit.addTextChangedListener(new TextWatcher()
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
                speedMultiplierTextChanged();
            }
        });
    }

    private void initializeCenterRadius()
    {
        centerRadiusEdit.setText(String.valueOf(vrShoe.centerRadius));
        centerRadiusEdit.addTextChangedListener(new TextWatcher()
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
                centerRadiusTextChanged();
            }
        });
    }

    private void initializeCenterOffset()
    {
        centerOffsetEdit.setText(String.valueOf(vrShoe.centerOffset));
        centerOffsetEdit.addTextChangedListener(new TextWatcher()
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
                centerOffsetTextChanged();
            }
        });
    }

    private void initializeCalculateStrideLength()
    {
        calculateStrideLengthCheckBox.setChecked(vrShoe.calculateStrideLength);
        calculateStrideLengthCheckBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
            {
                calculateStrideLengthStateChanged(isChecked);
            }
        });
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        dataTimer.cancel();
        stopWalking();
        communicator.getObservers().removeShoeConfigurationObserver(this);
        communicator.getObservers().removeSensorDataObserver(this);
    }

    public void backBtnClicked(View view)
    {
        stopWalking();
        finish();
    }

    public void resetDistanceBtnClick(View view)
    {
        communicator.resetDistance(communicator.getVrShoe1());
        communicator.resetDistance(communicator.getVrShoe2());
    }

    private void updatePowerStatistics()
    {
        communicator.getPowerStatistics(vrShoe);
        setPowerStatisticsText();
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

    private void setDistanceText()
    {
        forwardDistanceText.setText(getString(R.string.forward_distance) + vrShoe.getForwardDistance());
        sidewayDistanceText.setText(getString(R.string.sideway_distance) + vrShoe.getSidewaysDistance());
    }

    private void setStrideProperties()
    {
        strideLengthText.setText(getString(R.string.stride_length) + vrShoe.strideLength);
        String position;
        if(vrShoe.position == 0)
        {
            position = "behind";
        }
        else if(vrShoe.position == 1)
        {
            position = "center";
        }
        else if(vrShoe.position == 2)
        {
            position = "front";
        }
        else
        {
            position = "invalid " + vrShoe.position;
        }
        positionText.setText(getString(R.string.position) + position);
    }

    private void setPowerStatisticsText()
    {
        peakCurrentText.setText(getString(R.string.peak_current) + vrShoe.getForwardPeakCurrent());
        averageCurrentText.setText(getString(R.string.average_current) + vrShoe.getForwardAverageCurrent());
        ampHoursText.setText(getString(R.string.amp_hours) + vrShoe.getForwardAmpHours());
        ampHoursChargedText.setText(getString(R.string.amp_hours_charged) + vrShoe.getForwardAmpCharged());
    }

    private void startWalkingBtnClicked()
    {
        if(startWalkingBtn.isChecked())
        {
            communicator.startNegatingMotion(communicator.getVrShoe1());
            communicator.startNegatingMotion(communicator.getVrShoe2());
        }
        else
        {
            stopWalking();
        }
    }

    private void stopWalking()
    {
        communicator.stopNegatingMotion(communicator.getVrShoe1());
        communicator.stopNegatingMotion(communicator.getVrShoe2());
    }

    private void dutyCycleBoostTextChanged()
    {
        if(dutyCycleBoostEdit.getText().toString().equals(""))
        {
            return;
        }
        float boost = Float.parseFloat(dutyCycleBoostEdit.getText().toString());
        VrShoe vrShoe1 = communicator.getVrShoe1();
        VrShoe vrShoe2 = communicator.getVrShoe2();
        ShoeConfiguration vrShoe1Command = new ShoeConfiguration(vrShoe1);
        ShoeConfiguration vrShoe2Command = new ShoeConfiguration(vrShoe2);
        vrShoe1Command.dcb = boost;
        vrShoe2Command.dcb = boost;
        communicator.configureShoe(vrShoe1, vrShoe1Command);
        communicator.configureShoe(vrShoe2, vrShoe2Command);
        vrShoe1.setDutyCycleBoost(boost);
        vrShoe2.setDutyCycleBoost(boost);
    }

    private void speedMultiplierTextChanged()
    {
        if(speedMultiplierEdit.getText().toString().equals(""))
        {
            return;
        }
        float multiplier = Float.parseFloat(speedMultiplierEdit.getText().toString());
        VrShoe vrShoe1 = communicator.getVrShoe1();
        VrShoe vrShoe2 = communicator.getVrShoe2();
        ShoeConfiguration vrShoe1Command = new ShoeConfiguration(vrShoe1);
        ShoeConfiguration vrShoe2Command = new ShoeConfiguration(vrShoe2);
        vrShoe1Command.spm = multiplier;
        vrShoe2Command.spm = multiplier;
        communicator.configureShoe(vrShoe1, vrShoe1Command);
        communicator.configureShoe(vrShoe2, vrShoe2Command);
        vrShoe1.setSpeedMultiplier(multiplier);
        vrShoe2.setSpeedMultiplier(multiplier);
    }

    private void centerRadiusTextChanged()
    {
        if(centerRadiusEdit.getText().toString().equals(""))
        {
            return;
        }
        float centerRadius = Float.parseFloat(centerRadiusEdit.getText().toString());
        VrShoe vrShoe1 = communicator.getVrShoe1();
        VrShoe vrShoe2 = communicator.getVrShoe2();
        ShoeConfiguration vrShoe1Command = new ShoeConfiguration(vrShoe1);
        ShoeConfiguration vrShoe2Command = new ShoeConfiguration(vrShoe2);
        vrShoe1Command.cr = centerRadius;
        vrShoe2Command.cr = centerRadius;
        communicator.configureShoe(vrShoe1, vrShoe1Command);
        communicator.configureShoe(vrShoe2, vrShoe2Command);
        vrShoe1.centerRadius = centerRadius;
        vrShoe2.centerRadius = centerRadius;
    }

    private void centerOffsetTextChanged()
    {
        if(centerOffsetEdit.getText().toString().equals("") ||
           centerOffsetEdit.getText().toString().equals("-") ||
           centerOffsetEdit.getText().toString().equals("."))
        {
            return;
        }
        float centerOffset = Float.parseFloat(centerOffsetEdit.getText().toString());
        VrShoe vrShoe1 = communicator.getVrShoe1();
        VrShoe vrShoe2 = communicator.getVrShoe2();
        ShoeConfiguration vrShoe1Command = new ShoeConfiguration(vrShoe1);
        ShoeConfiguration vrShoe2Command = new ShoeConfiguration(vrShoe2);
        vrShoe1Command.co = centerOffset;
        vrShoe2Command.co = centerOffset;
        communicator.configureShoe(vrShoe1, vrShoe1Command);
        communicator.configureShoe(vrShoe2, vrShoe2Command);
        vrShoe1.centerOffset = centerOffset;
        vrShoe2.centerOffset = centerOffset;
    }

    private void calculateStrideLengthStateChanged(boolean calculate)
    {
        VrShoe vrShoe1 = communicator.getVrShoe1();
        VrShoe vrShoe2 = communicator.getVrShoe2();
        ShoeConfiguration vrShoe1Command = new ShoeConfiguration(vrShoe1);
        ShoeConfiguration vrShoe2Command = new ShoeConfiguration(vrShoe2);
        vrShoe1Command.csl = calculate;
        vrShoe2Command.csl = calculate;
        communicator.configureShoe(vrShoe1, vrShoe1Command);
        communicator.configureShoe(vrShoe2, vrShoe2Command);
        vrShoe1.calculateStrideLength = calculate;
        vrShoe2.calculateStrideLength = calculate;
    }
}
