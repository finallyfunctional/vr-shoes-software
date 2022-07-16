package com.finallyfunctional.vr_shoes.communication.exceptions;

public class CommunicationNotEnabledException extends Exception
{
    public final String requestToEnable;
    public final int dialogMessageId;

    public CommunicationNotEnabledException(String requestToEnable, int dialogMessageId)
    {
        this.requestToEnable = requestToEnable;
        this.dialogMessageId = dialogMessageId;
    }
}
