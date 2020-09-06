package com.finallyfunctional.vr_shoes.communication;

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
    private BluetoothAdapter btAdapter;
    private Settings settings;

    public BluetoothInitializer(Settings settings)
    {
        this.settings = settings;
    }

    public void setup() throws
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
