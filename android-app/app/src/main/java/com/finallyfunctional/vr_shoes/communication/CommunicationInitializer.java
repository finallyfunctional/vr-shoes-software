package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;
import com.finallyfunctional.vr_shoes.logging.CommunicatorLogger;
import com.finallyfunctional.vr_shoes.logging.monitor.VrShoesAggregateLogger;

import java.io.IOException;

public abstract class CommunicationInitializer
{
    protected static Communicator communicator;

    protected abstract void setup() throws
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
        setup();
        communicator.addObserver(new CommunicatorLogger(VrShoesAggregateLogger.getLogger()));
        communicator.start();
        communicator.readSensorDataFromShoes();
    }

    public static Communicator getCommunicator()
    {
        return communicator;
    }
}
