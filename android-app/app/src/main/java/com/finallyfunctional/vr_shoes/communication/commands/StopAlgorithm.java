package com.finallyfunctional.vr_shoes.communication.commands;

public class StopAlgorithm
{
    public static final String STOP_ALGORITHM_COMMAND = "soa";

    public String c; //command
    public String d; //destination

    public StopAlgorithm()
    {
        c = STOP_ALGORITHM_COMMAND;
    }
}
