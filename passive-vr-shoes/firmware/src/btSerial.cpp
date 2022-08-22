#include "btSerial.h"

const char* BtSerial::deviceIdPrefix = "VR-Shoe-";
const char* BtSerial::shoeIdKey = "device-id";

void BtSerial::initialize() {
    serialBt.begin(getShoeId());
    serialBt.setTimeout(10);
}

String BtSerial::getShoeId() {
    String shoeId = btPerferences.getString(shoeIdKey);
    if(shoeId == NULL || shoeId.equals(""))
    {
        shoeId = deviceIdPrefix + String(random(100000, 1000000));
        btPerferences.putString(shoeIdKey, shoeId);
    }
    return shoeId;
}

void BtSerial::println(String message) {
    serialBt.println(message);
}

void BtSerial::print(String message) {
    serialBt.print(message);
}