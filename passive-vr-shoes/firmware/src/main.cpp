#include <Arduino.h>
#include "Wire.h"
#include "imu.h"
#include "encoder.h"
#include "vrShoeSensorData.h"
#include "bleCommunicator.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

#define ENCODER_A 16
#define ENCODER_B 17

bool imuReady = false;
bool bluetoothReady = false;
VrShoeSensorData data, previouslySentData;
BleCommunicator bleCommunicator;
TaskHandle_t communicationTask;

void feedTheWatchdog() {
    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;
}

void updateSensorData() {
    data.encoderTicks = Encoder::ticks;
    data.qw = IMU::orientation.w;
    data.qx = IMU::orientation.x;
    data.qy = IMU::orientation.y;
    data.qz = IMU::orientation.z;
}

void core0Loop(void * parameters)
{
    Serial.println("Starting core 0 loop");
    while(true) {
        yield();
        if(data != previouslySentData) {
            bleCommunicator.update(data);
            previouslySentData = data;
        }
    }
}

void setup() {
    Wire.begin();
    Wire.setClock(400000);

    Serial.begin(115200);

    Serial.println("Initializing components");
    imuReady = IMU::initialize();
    Encoder::initialize(ENCODER_A, ENCODER_B);
    Serial.println("Initializing bluetooth");
    bluetoothReady = bleCommunicator.initialize();
    Serial.println("Initializing core 0");
    xTaskCreatePinnedToCore(
        core0Loop,
        "core0Loop",
        10000,
        NULL,
        1,
        &communicationTask,
        0
    );
    if (!imuReady || !bluetoothReady) {
        Serial.println("ERROR - Initializing components failed");
    }
    else {
        Serial.println("Ready");
    }
}

void loop() {
    feedTheWatchdog();
    yield();

    if (!imuReady || !bluetoothReady) {
        Serial.println("Components did not initialize successfully");
        delay(10000);
        return;
    }
    if (IMU::isNewData()) {
        IMU::updateOrientation();
    }
    updateSensorData();
}
