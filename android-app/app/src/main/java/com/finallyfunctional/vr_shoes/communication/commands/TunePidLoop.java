package com.finallyfunctional.vr_shoes.communication.commands;

public class TunePidLoop
{
    public static final String TUNE_PID_LOOP_COMMAND = "tpl";

    public String c;
    public float kp;
    public float ki;
    public float kd;

    public TunePidLoop()
    {
        c = TUNE_PID_LOOP_COMMAND;
    }
}
