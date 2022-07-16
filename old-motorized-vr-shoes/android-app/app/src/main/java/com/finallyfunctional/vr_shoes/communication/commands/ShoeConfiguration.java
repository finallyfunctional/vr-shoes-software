package com.finallyfunctional.vr_shoes.communication.commands;

import com.finallyfunctional.vr_shoes.VrShoe;

public class ShoeConfiguration
{
    public static final String SHOE_CONFIGURATION_COMMAND = "sc";

    public String c; //command
    public String id; //shoe ID
    public String d; //destination
    public boolean r; //reply
    public boolean g; //get
    public String osi; //other shoe ID
    public int si; //side
    public float dcb; //duty cycle boost
    public float spm; //speed multiplier
    public float cr; //center radius
    public float co; //center offset
    public boolean csl; //calculate stride length

    public ShoeConfiguration()
    {
        c = SHOE_CONFIGURATION_COMMAND;
    }

    public ShoeConfiguration(VrShoe vrShoe)
    {
        c = SHOE_CONFIGURATION_COMMAND;
        id = vrShoe.getDeviceId();
        osi = vrShoe.getOtherShoeDeviceId();
        dcb = vrShoe.getDutyCycleBoost();
        si = vrShoe.getSide();
        spm = vrShoe.getSpeedMultiplier();
        cr = vrShoe.centerRadius;
        co = vrShoe.centerOffset;
        csl = vrShoe.calculateStrideLength;
    }
}
