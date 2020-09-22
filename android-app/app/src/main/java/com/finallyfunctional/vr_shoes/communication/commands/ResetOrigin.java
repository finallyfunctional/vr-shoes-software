package com.finallyfunctional.vr_shoes.communication.commands;

public class ResetOrigin
{
    public static final String RESET_ORIGIN_COMMAND = "ro";

    public String c; //command
    public String d; //destination

    public ResetOrigin()
    {
        c = RESET_ORIGIN_COMMAND;
    }
}
