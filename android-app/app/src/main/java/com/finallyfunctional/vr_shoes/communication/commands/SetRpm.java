package com.finallyfunctional.vr_shoes.communication.commands;

public class SetRpm
{
    public static final String SET_RPM = "sr";

    public String c; //command
    public float fr; //forward RPM
    public float sr; //sideway RPM
    public String d; //destination
    public boolean r; //reply

    public SetRpm()
    {
        c = SET_RPM;
    }
}
