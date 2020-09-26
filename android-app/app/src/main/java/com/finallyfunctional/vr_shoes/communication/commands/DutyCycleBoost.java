package com.finallyfunctional.vr_shoes.communication.commands;

public class DutyCycleBoost
{
    public static final String DUTY_CYCLE_BOOST_COMMAND = "dcb";

    public String c; //command
    public String d; //destination
    public float dcb; //duty cycle boost

    public DutyCycleBoost()
    {
        c = DUTY_CYCLE_BOOST_COMMAND;
    }
}
