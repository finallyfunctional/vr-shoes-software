package com.finallyfunctional.vr_shoes;

public class VrShoe
{
    private String deviceId;
    private boolean frontButtonPressed;
    private boolean rearButtonPressed;
    private float forwardSpeed;
    private float sidewaySpeed;
    private int side;
    private float dutyCycleBoost;

    private float forwardDesiredSpeed;
    private float sidewayDesiredSpeed;
    private float forwardDutyCycle;
    private float sidewayDutyCycle;
    private float forwardDistanceFromOrigin;
    private float sidewayDistanceFromOrigin;

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

    public void setSide(int side)
    {
        this.side = side;
    }

    public int getSide()
    {
        return side;
    }

    public void setDutyCycleBoost(float boost) {dutyCycleBoost = boost;}

    public float getDutyCycleBoost() {return dutyCycleBoost;}

    public void setForwardDesiredSpeed(float speed) {forwardDesiredSpeed = speed;}

    public void setSidewayDesiredSpeed(float speed) {sidewayDesiredSpeed = speed;}

    public float getForwardDesiredSpeed() {return forwardDesiredSpeed;}

    public float getSidewayDesiredSpeed() {return sidewayDesiredSpeed;}

    public void setForwardDutyCycle(float dutyCycle) {forwardDutyCycle = dutyCycle;}

    public void setSidewayDutyCycle(float dutyCycle) {sidewayDutyCycle = dutyCycle;}

    public float getForwardDutyCycle() {return forwardDutyCycle;}

    public float getSidewayDutyCycle() {return sidewayDutyCycle;}

    public void setForwardDistanceFromOrigin(float distance) {forwardDistanceFromOrigin = distance;}

    public void setSidewayDistanceFromOrigin(float distance) {sidewayDistanceFromOrigin = distance;}

    public float getForwardDistanceFromOrigin() {return forwardDistanceFromOrigin;}

    public float getSidewayDistanceFromOrigin() {return sidewayDistanceFromOrigin;}
}
