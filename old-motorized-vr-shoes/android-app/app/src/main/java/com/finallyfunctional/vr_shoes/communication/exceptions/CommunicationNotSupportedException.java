package com.finallyfunctional.vr_shoes.communication.exceptions;

public class CommunicationNotSupportedException extends Exception
{
    public final int dialogMessageId;

    public CommunicationNotSupportedException(int dialogMessageId)
    {
        this.dialogMessageId = dialogMessageId;
    }
}
