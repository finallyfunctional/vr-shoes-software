package com.finallyfunctional.vr_shoes.logging.monitor;

public class MonitorLog
{
    public final MonitorLogLevels level;
    public final String tag;
    public final String message;
    public final Throwable ex;

    public MonitorLog(MonitorLogLevels level, String tag, String message, Throwable ex)
    {
        this.level = level;
        this.tag = tag;
        this.message = message;
        this.ex = ex;
    }
}
