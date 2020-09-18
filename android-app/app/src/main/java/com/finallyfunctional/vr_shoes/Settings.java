package com.finallyfunctional.vr_shoes;

import android.content.Context;
import android.content.SharedPreferences;

public class Settings
{
    private static SharedPreferences pref;

    public static String VR_SHOE_DEVICE_ID_PREFIX = "VR-Shoe-";
    private static String PAIRED_VR_SHOE_1 = "paired-vr-shoe-1";
    private static String PAIRED_VR_SHOE_2 = "paired-vr-shoe-2";

    public Settings(Context context)
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
}
