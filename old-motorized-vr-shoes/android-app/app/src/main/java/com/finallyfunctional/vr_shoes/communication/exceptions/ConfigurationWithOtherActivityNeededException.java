package com.finallyfunctional.vr_shoes.communication.exceptions;

public class ConfigurationWithOtherActivityNeededException extends Exception
{
    public final Class otherActivity;
    public final boolean showDialogFirst;
    public final int dialogMessageId;

    public ConfigurationWithOtherActivityNeededException(Class otherActivity)
    {
        this.otherActivity = otherActivity;
        this.showDialogFirst = false;
        this.dialogMessageId = -1;
    }

    public ConfigurationWithOtherActivityNeededException(Class otherActivity, int dialogMessageId)
    {
        this.otherActivity = otherActivity;
        this.showDialogFirst = true;
        this.dialogMessageId = dialogMessageId;
    }
}
