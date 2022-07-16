package com.finallyfunctional.vr_shoes.logging;

import android.util.Log;

public class VrShoesLogcatLogger implements IVrShoesAppLogger
{
    private static VrShoesLogcatLogger logger;

    private VrShoesLogcatLogger()
    {

    }

    public static VrShoesLogcatLogger getLogger()
    {
        if(logger == null)
        {
            logger = new VrShoesLogcatLogger();
        }
        return logger;
    }

    public static void initialize()
    {
        logger = new VrShoesLogcatLogger();
    }

    @Override
    public void error(String tag, String message)
    {
        Log.e(tag, message);
    }

    @Override
    public void warning(String tag, String message)
    {
        Log.w(tag, message);
    }

    @Override
    public void info(String tag, String message)
    {
        Log.i(tag, message);
    }

    @Override
    public void debug(String tag, String message)
    {
        Log.d(tag, message);
    }

    @Override
    public void verbose(String tag, String message)
    {
        Log.v(tag, message);
    }

    @Override
    public void error(String tag, String message, Throwable ex)
    {
        Log.e(tag, message, ex);
    }

    @Override
    public void warning(String tag, String message, Throwable ex)
    {
        Log.w(tag, message, ex);
    }

    @Override
    public void info(String tag, String message, Throwable ex)
    {
        Log.i(tag, message, ex);
    }

    @Override
    public void debug(String tag, String message, Throwable ex)
    {
        Log.d(tag, message, ex);
    }

    @Override
    public void verbose(String tag, String message, Throwable ex)
    {
        Log.v(tag, message, ex);
    }
}
