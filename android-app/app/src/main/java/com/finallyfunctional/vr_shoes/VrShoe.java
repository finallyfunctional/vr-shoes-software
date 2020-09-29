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

    private float sidewayPeakCurrent;
    private float sidewayCurrentNow;
    private float sidewayAverageCurrent;
    private float sidewayAmpHours;
    private float sidewayAmpCharged;

    private float forwardPeakCurrent;
    private float forwardCurrentNow;
    private float forwardAverageCurrent;
    private float forwardAmpHours;
    private float forwardAmpCharged;

    private float speedMultiplier;

    private float kp, ki, kd;

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

    public void setSidewayPeakCurrent(float current) {sidewayPeakCurrent = current;}

    public float getSidewayPeakCurrent() {return sidewayPeakCurrent;}

    public void setSidewayCurrentNow(float current) {sidewayCurrentNow = current;}

    public float getSidewayCurrentNow() {return sidewayCurrentNow;}

    public void setSidewayAverageCurrent(float current) {sidewayAverageCurrent = current;}

    public float getSidewayAverageCurrent() {return sidewayAverageCurrent;}

    public void setSidewayAmpHours(float ampHours) {sidewayAmpHours = ampHours;}

    public float getSidewayAmpHours() {return sidewayAmpHours;}

    public void setSidewayAmpCharged(float ampHours) {sidewayAmpCharged = ampHours;}

    public float getSidewayAmpCharged() {return sidewayAmpCharged;}

    public void setForwardPeakCurrent(float current) {forwardPeakCurrent = current;}

    public float getForwardPeakCurrent() {return forwardPeakCurrent;}

    public void setForwardCurrentNow(float current) {forwardCurrentNow = current;}

    public float getForwardCurrentNow() {return forwardCurrentNow;}

    public void setForwardAverageCurrent(float current) {forwardAverageCurrent = current;}

    public float getForwardAverageCurrent() {return forwardAverageCurrent;}

    public void setForwardAmpHours(float ampHours) {forwardAmpHours = ampHours;}

    public float getForwardAmpHours() {return forwardAmpHours;}

    public void setForwardAmpCharged(float ampHours) {forwardAmpCharged = ampHours;}

    public float getForwardAmpCharged() {return forwardAmpCharged;}

    public float getSpeedMultiplier()
    {
        return speedMultiplier;
    }

    public void setSpeedMultiplier(float speedMultiplier)
    {
        this.speedMultiplier = speedMultiplier;
    }

    public void setPidParameters(float kp, float ki, float kd)
    {
        this.kp = kp;
        this.ki = ki;
        this.kd = kd;
    }

    public float getPidKp()
    {
        return kp;
    }

    public float getPidKi()
    {
        return ki;
    }

    public float getPidKd()
    {
        return kd;
    }
}
