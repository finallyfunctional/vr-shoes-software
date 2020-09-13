package com.finallyfunctional.vr_shoes.communication.commands;

public class SetRpm
{
    public static final String SET_RPM = "setRpm";

    public String command;
    public float forwardRpm;
    public float sidewayRpm;

    public boolean reply;
    public String message;

    public SetRpm()
    {
        command = SET_RPM;
    }
}
