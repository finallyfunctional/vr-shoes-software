package com.finallyfunctional.vr_shoes.logging.monitor;

import com.finallyfunctional.vr_shoes.logging.IVrShoesAppLogger;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class VrShoesMonitor implements IVrShoesAppLogger
{
    private static VrShoesMonitor logger;
    private static final int MAX_MESSAGES = 500;

    private LinkedList<MonitorLog> messages;
    private ArrayList<IMonitorLogObserver> observers;

    private VrShoesMonitor()
    {
        messages = new LinkedList<>();
        observers = new ArrayList<>();
    }

    public void addObserver(IMonitorLogObserver observer)
    {
        observers.add(observer);
    }

    public void removeObserver(IMonitorLogObserver observer)
    {
        observers.remove(observer);
    }

    public static void initialize()
    {
        logger = new VrShoesMonitor();
    }

    public static VrShoesMonitor getLogger()
    {
        if(logger == null)
        {
            logger = new VrShoesMonitor();
        }
        return logger;
    }

    public List<MonitorLog> getLogs()
    {
        return messages;
    }

    private void maintainListMaximum()
    {
        while(messages.size() > MAX_MESSAGES)
        {
            messages.remove();
        }
    }

    private void logMessage(MonitorLogLevels level, String tag, String message, Throwable ex)
    {
        MonitorLog log = new MonitorLog(level, tag, message, ex);
        messages.add(log);
        maintainListMaximum();
        for(IMonitorLogObserver observer : observers)
        {
            observer.messageLogged(log);
        }
    }

    @Override
    public void error(String tag, String message)
    {
        logMessage(MonitorLogLevels.error, tag, message, null);
    }

    @Override
    public void warning(String tag, String message)
    {
        logMessage(MonitorLogLevels.warning, tag, message, null);
    }

    @Override
    public void info(String tag, String message)
    {
        logMessage(MonitorLogLevels.info, tag, message, null);
    }

    @Override
    public void debug(String tag, String message)
    {
        logMessage(MonitorLogLevels.debug, tag, message, null);
    }

    @Override
    public void verbose(String tag, String message)
    {
        logMessage(MonitorLogLevels.verbose, tag, message, null);
    }

    @Override
    public void error(String tag, String message, Throwable ex)
    {
        logMessage(MonitorLogLevels.error, tag, message, ex);
    }

    @Override
    public void warning(String tag, String message, Throwable ex)
    {
        logMessage(MonitorLogLevels.warning, tag, message, ex);
    }

    @Override
    public void info(String tag, String message, Throwable ex)
    {
        logMessage(MonitorLogLevels.info, tag, message, ex);
    }

    @Override
    public void debug(String tag, String message, Throwable ex)
    {
        logMessage(MonitorLogLevels.debug, tag, message, ex);
    }

    @Override
    public void verbose(String tag, String message, Throwable ex)
    {
        logMessage(MonitorLogLevels.verbose, tag, message, ex);
    }
}
