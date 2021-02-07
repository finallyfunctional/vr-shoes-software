package com.finallyfunctional.vr_shoes.communication.commands;

public class ResetDistanceTracker
{
    public static final String RESET_DISTANCE_TRACKER_COMMAND = "rd";

    public String c; //command
    public String d; //destination

    public ResetDistanceTracker()
    {
        c = RESET_DISTANCE_TRACKER_COMMAND;
    }
}
