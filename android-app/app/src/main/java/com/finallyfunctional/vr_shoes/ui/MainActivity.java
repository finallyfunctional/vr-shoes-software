package com.finallyfunctional.vr_shoes.ui;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.StoredSettings;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.bluetooth.BluetoothInitializer;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.logging.monitor.VrShoesAggregateLogger;

import java.io.IOException;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity
{
    private CommunicationInitializer communicationInitializer;
    private ProgressBar progressBar;
    private ToggleButton startWalkingBtn;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        progressBar = findViewById(R.id.mainProgressBar);
        startWalkingBtn = findViewById(R.id.mainStartWalkingBtn);
        startWalkingBtn.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                startWalkingBtnClicked();
            }
        });
        communicationInitializer = new BluetoothInitializer(new StoredSettings(this));
        VrShoesAggregateLogger.initialize();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        try
        {
            ProgressBarUtility.showProgressBar(progressBar, this);
            communicationInitializer.initialize();
            ProgressBarUtility.hideProgressBar(progressBar, MainActivity.this);
        }
        catch (IOException ex)
        {
            ex.printStackTrace();
            DialogUtility.showOkayDialog(this, getString(R.string.communication_error), new Runnable()
            {
                @Override
                public void run()
                {
                    finish();
                    System.exit(0);
                }
            });
        }
        catch (CommunicationNotSupportedException ex)
        {
            DialogUtility.showOkayDialog(this, getString(ex.dialogMessageId), new Runnable()
            {
                @Override
                public void run()
                {
                    finish();
                    System.exit(0);
                }
            });
        }
        catch(CommunicationNotEnabledException ex)
        {
            Intent enableBtIntent = new Intent(ex.requestToEnable);
            startActivityForResult(enableBtIntent, 1);
            Toast.makeText(getApplicationContext(), getString(ex.dialogMessageId), Toast.LENGTH_LONG).show();
        }
        catch(final ConfigurationWithOtherActivityNeededException ex)
        {
            if(ex.showDialogFirst)
            {
                DialogUtility.showOkayDialog(this, getString(ex.dialogMessageId), new Runnable()
                {
                    @Override
                    public void run()
                    {
                        Intent intent = new Intent(MainActivity.this, ex.otherActivity);
                        startActivity(intent);
                    }
                });
            }
            else
            {
                Intent intent = new Intent(this, ex.otherActivity);
                startActivity(intent);
            }
        }
    }

    public void settingsBtnClicked(View view)
    {
        Intent intent = new Intent(MainActivity.this, SettingsActivity.class);
        startActivity(intent);
    }

    public void diagnosticsBtnClicked(View view)
    {
        Intent intent = new Intent(MainActivity.this, DiagnosticsActivity.class);
        startActivity(intent);
    }

    private void startWalkingBtnClicked()
    {
        Communicator communicator = CommunicationInitializer.getCommunicator();
        VrShoe vrShoe1 = communicator.getVrShoe1();
        VrShoe vrShoe2 = communicator.getVrShoe2();
        if(startWalkingBtn.isChecked())
        {
            communicator.startAlgorithm(vrShoe1);
            communicator.startAlgorithm(vrShoe2);
        }
        else
        {
            communicator.stopAlgorithm(vrShoe1);
            communicator.stopAlgorithm(vrShoe2);
        }
    }

}
