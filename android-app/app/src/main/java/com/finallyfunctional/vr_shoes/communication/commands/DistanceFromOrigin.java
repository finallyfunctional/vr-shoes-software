package com.finallyfunctional.vr_shoes.communication.commands;

public class DistanceFromOrigin
{
    public static final String DISTANCE_FROM_ORIGIN_COMMAND = "do";

    public String c; //command
    public String d; //destination
    public boolean r; //reply
    public String id; //shoe ID
    public float fd; //forward distance
    public float sd; //sideways distance

    public DistanceFromOrigin()
    {
        c = DISTANCE_FROM_ORIGIN_COMMAND;
    }
}
