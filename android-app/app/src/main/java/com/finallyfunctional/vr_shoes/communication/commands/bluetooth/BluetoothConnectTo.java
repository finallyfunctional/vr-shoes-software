package com.finallyfunctional.vr_shoes.communication.commands.bluetooth;

public class BluetoothConnectTo
{
    public static final String BT_CONNECT_TO_COMMAND = "btConnectTo";

    public String command;
    public String remoteName;
    public String deviceId;
    public boolean reply;
    public boolean success;

    public BluetoothConnectTo()
    {
        command = BT_CONNECT_TO_COMMAND;
    }
}
