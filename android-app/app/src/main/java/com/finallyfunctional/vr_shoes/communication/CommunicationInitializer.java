package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.Settings;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.logging.CommunicatorLogger;
import com.finallyfunctional.vr_shoes.logging.monitor.VrShoesAggregateLogger;
import com.finallyfunctional.vr_shoes.ui.PairVrShoesActivity;

import java.io.IOException;

public abstract class CommunicationInitializer
{
    protected static Communicator communicator1;
    protected static Communicator communicator2;
    protected Settings settings;

    protected CommunicationInitializer(Settings settings)
    {
        this.settings = settings;
    }

    protected abstract Communicator setup(String deviceId) throws
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
        String deviceId1 = settings.getPairedVrShoe1();
        String deviceId2 = settings.getPairedVrShoe2();
        communicator1 = initializeCommunicator(deviceId1);
        communicator2 = initializeCommunicator(deviceId2);
    }

    private Communicator initializeCommunicator(String deviceId)
            throws ConfigurationWithOtherActivityNeededException,
            CommunicationNotEnabledException,
            IOException,
            CommunicationNotSupportedException
    {
        if(deviceId == null || deviceId.equals(""))
        {
            throw new ConfigurationWithOtherActivityNeededException(PairVrShoesActivity.class);
        }
        Communicator communicator = setup(deviceId);
        communicator.addObserver(new CommunicatorLogger(VrShoesAggregateLogger.getLogger()));
        communicator.start();
        communicator.readSensorDataFromShoes();
        return communicator;
    }

    public static Communicator getCommunicator1()
    {
        return communicator1;
    }

    public static Communicator getCommunicator2() {return communicator2;}
}
