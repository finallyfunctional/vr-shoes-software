package com.finallyfunctional.vr_shoes.communication.commands;

public class ShoeSide
{
    public static final String SHOE_SIDE_COMMAND = "ss";
    public static final int UNSPECIFIED = -1;
    public static final int RIGHT_SIDE = 0;
    public static final int LEFT_SIDE = 1;

    public String c; //command
    public int si; //side
    public String d; //destination
    public boolean r; //reply
    public String id; //shoe ID
    public boolean g; //get

    public ShoeSide()
    {
        c = SHOE_SIDE_COMMAND;
    }
}
