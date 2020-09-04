#include "../../include/Communicators/AbstractCommunicator.h"

const String AbstractCommunicator::DEVICE_ID_PREFIX = "VR-Shoe-";
const char* AbstractCommunicator::DEVICE_ID_KEY = "device-id";

void AbstractCommunicator::initialize(Preferences preferences)
{
    deviceId = preferences.getString(DEVICE_ID_KEY);
    if(deviceId == NULL || deviceId.equals(""))
    {
        deviceId = DEVICE_ID_PREFIX + String(random(100000, 1000000));
        preferences.putString(DEVICE_ID_KEY, deviceId);
    }
    initializeCommunication();
}