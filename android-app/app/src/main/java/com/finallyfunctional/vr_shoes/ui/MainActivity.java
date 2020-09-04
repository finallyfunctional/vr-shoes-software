package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.BluetoothInitializer;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;

import java.io.IOException;

public class MainActivity extends AppCompatActivity
{
    private CommunicationInitializer communicationInitializer;

    public static final String COMMUNICATOR_KEY = "communicator";

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        communicationInitializer = new BluetoothInitializer(this);
    }

    @Override
    public void onResume()
    {
        super.onResume();
        try
        {
            communicationInitializer.initialize();

            //TODO going straight to diagnostics now. Later I'll implement the main activity.
            Intent intent = new Intent(this, DiagnosticsActivity.class);
            startActivity(intent);
        }
        catch (IOException ex)
        {
            ex.printStackTrace();
            //TODO handle error
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
