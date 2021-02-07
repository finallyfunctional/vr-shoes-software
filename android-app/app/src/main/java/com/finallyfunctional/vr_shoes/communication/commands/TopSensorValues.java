package com.finallyfunctional.vr_shoes.communication.commands;

public class TopSensorValues
{
    public static final String TOP_SENSOR_VALUES_COMMAND = "tsv";

    public String c; //command
    public String id; //shoe ID
    public int fbpv; //front button pressed value
    public int rbpv; //rear button pressed value
    public int bmd; //button max difference

    public TopSensorValues()
    {
        c = TOP_SENSOR_VALUES_COMMAND;
    }
}
