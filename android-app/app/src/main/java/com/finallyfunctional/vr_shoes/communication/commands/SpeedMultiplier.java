package com.finallyfunctional.vr_shoes.communication.commands;

public class SpeedMultiplier
{
    public static final String SPEED_MULTIPLER_COMMAND = "sm";

    public String c; //command
    public boolean g; //get
    public float spm; //speed multipler

    public SpeedMultiplier()
    {
        c = SPEED_MULTIPLER_COMMAND;
    }
}
