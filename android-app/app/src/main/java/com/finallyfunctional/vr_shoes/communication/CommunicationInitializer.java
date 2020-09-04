package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotEnabledException;
import com.finallyfunctional.vr_shoes.communication.exceptions.CommunicationNotSupportedException;
import com.finallyfunctional.vr_shoes.communication.exceptions.ConfigurationWithOtherActivityNeededException;

import java.io.IOException;

public abstract class CommunicationInitializer
{
    protected static Communicator communicator;

    public abstract void initialize() throws
            IOException,
            CommunicationNotSupportedException,
            CommunicationNotEnabledException,
            ConfigurationWithOtherActivityNeededException;

    public static Communicator getCommunicator()
    {
        return communicator;
    }
}
