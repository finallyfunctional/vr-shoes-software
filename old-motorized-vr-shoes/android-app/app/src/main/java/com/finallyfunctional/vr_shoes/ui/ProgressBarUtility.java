package com.finallyfunctional.vr_shoes.ui;

import android.app.Activity;
import android.view.View;
import android.view.WindowManager;
import android.widget.ProgressBar;

public class ProgressBarUtility
{
    public static void showProgressBar(ProgressBar bar, Activity activity)
    {
        bar.bringToFront();
        bar.setVisibility(View.VISIBLE);
        activity.getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE,
                WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
    }

    public static void hideProgressBar(ProgressBar bar, Activity activity)
    {
        bar.bringToFront();
        bar.setVisibility(View.INVISIBLE);
        activity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE);
    }
}
