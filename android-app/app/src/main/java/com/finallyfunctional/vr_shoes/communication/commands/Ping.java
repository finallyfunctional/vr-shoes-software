package com.finallyfunctional.vr_shoes.communication.commands;

public class Ping
{
    public static final String PING_COMMAND = "p";

    public String c; //command
    public String d; //destination

    public Ping()
    {
        c = PING_COMMAND;
    }
}
