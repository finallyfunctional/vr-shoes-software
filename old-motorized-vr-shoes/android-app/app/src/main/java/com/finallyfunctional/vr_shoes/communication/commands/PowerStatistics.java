package com.finallyfunctional.vr_shoes.communication.commands;

public class PowerStatistics
{
    public static final String POWER_STATISTICS_COMMAND = "ps";

    public String c; //command
    public String d; //destination
    public String id; //shoe ID
    public boolean r; //reply
    public float spk; //sideway peak current
    public float scn; //sideway current now
    public float sac; //sideway average current
    public float sah; //sideway amp hours
    public float sahc; //sideway amp hours charged
    public float fpc; //forward peak current
    public float fcn; //forward current now
    public float fac; //forward average current
    public float fah; //forward amp hours
    public float fahc; //forward amp hours charged

    public PowerStatistics()
    {
        c = POWER_STATISTICS_COMMAND;
    }
}
