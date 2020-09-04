#ifndef AbstractCommunicator_h
#define AbstractCommunicator_h

#include <Arduino.h>
#include <Preferences.h>

class AbstractCommunicator
{
    public:
    virtual void processMessages() = 0;
    void initialize(Preferences preferences);

    private:
    static const char* DEVICE_ID_KEY;

    protected:
    virtual void initializeCommunication() = 0;

    String deviceId;
};

#endif