package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ProgressBar;
import android.widget.Toast;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.Settings;
import com.finallyfunctional.vr_shoes.communication.bluetooth.BluetoothInitializer;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.logging.monitor.VrShoesAggregateLogger;

import java.io.IOException;

public class MainActivity extends AppCompatActivity
{
    private CommunicationInitializer communicationInitializer;
    private ProgressBar progressBar;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        progressBar = findViewById(R.id.mainProgressBar);
        communicationInitializer = new BluetoothInitializer(new Settings(this));
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
            //TODO going straight to diagnostics now. Later I'll implement the main activity.
            Intent intent = new Intent(MainActivity.this, DiagnosticsActivity.class);
            startActivity(intent);
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
}
