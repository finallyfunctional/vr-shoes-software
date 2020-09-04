package com.finallyfunctional.vr_shoes.ui;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;

import com.finallyfunctional.vr_shoes.BluetoothSerial;
import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.Settings;

import java.io.IOException;
import java.util.Set;

public class MainActivity extends AppCompatActivity
{
    private BluetoothAdapter btAdapter;
    private Settings settings;
    private BluetoothSerial btSerial;

    private final static int REQUEST_ENABLE_BT = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        settings = new Settings(this);
    }

    @Override
    public void onResume()
    {
        super.onResume();
        initializeBluetooth();
        //sendTestMessage();
    }

    private void sendTestMessage()
    {
        Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
        String vrShoeId = settings.getPairedVrShoe();
        BluetoothDevice btDevice = null;
        for(BluetoothDevice device : pairedDevices)
        {
            if(device.getName().equals(vrShoeId))
            {
                btDevice = device;
            }
        }
        try
        {
            btSerial = new BluetoothSerial(btDevice);
            btSerial.startReading();
            btSerial.writeMessage("{\"command\":\"ping\"}");
        }
        catch (IOException ex)
        {
            ex.printStackTrace();
        }
    }

    private void initializeBluetooth()
    {
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        if(btAdapter == null)
        {
            DialogUtility.showOkayDialog(this, getString(R.string.bt_not_available), new Runnable()
            {
                @Override
                public void run()
                {
                    MainActivity.this.finish();
                    System.exit(0);
                }
            });
            return;
        }
        else if(!btAdapter.isEnabled())
        {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            Toast.makeText(getApplicationContext(), getString(R.string.bt_enabled_msg), Toast.LENGTH_LONG).show();
        }
        pairDevices();
    }

    private void pairDevices()
    {
        String vrShoeId = settings.getPairedVrShoe();
        if(settings.getPairedVrShoe() == "")
        {
            Intent intent = new Intent(this, PairVrShoesActivity.class);
            startActivity(intent);
            return;
        }
        Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
        for(BluetoothDevice device : pairedDevices)
        {
            if(device.getName().equals(vrShoeId))
            {
                Toast.makeText(getApplicationContext(),"Paired and ready to go!",Toast.LENGTH_LONG).show();
                return;
            }
        }
        DialogUtility.showOkayDialog(this, getString(R.string.stored_vr_shoes_not_found), new Runnable()
        {
            @Override
            public void run()
            {
                Intent intent = new Intent(MainActivity.this, PairVrShoesActivity.class);
                startActivity(intent);
            }
        });
    }
}
