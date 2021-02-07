package com.finallyfunctional.vr_shoes.communication.commands;

public class StartNegatingMotion
{
    public static final String START_NEGATING_MOTION = "nm";

    public String c; //command
    public String d; //destination

    public StartNegatingMotion()
    {
        c = START_NEGATING_MOTION;
    }
}
