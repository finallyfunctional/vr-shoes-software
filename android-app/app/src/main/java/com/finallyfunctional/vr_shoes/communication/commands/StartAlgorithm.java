package com.finallyfunctional.vr_shoes.communication.commands;

public class StartAlgorithm
{
    public static final String START_ALGORITHM_COMMAND = "sa";

    public String c; //command
    public String d; //destination

    public StartAlgorithm()
    {
        c = START_ALGORITHM_COMMAND;
    }
}
