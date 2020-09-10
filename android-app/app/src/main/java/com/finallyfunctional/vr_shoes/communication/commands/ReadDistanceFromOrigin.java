package com.finallyfunctional.vr_shoes.communication.commands;

public class ReadDistanceFromOrigin
{
    public static final String READ_DISTANCE_FROM_ORIGIN_COMMAND = "readDistanceFromOrigin";

    public String command;
    public boolean reply;
    public String deviceId;
    public float forwardDirection;
    public float sidewaysDirection;

    public ReadDistanceFromOrigin()
    {
        command = READ_DISTANCE_FROM_ORIGIN_COMMAND;
    }
}
