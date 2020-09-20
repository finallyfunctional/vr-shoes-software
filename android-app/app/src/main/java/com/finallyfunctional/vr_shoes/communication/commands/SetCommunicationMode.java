package com.finallyfunctional.vr_shoes.communication.commands;

public class SetCommunicationMode
{
    public static final String SET_COMMUNICATION_MODE_COMMAND = "setCommunicationMode";
    public static final String BT_MASTER_MODE = "btMasterMode";
    public static final String BT_SLAVE_MODE = "btSlaveMode";

    public String command;
    public String to;
    public String mode;
    public String otherShoeId;

    public SetCommunicationMode()
    {
        command = SET_COMMUNICATION_MODE_COMMAND;
    }
}
