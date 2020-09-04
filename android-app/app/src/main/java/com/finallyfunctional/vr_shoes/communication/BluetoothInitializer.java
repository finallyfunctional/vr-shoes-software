package com.finallyfunctional.vr_shoes.communication;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.Settings;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.ui.PairVrShoesActivity;

import java.io.IOException;
import java.util.Set;

public class BluetoothInitializer extends CommunicationInitializer
{
    private Activity activity;
    private BluetoothAdapter btAdapter;
    private Settings settings;

    private final static int REQUEST_ENABLE_BT = 1;

    public BluetoothInitializer(Activity activity)
    {
        this.activity = activity;
        settings = new Settings(activity);
    }

    public void initialize() throws
            IOException,
            CommunicationNotSupportedException,
            CommunicationNotEnabledException,
            ConfigurationWithOtherActivityNeededException
    {
        btAdapter = BluetoothAdapter.getDefaultAdapter();
        if(btAdapter == null)
        {
            throw new CommunicationNotSupportedException(R.string.bt_not_available);
        }
        else if(!btAdapter.isEnabled())
        {
            throw new CommunicationNotEnabledException(BluetoothAdapter.ACTION_REQUEST_ENABLE, R.string.bt_enabled_msg);
        }
        communicator = pairDevices();
    }

    private Communicator pairDevices() throws IOException, ConfigurationWithOtherActivityNeededException
    {
        String vrShoeId = settings.getPairedVrShoe();
        if(settings.getPairedVrShoe() == "")
        {
            throw new ConfigurationWithOtherActivityNeededException(PairVrShoesActivity.class);
        }
        Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
        for(BluetoothDevice device : pairedDevices)
        {
            if(device.getName().equals(vrShoeId))
            {
                Communicator communicator = new BluetoothSerial(device);
                return communicator;
            }
        }
        throw new ConfigurationWithOtherActivityNeededException(PairVrShoesActivity.class, R.string.stored_vr_shoes_not_found);
    }
}
