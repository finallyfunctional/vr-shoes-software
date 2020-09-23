package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.StoredSettings;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.logging.CommunicatorLogger;
import com.finallyfunctional.vr_shoes.logging.monitor.VrShoesAggregateLogger;
import com.finallyfunctional.vr_shoes.ui.PairVrShoesActivity;

import java.io.IOException;

public abstract class CommunicationInitializer
{
    protected static Communicator communicator;
    protected StoredSettings settings;

    protected CommunicationInitializer(StoredSettings settings)
    {
        this.settings = settings;
    }

    protected abstract void setup(String deviceId1, String deviceId2) throws
            IOException,
            CommunicationNotSupportedException,
            CommunicationNotEnabledException,
            ConfigurationWithOtherActivityNeededException;

    public void initialize()
            throws
            IOException,
            CommunicationNotSupportedException,
            CommunicationNotEnabledException,
            ConfigurationWithOtherActivityNeededException
    {
        if(communicator != null)
        {
            return; //already initialized
        }
        String deviceId1 = settings.getPairedVrShoe1();
        String deviceId2 = settings.getPairedVrShoe2();
        initializeCommunicator(deviceId1, deviceId2);
    }

    private void initializeCommunicator(String shoe1Id, String shoe2Id)
            throws ConfigurationWithOtherActivityNeededException,
            CommunicationNotEnabledException,
            IOException,
            CommunicationNotSupportedException
    {
        if(shoe1Id == null || shoe1Id.equals("") || shoe2Id == null || shoe2Id.equals(""))
        {
            throw new ConfigurationWithOtherActivityNeededException(PairVrShoesActivity.class);
        }
        setup(shoe1Id, shoe2Id);
        communicator.addObserver(new CommunicatorLogger(VrShoesAggregateLogger.getLogger()));
        communicator.start();
        communicator.sendOtherShoeId(communicator.getVrShoe1(), communicator.getVrShoe2());
        communicator.sendOtherShoeId(communicator.getVrShoe2(), communicator.getVrShoe1());
        communicator.readSensorDataFromShoes();
        communicator.getShoeSide(communicator.getVrShoe1());
        communicator.getShoeSide(communicator.getVrShoe2());
    }

    public static Communicator getCommunicator()
    {
        return communicator;
    }
}
