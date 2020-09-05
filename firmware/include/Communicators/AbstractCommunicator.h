#ifndef AbstractCommunicator_h
#define AbstractCommunicator_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "./Messages.h"

class AbstractCommunicator
{
    public:
    virtual void processMessages() = 0;
    void initialize(Preferences preferences);

    private:
    static const char* DEVICE_ID_KEY;
    static const String DEVICE_ID_PREFIX;

    StaticJsonDocument<200> json;

    protected:
    static const char MESSAGE_TERMINATOR;

    virtual void initializeCommunication() = 0;
    void handleMessage(String message);
    void ping();
    virtual void sendMessage(String message) = 0;

    String deviceId;
};

#endif