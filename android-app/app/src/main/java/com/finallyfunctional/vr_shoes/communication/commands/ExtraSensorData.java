package com.finallyfunctional.vr_shoes.communication.commands;

public class ExtraSensorData
{
    public static final String EXTRA_SENSOR_DATA_COMMAND = "esd";

    public String c; //command
    public String d; //destination
    public String id; //shoe ID
    public boolean r; //reply
    public float fds; //forward desired speed
    public float sds; //sideways desired speed
    public float fdc; //forward duty cycle
    public float sdc; //sideways duty cycle

    public ExtraSensorData()
    {
        c = EXTRA_SENSOR_DATA_COMMAND;
    }
}
