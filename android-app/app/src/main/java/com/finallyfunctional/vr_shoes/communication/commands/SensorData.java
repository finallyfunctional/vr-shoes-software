package com.finallyfunctional.vr_shoes.communication.commands;

public class SensorData
{
    public static final String SENSOR_DATA_COMMAND = "sd";

    public String c; //command
    public String id; //shoe ID
    public boolean g; //get
    public boolean fb; //front button pressed
    public boolean rb; //rear button pressed
    public float fs; //forward speed
    public float ss; //sidway speed

    public SensorData()
    {
        c = SENSOR_DATA_COMMAND;
    }
}
