#include "bleCommunicator.h"

const char* BleCommunicator::SERVICE_UUID = "a0661d83-4017-4d5e-a7a9-330c74de60a3";
const char* BleCommunicator::CHARACTERISTIC_UUID = "241ff4a0-c71f-4843-9c12-11e8cfea0079";
const char* BleCommunicator::DEVICE_ID_PREFIX = "VR-Shoe-";
const char* BleCommunicator::SHOE_ID_KEY = "device-id";

bool BleCommunicator::initialize() {
    String deviceId = getShoeId();
    BLEDevice::init(deviceId.c_str());
    bleServer = BLEDevice::createServer();
    bleService = bleServer->createService(SERVICE_UUID);
    bleCharacteristic = bleService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    bleService->start();
    bleAdvertisementData.setName(deviceId.c_str());
    bleAdvertising = BLEDevice::getAdvertising();
    bleAdvertising->setAdvertisementData(bleAdvertisementData);
    bleAdvertising->addServiceUUID(SERVICE_UUID);
    bleAdvertising->setScanResponse(true);
    bleAdvertising->setMinPreferred(0x06);
    bleAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    return true;
}

String BleCommunicator::getShoeId() {
    String shoeId = blPreferences.getString(SHOE_ID_KEY);
    if(shoeId == NULL || shoeId.equals(""))
    {
        shoeId = DEVICE_ID_PREFIX + String(random(100000, 1000000));
        blPreferences.putString(SHOE_ID_KEY, shoeId);
    }
    return shoeId;
}

void BleCommunicator::update(VrShoeSensorData data) {
    constructMessage(data);
    String message;
    serializeJson(json, message);
    bleCharacteristic->setValue(message.c_str());
}

void BleCommunicator::constructMessage(VrShoeSensorData data) {
    json.clear();
    json["type"] = "data";
    json["qw"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qw);
    json["qx"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qx);
    json["qy"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qy);
    json["qz"] = FloatUtils::roundFloatToTwoDecimalPlaces(data.qz);
    json["ticks"] = data.encoderTicks;
}