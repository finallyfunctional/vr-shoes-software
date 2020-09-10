package com.finallyfunctional.vr_shoes.logging;

import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.ICommunicatorObserver;

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
    public void messageWritten(String message)
    {
        logger.verbose(CommunicatorLogger.class.toString(), "Message Written: " + message);
    }

    @Override
    public void sensorDataRead(VrShoe vrShoe1)
    {

    }

    @Override
    public void distanceFromOriginRead(String deviceId, float forwardDistance, float sidewayDistance)
    {

    }
}
