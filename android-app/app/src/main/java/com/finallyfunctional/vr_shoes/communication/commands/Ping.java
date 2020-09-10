package com.finallyfunctional.vr_shoes.communication.commands;

public class Ping
{
    public static final String PING_COMMAND = "ping";

    public String command;

    public Ping()
    {
        command = PING_COMMAND;
    }
}
