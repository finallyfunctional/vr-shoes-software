package com.finallyfunctional.vr_shoes.communication;

import java.io.IOException;

public abstract class Communicator
{
    public abstract void startReading();
    public abstract void stopReading();

    protected abstract String readNextMessage();
    protected abstract void writeMessage(String message) throws IOException;
}
