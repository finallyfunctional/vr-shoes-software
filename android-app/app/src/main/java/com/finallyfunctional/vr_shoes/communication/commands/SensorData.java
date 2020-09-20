package com.finallyfunctional.vr_shoes.communication.commands;

public class SensorData
{
    public static final String SENSOR_DATA_COMMAND = "sensorData";

    public String command;
    public String deviceId;
    public boolean get;
    public boolean frontButtonPressed;
    public boolean rearButtonPressed;
    public float forwardSpeed;
    public float sidewaySpeed;

    public SensorData()
    {
        command = SENSOR_DATA_COMMAND;
    }
}
