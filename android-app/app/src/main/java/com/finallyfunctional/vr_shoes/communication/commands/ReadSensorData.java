package com.finallyfunctional.vr_shoes.communication.commands;

public class ReadSensorData
{
    public static final String READ_SENSOR_DATA_COMMAND = "readSensorData";

    public String command;
    public String deviceId;
    public boolean frontButtonPressed;
    public boolean rearButtonPressed;

    public ReadSensorData()
    {
        command = READ_SENSOR_DATA_COMMAND;
    }
}
