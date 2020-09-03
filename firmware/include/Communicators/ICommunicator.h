#ifndef ICommunicator_h
#define ICommunicator_h

#include <Arduino.h>

class ICommunicator
{
    public:
    virtual void initialize() = 0;
    virtual void processMessages() = 0;
};

#endif