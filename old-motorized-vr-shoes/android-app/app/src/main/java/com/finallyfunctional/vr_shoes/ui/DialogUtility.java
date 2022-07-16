package com.finallyfunctional.vr_shoes.ui;

import android.app.Activity;
import android.content.DialogInterface;

import com.finallyfunctional.vr_shoes.R;

import androidx.appcompat.app.AlertDialog;

public class DialogUtility
{
    public static void showOkayDialog(Activity activity, String message, final Runnable callback)
    {
        AlertDialog.Builder builder = new AlertDialog.Builder(activity);
        builder.setMessage(message);
        builder.setPositiveButton(activity.getString(R.string.OK), new DialogInterface.OnClickListener()
        {
            @Override
            public void onClick(DialogInterface dialog, int which)
            {
                dialog.dismiss();
                callback.run();
            }
        });
        builder.setCancelable(false);
        AlertDialog dialog =  builder.create();
        dialog.show();
    }
}
