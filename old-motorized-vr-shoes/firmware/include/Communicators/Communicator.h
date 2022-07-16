#ifndef Communicator_h
#define Communicator_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include "./Messages.h"
#include "../Sensors/Sensors.h"
#include "../Vector2D.h"
#include "../VrShoeGlobals.h"
#include "./ResponseCodes.h"
#include "./MessageKeys.h"
#include "../Timer.h"
#include "../ShoeSides.h"
#include "../AutoShoeControllers/AutoShoeController.h"
#include "../Sensors/PowerStatistics.h"
#include "../Sensors/PowerTracker.h"
#include "../Sensors/Buttons/Button.h"

class Communicator
{
    public:
    void initialize(Sensors* sensors, AutoShoeController* shoeController);
    void update();

    private:
    int ping();
    int replyWithSensorData();
    void sendSensorDataIfStale(String sendingToDeviceId);
    void sendSensorData();
    int recieveSensorData();
    int resetDistanceTracker();
    int setRpm();
    int configureShoe();
    int getShoeConfigurations();
    int startNegatingMotion();
    int stopNegatingMotion();
    int getPowerStatistics();
    float roundFloatToTwoDecimalPlaces(float number);
    bool isGetRequest();
    int setShoeSide(int shoeSide);

    static const char* SHOE_ID_KEY;
    static const String DEVICE_ID_PREFIX;

    protected:
    virtual void initializeCommunication() = 0;
    virtual void processMessages() = 0;
    virtual int handleImplementationSpecificMessage(String commandId) = 0;
    virtual int setCommunicationMode() = 0;
    virtual bool isInitialized() = 0;
    virtual bool shouldForwardMessage() = 0;
    virtual void sendMessageTo(String sendingToDeviceId) = 0;
    int handleRecievedMessage(String message);
    String cleanMessage(String message);

    Sensors* sensors;
    AutoShoeController* shoeController;
    Timer* sendSensorDataTimer;

    static const char MESSAGE_TERMINATOR;

    String shoeId;
    String lastSensorDataMessageSent;
    StaticJsonDocument<500> json;
};

#endif