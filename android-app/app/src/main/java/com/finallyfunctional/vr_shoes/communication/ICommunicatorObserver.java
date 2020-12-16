package com.finallyfunctional.vr_shoes.communication;

import com.finallyfunctional.vr_shoes.VrShoe;

public interface ICommunicatorObserver
{
    void messageRead(String message);
    void messageWritten(VrShoe vrShoe, String message);
    void sensorDataRead(VrShoe vrShoe1);
    void buttonValuesRead(VrShoe vrShoe, int frontButtonValue, int rearButtonValue, int maxDifference);
}
