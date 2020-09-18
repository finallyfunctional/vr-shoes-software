package com.finallyfunctional.vr_shoes.logging.monitor;

import com.finallyfunctional.vr_shoes.logging.IVrShoesAppLogger;
import com.finallyfunctional.vr_shoes.logging.VrShoesLogcatLogger;

import java.util.ArrayList;

public class VrShoesAggregateLogger implements IVrShoesAppLogger
{
    private static VrShoesAggregateLogger logger;

    private ArrayList<IVrShoesAppLogger> loggers;

    private VrShoesAggregateLogger()
    {
        loggers = new ArrayList<>();
        loggers.add(VrShoesMonitor.getLogger());
        loggers.add(VrShoesLogcatLogger.getLogger());
    }

    public static void initialize()
    {
        logger = new VrShoesAggregateLogger();
    }

    public static VrShoesAggregateLogger getLogger()
    {
        if(logger == null)
        {
            logger = new VrShoesAggregateLogger();
        }
        return logger;
    }

    private void logMessage(LogExpression expression)
    {
        for(IVrShoesAppLogger logger : loggers)
        {
            expression.logMessage(logger);
        }
    }

    @Override
    public void error(final String tag, final String message)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.error(tag, message);
            }
        });
    }

    @Override
    public void warning(final String tag, final String message)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.warning(tag, message);
            }
        });
    }

    @Override
    public void info(final String tag, final String message)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.info(tag, message);
            }
        });
    }

    @Override
    public void debug(final String tag, final String message)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.debug(tag, message);
            }
        });
    }

    @Override
    public void verbose(final String tag, final String message)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.verbose(tag, message);
            }
        });
    }

    @Override
    public void error(final String tag, final String message, final Throwable ex)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.error(tag, message, ex);
            }
        });
    }

    @Override
    public void warning(final String tag, final String message, final Throwable ex)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.warning(tag, message, ex);
            }
        });
    }

    @Override
    public void info(final String tag, final String message, final Throwable ex)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.info(tag, message, ex);
            }
        });
    }

    @Override
    public void debug(final String tag, final String message, final Throwable ex)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.debug(tag, message, ex);
            }
        });
    }

    @Override
    public void verbose(final String tag, final String message, final Throwable ex)
    {
        logMessage(new LogExpression()
        {
            @Override
            public void logMessage(IVrShoesAppLogger logger)
            {
                logger.verbose(tag, message, ex);
            }
        });
    }

    private interface LogExpression
    {
        void logMessage(IVrShoesAppLogger logger);
    }
}
