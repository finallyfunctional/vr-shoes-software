package com.finallyfunctional.vr_shoes.logging;

import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.observers.ICommunicatorObserver;

public class CommunicatorLogger implements ICommunicatorObserver
{
    private IVrShoesAppLogger logger;

    public CommunicatorLogger(IVrShoesAppLogger logger)
    {
        this.logger = logger;
    }

    @Override
    public void messageRead(String message)
    {
        logger.verbose(CommunicatorLogger.class.toString(), "Message Read: " + message);
    }

    @Override
    public void messageWritten(VrShoe vrShoe, String message)
    {
        logger.verbose(CommunicatorLogger.class.toString(), "Message Written: " + message);
    }
}
