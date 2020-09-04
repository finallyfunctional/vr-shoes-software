package com.finallyfunctional.vr_shoes;

import android.content.Context;
import android.content.SharedPreferences;

public class Settings
{
    private static SharedPreferences pref;

    public static String VR_SHOE_DEVICE_ID_PREFIX = "VR-Shoe-";
    private static String PAIRED_VR_SHOE = "paired-vr-shoe";

    public Settings(Context context)
    {
        if(pref == null)
        {
            pref = context.getSharedPreferences("Finally-Functional-VR-Shoes", Context.MODE_PRIVATE);
        }
    }

    public String getPairedVrShoe()
    {
        return pref.getString(PAIRED_VR_SHOE, "");
    }

    public void storePairedVrShoe(String vrShoeId)
    {
        SharedPreferences.Editor editor = pref.edit();
        editor.putString(PAIRED_VR_SHOE, vrShoeId);
        editor.apply();
    }
}
