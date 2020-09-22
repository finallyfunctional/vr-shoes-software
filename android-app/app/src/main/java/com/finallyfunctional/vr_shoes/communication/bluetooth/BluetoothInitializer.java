package com.finallyfunctional.vr_shoes.communication.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import com.finallyfunctional.vr_shoes.R;
import com.finallyfunctional.vr_shoes.Settings;
import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.CommunicationInitializer;
import com.finallyfunctional.vr_shoes.communication.CommunicationModes;
import com.finallyfunctional.vr_shoes.communication.Communicator;
import com.finallyfunctional.vr_shoes.communication.commands.SetCommunicationMode;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.ui.PairVrShoesActivity;

import java.io.IOException;
import java.util.Set;

public class BluetoothInitializer extends CommunicationInitializer
{
    private BluetoothAdapter btAdapter;
    private BluetoothDevice shoe1, shoe2;

    public BluetoothInitializer(Settings settings)
    {
        super(settings);
    }

    public void setup(String shoe1Id, String shoe2Id) throws
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
        getShoesAsBtDevices(shoe1Id, shoe2Id);
//         master-slave mode doesn't work
//        if(settings.getCommunicationMode().equals(CommunicationModes.BLUETOOTH_MASTER_SLAVE))
//        {
//            throw new UnsupportedOperationException();
//            String masterShoeId = settings.getMasterVrShoe();
//            communicator = setupShoeAsMaster(masterShoeId);
//        }
        communicator = new BluetoothSerialSlaveSlave(shoe1, shoe2);
    }

    private void getShoesAsBtDevices(String shoe1Id, String shoe2Id) throws ConfigurationWithOtherActivityNeededException
    {
        Set<BluetoothDevice> pairedDevices = btAdapter.getBondedDevices();
        for(BluetoothDevice device : pairedDevices)
        {
            if(device.getName().equals(shoe1Id))
            {
                shoe1 = device;
            }
            else if(device.getName().equals(shoe2Id))
            {
                shoe2 = device;
            }
        }
        if(shoe1 == null || shoe2 == null)
        {
            throw new ConfigurationWithOtherActivityNeededException(PairVrShoesActivity.class, R.string.stored_vr_shoes_not_found);
        }
    }

    private Communicator setupShoeAsMaster(String masterShoeId) throws IOException
    {
        BluetoothDevice masterShoe, slaveShoe;
        if(shoe1.getName().equals(masterShoeId))
        {
            masterShoe = shoe1;
            slaveShoe = shoe2;
        }
        else
        {
            masterShoe = shoe2;
            slaveShoe = shoe1;
        }
        BluetoothSerial setupSerial = new BluetoothSerialSlaveSlave(masterShoe, slaveShoe);
        VrShoe masterVrShoe = setupSerial.getVrShoe1();
        if(setupSerial.getVrShoe2().getDeviceId().equals(masterShoe.getName()))
        {
            masterVrShoe = setupSerial.getVrShoe2();
        }

        setupSerial.start();
        SetCommunicationMode command = new SetCommunicationMode();
        command.d = masterShoeId;
        command.m = SetCommunicationMode.BT_MASTER_MODE;
        command.oi = slaveShoe.getName();
        setupSerial.setCommunicationMode(masterVrShoe, command);
        setupSerial.stop();
        settings.saveMasterVrShoe(masterShoeId);
        settings.saveCommunicationMode(CommunicationModes.BLUETOOTH_MASTER_SLAVE);
        return new BluetoothSerialMasterSlave(slaveShoe, masterShoeId);
    }
}
