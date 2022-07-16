package com.finallyfunctional.vr_shoes.communication.observers;

import com.finallyfunctional.vr_shoes.VrShoe;

public interface ICommunicatorObserver
{
    void messageRead(String message);
    void messageWritten(VrShoe vrShoe, String message);
}
