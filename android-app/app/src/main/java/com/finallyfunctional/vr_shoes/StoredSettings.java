package com.finallyfunctional.vr_shoes;

import android.content.Context;
import android.content.SharedPreferences;

import com.finallyfunctional.vr_shoes.communication.CommunicationModes;

public class StoredSettings
{
    private static SharedPreferences pref;

    public static String VR_SHOE_DEVICE_ID_PREFIX = "VR-Shoe-";
    private static String PAIRED_VR_SHOE_1 = "paired-vr-shoe-1";
    private static String PAIRED_VR_SHOE_2 = "paired-vr-shoe-2";
    private static String MASTER_VR_SHOE = "master-vr-shoe";
    private static String COMMUNICATION_MODE = "communication-mode";
    private static String DUTY_CYCLE_BOOST = "duty-cycle-boost";
    private static String SPEED_MULTIPLIER = "speed-multiplier";
    private static String KP = "pid-kp";
    private static String KI = "pid-ki";
    private static String KD = "pid-kd";

    public StoredSettings(Context context)
    {
        if(pref == null)
        {
            pref = context.getSharedPreferences("Finally-Functional-VR-Shoes", Context.MODE_PRIVATE);
        }
    }

    public String getPairedVrShoe1()
    {
        return pref.getString(PAIRED_VR_SHOE_1, "");
    }

    public String getPairedVrShoe2() {return pref.getString(PAIRED_VR_SHOE_2, "");}

    public float getDutyCycleBoost() {return pref.getFloat(DUTY_CYCLE_BOOST, 0);}

    public String getMasterVrShoe() {return pref.getString(MASTER_VR_SHOE, "");}

    public String getCommunicationMode() {return pref.getString(COMMUNICATION_MODE, CommunicationModes.BLUETOOTH_SLAVE_SLAVE);}

    public float getSpeedMultiplier() {return pref.getFloat(SPEED_MULTIPLIER, 1);}

    public float getPidKp() {return pref.getFloat(KP, 0);}

    public float getPidKi() {return pref.getFloat(KI, 0);}

    public float getPidKd() {return pref.getFloat(KD, 0);}

    public void storePairedVrShoe1(String vrShoeId)
    {
        SharedPreferences.Editor editor = pref.edit();
        editor.putString(PAIRED_VR_SHOE_1, vrShoeId);
        editor.apply();
    }

    public void storePairedVrShoe2(String vrShoeId)
    {
        SharedPreferences.Editor editor = pref.edit();
        editor.putString(PAIRED_VR_SHOE_2, vrShoeId);
        editor.apply();
    }

    public void saveMasterVrShoe(String masterVrShoeId)
    {
        SharedPreferences.Editor editor = pref.edit();
        editor.putString(MASTER_VR_SHOE, masterVrShoeId);
        editor.apply();
    }

    public void saveCommunicationMode(String communicationMode)
    {
        SharedPreferences.Editor editor = pref.edit();
        editor.putString(COMMUNICATION_MODE, communicationMode);
        editor.apply();
    }

    public void saveDutyCycleBoost(float boost)
    {
        SharedPreferences.Editor editor = pref.edit();
        editor.putFloat(DUTY_CYCLE_BOOST, boost);
        editor.apply();
    }

    public void saveSpeedMultiplier(float multiplier)
    {
        SharedPreferences.Editor editor = pref.edit();
        editor.putFloat(SPEED_MULTIPLIER, multiplier);
        editor.apply();
    }

    public void savePidParameters(float kp, float ki, float kd)
    {
        SharedPreferences.Editor editor = pref.edit();
        editor.putFloat(KP, kp);
        editor.putFloat(KI, kp);
        editor.putFloat(KD, kp);
        editor.apply();
    }
}
