#include <Arduino.h>
#include "Wire.h"
#include "imu.h"
#include "encoder.h"
#include "vrShoeSensorData.h"
#include "serialBluetooth.h"

#define ENCODER_A 16
#define ENCODER_B 17

bool imuReady = false;
bool bluetoothReady = false;
VrShoeSensorData data, previouslySentData;
SerialBluetooth serialBt;

void updateSensorData() {
    data.encoderTicks = Encoder::ticks;
    data.qw = IMU::orientation.w;
    data.qx = IMU::orientation.x;
    data.qy = IMU::orientation.y;
    data.qz = IMU::orientation.z;
}

void setup() {
    Wire.begin();
    Wire.setClock(400000);

    Serial.begin(115200);

    Serial.println("Initializing components");
    imuReady = IMU::initialize();
    Encoder::initialize(ENCODER_A, ENCODER_B);
    Serial.println("Initializing bluetooth");
    bluetoothReady = serialBt.initialize();
}

void loop() {
    if (!imuReady || !bluetoothReady) {
        return;
    }
    if (IMU::isNewData()) {
        IMU::updateOrientation();
    }
    updateSensorData();
    if(data != previouslySentData) {
        serialBt.update(data);
        previouslySentData = data;
    }
}
