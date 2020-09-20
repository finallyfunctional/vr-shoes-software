package com.finallyfunctional.vr_shoes.communication.commands;

public class DistanceFromOrigin
{
    public static final String DISTANCE_FROM_ORIGIN_COMMAND = "distanceFromOrigin";

    public String command;
    public String to;
    public boolean reply;
    public String deviceId;
    public float forwardDistance;
    public float sidewaysDistance;

    public DistanceFromOrigin()
    {
        command = DISTANCE_FROM_ORIGIN_COMMAND;
    }
}
