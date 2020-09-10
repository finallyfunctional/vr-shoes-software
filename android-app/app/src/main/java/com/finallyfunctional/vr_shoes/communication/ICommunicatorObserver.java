package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.VrShoe;

public interface ICommunicatorObserver
{
    void messageRead(String message);
    void messageWritten(String message);
    void sensorDataRead(VrShoe vrShoe1);
    void distanceFromOriginRead(String deviceId, float forwardDistance, float sidewayDistance);
}
