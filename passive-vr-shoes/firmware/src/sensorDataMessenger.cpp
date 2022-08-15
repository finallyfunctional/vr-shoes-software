#include "sensorDataMessenger.h"

StaticJsonDocument<500> SensorDataMessenger::json;

void SensorDataMessenger::constructMessage(VrShoeSensorData data, String& message) {
    SensorDataMessenger::json.clear();
    SensorDataMessenger::json["type"] = "data";
    SensorDataMessenger::json["qw"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qw);
    SensorDataMessenger::json["qx"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qx);
    SensorDataMessenger::json["qy"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qy);
    SensorDataMessenger::json["qz"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qz);
    SensorDataMessenger::json["ticks"] = data.encoderTicks;

    serializeJson(json, message);
}

void SensorDataMessenger::serialPrintSensorData(VrShoeSensorData data) {
    String message;
    SensorDataMessenger::constructMessage(data, message);
    Serial.println(message);
}