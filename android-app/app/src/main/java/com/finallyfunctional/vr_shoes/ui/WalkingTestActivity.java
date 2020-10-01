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
import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.utils.ColorTemplate;

import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;

import androidx.appcompat.app.AppCompatActivity;

public class WalkingTestActivity extends AppCompatActivity
{
    private ToggleButton startWalkingBtn;
    private EditText dutyCycleBoostEdit;
    private EditText speedMultiplierEdit;
    private Spinner vrShoeSpinner;
    private TextView forwardDutyCycleText, sidewayDutyCycleText;
    private TextView forwardDistanceText, sidewayDistanceText;
    private TextView peakCurrentText, averageCurrentText, ampHoursText, ampHoursChargedText;
    private EditText kpEdit, kiEdit, kdEdit;

    private BarChart speedChart;
    private BarEntry actualSpeedEntry;
    private BarEntry desiredSpeedEntry;

    private Communicator communicator;
    private VrShoe vrShoe;
    private StoredSettings settings;
    private Timer dataTimer;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_walking_test);
        dataTimer = new Timer();
        communicator = CommunicationInitializer.getCommunicator();
        vrShoe = communicator.getVrShoe1();
        settings = new StoredSettings(this);
        assignUiElements();
        initializeVrShoeSpinner();
        initializeStartWalkingBtn();
        initializeDutyCycleBoost();
        initializeSpeedMultiplier();
        initializePidParameters();
        initializeSpeedChart();

        setDistanceText();
        setPowerStatisticsText();
        dataTimer.scheduleAtFixedRate(new TimerTask()
        {
            @Override
            public void run()
            {
                updateExtraSensorData();
            }
        }, 0, 100);
        dataTimer.scheduleAtFixedRate(new TimerTask()
        {
            @Override
            public void run()
            {
                updatePowerStatistics();
            }
        }, 0, 1000 * 10);
    }

    private void assignUiElements()
    {
        startWalkingBtn = findViewById(R.id.walkingTestStartBtn);
        dutyCycleBoostEdit = findViewById(R.id.walkingTestDutyCycleBoostText);
        speedMultiplierEdit = findViewById(R.id.walkingTestSpeedMultiplierText);
        vrShoeSpinner = findViewById(R.id.walkingTestVrShoeSpinner);
        forwardDutyCycleText = findViewById(R.id.walkingTestCurrentForwardDutyCycleText);
        sidewayDutyCycleText = findViewById(R.id.walkingTestCurrentSidewayDutyCycleText);
        forwardDistanceText = findViewById(R.id.walkingTestForwardDistanceText);
        sidewayDistanceText = findViewById(R.id.walkingTestSidewayDistanceText);
        peakCurrentText = findViewById(R.id.walkingTestPeakCurrentLabel);
        averageCurrentText = findViewById(R.id.walkingTestAverageCurrentLabel);
        ampHoursText = findViewById(R.id.walkingTestAmpHoursLabel);
        ampHoursChargedText = findViewById(R.id.walkingTestAmpHoursChargedLabel);
        kpEdit = findViewById(R.id.walkingTestKpEditText);
        kiEdit = findViewById(R.id.walkingTestKiEditText);
        kdEdit = findViewById(R.id.walkingTestKdEditText);
        speedChart = findViewById(R.id.walkingTestSpeedChart);
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
        dutyCycleBoostEdit.setText(String.valueOf(settings.getDutyCycleBoost()));
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
        setDutyCycleText();
    }

    private void initializeSpeedMultiplier()
    {
        speedMultiplierEdit.setText(String.valueOf(settings.getSpeedMultiplier()));
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

    private void initializePidParameters()
    {
        kpEdit.setText(String.valueOf(settings.getPidKp()));
        kiEdit.setText(String.valueOf(settings.getPidKi()));
        kdEdit.setText(String.valueOf(settings.getPidKd()));

        TextWatcher watcher = new TextWatcher()
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
                pidParametersChanged();
            }
        };

        kpEdit.addTextChangedListener(watcher);
        kiEdit.addTextChangedListener(watcher);
        kdEdit.addTextChangedListener(watcher);
    }

    private void initializeSpeedChart()
    {
        ArrayList<BarEntry> entries = new ArrayList<>();
        actualSpeedEntry = new BarEntry(vrShoe.getForwardSpeed(), 0);
        desiredSpeedEntry = new BarEntry(vrShoe.getForwardDesiredSpeed(), 1);
        entries.add(actualSpeedEntry);
        entries.add(desiredSpeedEntry);
        BarDataSet set = new BarDataSet(entries, getString(R.string.speeds));
        set.setColors(ColorTemplate.COLORFUL_COLORS);

        ArrayList<String> labels = new ArrayList<>();
        labels.add(getString(R.string.actual_speed));
        labels.add(getString(R.string.desired_speed));

        BarData speedChartData = new BarData(labels, set);
        speedChart.setData(speedChartData);
    }

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        dataTimer.cancel();
        stopAlgorithm();
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

    private void updateExtraSensorData()
    {
        communicator.getExtraSensorData(vrShoe);
        setDutyCycleText();
        setDistanceText();
        updateSpeedChart();
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

    private void setDutyCycleText()
    {
        forwardDutyCycleText.setText(getString(R.string.current_forward_duty_cycle) + vrShoe.getForwardDutyCycle());
        sidewayDutyCycleText.setText(getString(R.string.current_sideway_duty_cycle) + vrShoe.getSidewayDutyCycle());
    }

    private void setDistanceText()
    {
        forwardDistanceText.setText(getString(R.string.forward_distance) + vrShoe.getForwardDistanceFromOrigin());
        sidewayDistanceText.setText(getString(R.string.sideway_distance) + vrShoe.getSidewayDistanceFromOrigin());
    }

    private void setPowerStatisticsText()
    {
        peakCurrentText.setText(getString(R.string.peak_current) + vrShoe.getForwardPeakCurrent());
        averageCurrentText.setText(getString(R.string.average_current) + vrShoe.getForwardAverageCurrent());
        ampHoursText.setText(getString(R.string.amp_hours) + vrShoe.getForwardAmpHours());
        ampHoursChargedText.setText(getString(R.string.amp_hours_charged) + vrShoe.getForwardAmpCharged());
    }

    private void updateSpeedChart()
    {
        actualSpeedEntry.setVal(vrShoe.getForwardSpeed());
        desiredSpeedEntry.setVal(vrShoe.getForwardDesiredSpeed());
        speedChart.notifyDataSetChanged();
        speedChart.invalidate();
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
        if(dutyCycleBoostEdit.getText().toString().equals(""))
        {
            return;
        }
        float boost = Float.parseFloat(dutyCycleBoostEdit.getText().toString());
        communicator.setDutyCycleBoost(communicator.getVrShoe1(), boost);
        communicator.setDutyCycleBoost(communicator.getVrShoe2(), boost);
        settings.saveDutyCycleBoost(boost);
    }

    private void speedMultiplierTextChanged()
    {
        if(speedMultiplierEdit.getText().toString().equals(""))
        {
            return;
        }
        float multiplier = Float.parseFloat(speedMultiplierEdit.getText().toString());
        communicator.setSpeedMultiplier(communicator.getVrShoe1(), multiplier);
        communicator.setSpeedMultiplier(communicator.getVrShoe2(), multiplier);
        settings.saveSpeedMultiplier(multiplier);
    }

    private void pidParametersChanged()
    {
        if(kpEdit.getText().toString().equals("") ||
           kiEdit.getText().toString().equals("") ||
           kdEdit.getText().toString().equals(""))
        {
            return;
        }
        float kp = Float.parseFloat(kpEdit.getText().toString());
        float ki = Float.parseFloat(kiEdit.getText().toString());
        float kd = Float.parseFloat(kdEdit.getText().toString());
        communicator.tunePidLoop(communicator.getVrShoe1(), kp, ki, kd);
        communicator.tunePidLoop(communicator.getVrShoe2(), kp, ki, kd);
        settings.savePidParameters(kp, ki, kd);
    }
}
