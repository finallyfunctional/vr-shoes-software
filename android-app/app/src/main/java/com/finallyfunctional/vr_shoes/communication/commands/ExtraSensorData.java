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
    public float spk; //sideway peak current
    public float scn; //sideway current now
    public float sac; //sideway average current
    public float sah; //sideway amp hours
    public float sahc; //sideway amp hours charged
    public float fpc; //forward peak current
    public float fcn; //forward current now
    public float fac; //forward average current
    public float fah; //forward amp hours
    public float fahc; //forward amp hours charged

    public ExtraSensorData()
    {
        c = EXTRA_SENSOR_DATA_COMMAND;
    }
}
