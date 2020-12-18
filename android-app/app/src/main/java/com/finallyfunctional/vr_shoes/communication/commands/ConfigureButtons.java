package com.finallyfunctional.vr_shoes.communication.commands;

public class ConfigureButtons
{
    public static final String COMMAND = "cbs";

    public String c; //command
    public int bmd; //button max difference

    public ConfigureButtons()
    {
        c = COMMAND;
    }
}
