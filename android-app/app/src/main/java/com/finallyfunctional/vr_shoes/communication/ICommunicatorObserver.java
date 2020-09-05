package com.finallyfunctional.vr_shoes.communication;

public interface ICommunicatorObserver
{
    void messageRead(String message);
    void messageWritten(String message);
}
