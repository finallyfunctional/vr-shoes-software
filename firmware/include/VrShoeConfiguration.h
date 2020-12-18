#ifndef VrShoeConfiguration_h
#define VrShoeConfiguration_h

#include <Arduino.h>
#include <Preferences.h>
#include "./Communicators/Communicator.h"
#include "./Communicators/Bluetooth/BluetoothCommunicator.h"
#include "./Communicators/CommunicatorTypes.h"
#include "./Sensors/Buttons/Button.h"
#include "./Sensors/Buttons/LinearHallEffectSensorButton.h"
#include "./Sensors/Sensors.h"
#include "./Sensors/VescPair.h"
#include "./Sensors/Vesc.h"
#include "./Wheels/Rotacaster50mmWheel.h"
#include "./Motors/Flipsky270kvMotor.h"
#include "./Sensors/RemoteVrShoe.h"
#include "./VrShoeGlobals.h"
#include "./AutoShoeControllers/AutoShoeController.h"
#include "./AutoShoeControllers/OmniPoweredWalking.h"
#include "./ShoeSides.h"
#include "./Sensors/PowerTracker.h"

class VrShoeConfiguration
{
    public:
    void initialize();
    Communicator* getCommunicator();
    Sensors* getSensors();
    AutoShoeController* getShoeController();

    private:
    Communicator* communicator;
    Sensors* sensors;
    AutoShoeController* shoeController;

    static const char* COMMUNICATOR_TYPE_KEY;
    static const int FRONT_BUTTON_PIN = 35;
    static const int REAR_BUTTON_PIN = 39;

};

#endif