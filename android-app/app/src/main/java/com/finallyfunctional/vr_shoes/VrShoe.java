package com.finallyfunctional.vr_shoes;

public class VrShoe
{
    private String deviceId;
    private boolean frontButtonPressed;
    private boolean rearButtonPressed;
    private float forwardSpeed;
    private float sidewaySpeed;

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

    public void setForwardSpeed(float speed)
    {
        forwardSpeed = speed;
    }

    public void setSidewaySpeed(float speed)
    {
        sidewaySpeed = speed;
    }

    public float getForwardSpeed()
    {
        return forwardSpeed;
    }

    public float getSidewaySpeed()
    {
        return sidewaySpeed;
    }
}
