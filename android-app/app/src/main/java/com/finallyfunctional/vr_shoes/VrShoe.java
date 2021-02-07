package com.finallyfunctional.vr_shoes;

public class VrShoe
{
    private String deviceId;
    private String otherShoeDeviceId;
    private boolean frontButtonPressed;
    private boolean rearButtonPressed;
    private float forwardSpeed;
    private float sidewaysSpeed;
    private int side;

    private float dutyCycleBoost;
    private float forwardDistance;
    private float sidewaysDistance;

    private float sidewaysPeakCurrent;
    private float sidewaysCurrentNow;
    private float sidewaysAverageCurrent;
    private float sidewaysAmpHours;
    private float sidewaysAmpCharged;

    private float forwardPeakCurrent;
    private float forwardCurrentNow;
    private float forwardAverageCurrent;
    private float forwardAmpHours;
    private float forwardAmpCharged;

    private float speedMultiplier;

    public String getDeviceId()
    {
        return deviceId;
    }

    public String getOtherShoeDeviceId() {return otherShoeDeviceId;}

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

    public void setOtherShoeDeviceId(String otherShoeDeviceId) {this.otherShoeDeviceId = otherShoeDeviceId;};

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

    public void setSidewaysSpeed(float speed)
    {
        sidewaysSpeed = speed;
    }

    public float getForwardSpeed()
    {
        return forwardSpeed;
    }

    public float getSidewaysSpeed()
    {
        return sidewaysSpeed;
    }

    public void setSide(int side)
    {
        this.side = side;
    }

    public int getSide()
    {
        return side;
    }

    public float getDutyCycleBoost()
    {
        return dutyCycleBoost;
    }

    public void setDutyCycleBoost(float boost) {dutyCycleBoost = boost;}

    public void setForwardDistance(float distance) {
        forwardDistance = distance;}

    public void setSidewaysDistance(float distance) {
        sidewaysDistance = distance;}

    public float getForwardDistance() {return forwardDistance;}

    public float getSidewaysDistance() {return sidewaysDistance;}

    public void setSidewaysPeakCurrent(float current) {
        sidewaysPeakCurrent = current;}

    public float getSidewaysPeakCurrent() {return sidewaysPeakCurrent;}

    public void setSidewaysCurrentNow(float current) {
        sidewaysCurrentNow = current;}

    public float getSidewaysCurrentNow() {return sidewaysCurrentNow;}

    public void setSidewaysAverageCurrent(float current) {
        sidewaysAverageCurrent = current;}

    public float getSidewaysAverageCurrent() {return sidewaysAverageCurrent;}

    public void setSidewaysAmpHours(float ampHours) {
        sidewaysAmpHours = ampHours;}

    public float getSidewaysAmpHours() {return sidewaysAmpHours;}

    public void setSidewaysAmpCharged(float ampHours) {
        sidewaysAmpCharged = ampHours;}

    public float getSidewaysAmpCharged() {return sidewaysAmpCharged;}

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
}
