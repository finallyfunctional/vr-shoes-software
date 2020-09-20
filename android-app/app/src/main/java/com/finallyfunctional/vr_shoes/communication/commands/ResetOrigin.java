package com.finallyfunctional.vr_shoes.communication.commands;

public class ResetOrigin
{
    public static final String RESET_ORIGIN_COMMAND = "resetOrigin";

    public String command;
    public String to;

    public ResetOrigin()
    {
        command = RESET_ORIGIN_COMMAND;
    }
}
