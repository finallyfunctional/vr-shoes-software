package com.finallyfunctional.vr_shoes.communication.observers;

import com.finallyfunctional.vr_shoes.VrShoe;

public interface ISensorDataObserver
{
    void sensorDataRead(VrShoe vrShoe);
}
