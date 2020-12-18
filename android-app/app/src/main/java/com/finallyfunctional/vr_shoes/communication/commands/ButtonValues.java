package com.finallyfunctional.vr_shoes.communication.commands;

public class ButtonValues
{
    public static final String BUTTON_VALUES_COMMAND = "bv";

    public String c; //command
    public String id; //shoe ID
    public int fbpv; //front button pressed value
    public int rbpv; //rear button pressed value
    public int bmd; //button max difference

    public ButtonValues()
    {
        c = BUTTON_VALUES_COMMAND;
    }
}
