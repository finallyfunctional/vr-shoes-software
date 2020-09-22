package com.finallyfunctional.vr_shoes.communication.commands;

public class SetCommunicationMode
{
    public static final String SET_COMMUNICATION_MODE_COMMAND = "scm";
    public static final String BT_MASTER_MODE = "m";
    public static final String BT_SLAVE_MODE = "s";

    public String c; //command
    public String d; //destination
    public String m; //mode
    public String oi; //other shoe ID

    public SetCommunicationMode()
    {
        c = SET_COMMUNICATION_MODE_COMMAND;
    }
}
