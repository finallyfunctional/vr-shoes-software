package com.finallyfunctional.vr_shoes.communication.commands;

public class StopNegatingMotion
{
    public static final String STOP_NEGATING_MOTION = "snm";

    public String c; //command
    public String d; //destination

    public StopNegatingMotion()
    {
        c = STOP_NEGATING_MOTION;
    }
}
