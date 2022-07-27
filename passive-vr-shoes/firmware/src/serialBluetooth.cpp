#include "serialBluetooth.h"

const String SerialBluetooth::DEVICE_ID_PREFIX = "VR-Shoe-";
const char* SerialBluetooth::SHOE_ID_KEY = "device-id";

bool SerialBluetooth::initialize() {
    if(serialBt.begin(getShoeId())) {
        serialBt.setTimeout(10);
        return true;
    }
    Serial.println("Bluetooth failed to initialize");
    return false;
}

String SerialBluetooth::getShoeId() {
    String shoeId = blPreferences.getString(SHOE_ID_KEY);
    if(shoeId == NULL || shoeId.equals(""))
    {
        shoeId = DEVICE_ID_PREFIX + String(random(100000, 1000000));
        blPreferences.putString(SHOE_ID_KEY, shoeId);
    }
    return shoeId;
}

void SerialBluetooth::update(VrShoeSensorData data) {
    constructMessage(data);
    String message;
    serializeJson(json, message);
    serialBt.println(message);
}

void SerialBluetooth::constructMessage(VrShoeSensorData data) {
    json.clear();
    json["type"] = "data";
    json["qw"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qw);
    json["qx"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qx);
    json["qy"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qy);
    json["qz"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qz);
    json["ticks"] = data.encoderTicks;
}
