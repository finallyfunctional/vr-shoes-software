package com.finallyfunctional.vr_shoes.logging;

public interface IVrShoesAppLogger
{
    void error(String tag, String message);
    void warning(String tag, String message);
    void info(String tag, String message);
    void debug(String tag, String message);
    void verbose(String tag, String message);

    void error(String tag, String message, Throwable ex);
    void warning(String tag, String message, Throwable ex);
    void info(String tag, String message, Throwable ex);
    void debug(String tag, String message, Throwable ex);
    void verbose(String tag, String message, Throwable ex);
}
