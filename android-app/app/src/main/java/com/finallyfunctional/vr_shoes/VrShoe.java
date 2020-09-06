package com.finallyfunctional.vr_shoes;

public class VrShoe
{
    private String deviceId;
    private boolean frontButtonPressed;
    private boolean rearButtonPressed;

    public String getDeviceId()
    {
        return deviceId;
    }

    public boolean isFrontButtonPressed()
    {
        return frontButtonPressed;
    }

    public boolean isRearButtonPressed()
    {
        return rearButtonPressed;
    }

    public void setDeviceId(String deviceId)
    {
        this.deviceId = deviceId;
    }

    public void frontButtonPressed(boolean pressed)
    {
        frontButtonPressed = pressed;
    }

    public void rearButtonPressed(boolean pressed)
    {
        rearButtonPressed = pressed;
    }
}
