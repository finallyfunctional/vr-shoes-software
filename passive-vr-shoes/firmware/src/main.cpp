#include <Arduino.h>
#include "Wire.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "imu.h"
#include "encoder.h"
#include "vrShoeSensorData.h"
#include "sensorDataMessenger.h"


#define ENCODER_A 16
#define ENCODER_B 17

bool core0TaskInitialized = false;
bool imuReady = false;
VrShoeSensorData data;
TaskHandle_t communicationTask;
SemaphoreHandle_t dataSemaphore;

void feedTheWatchdog() {
    TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
    TIMERG0.wdt_feed=1;
    TIMERG0.wdt_wprotect=0;
    yield();
}

void updateSensorData() {
    xSemaphoreTake(dataSemaphore, portMAX_DELAY );
    data.encoderTicks = Encoder::ticks;
    data.qw = IMU::orientation.w;
    data.qx = IMU::orientation.x;
    data.qy = IMU::orientation.y;
    data.qz = IMU::orientation.z;
    xSemaphoreGive(dataSemaphore);
}

void core0Loop(void * parameters)
{
    Serial.println("Starting core 0 loop");
    while(true) {
        yield();
        xSemaphoreTake(dataSemaphore, portMAX_DELAY );
        xSemaphoreGive(dataSemaphore);
    }
}

void initialzeCore0Task() {
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
    core0TaskInitialized = true;
}

void setup() {
    Wire.begin();

    Serial.begin(115200);
    dataSemaphore = xSemaphoreCreateMutex();
    Serial.println("Initializing components");
    Encoder::initialize(ENCODER_A, ENCODER_B);
    imuReady = IMU::initialize();
    // if(!IMU::calibrate()) {
    //     imuReady = false;
    // }
    if (!imuReady) {
        Serial.println("ERROR - Initializing componentas failed");
    }
    else {
        Serial.println("Ready");
    }

    Wire.setClock(400000);
}

void loop() {
    if(core0TaskInitialized) {
        feedTheWatchdog();
    }
    if (!imuReady) {
        Serial.println("Components did not initialize successfully");
        delay(10000);
        return;
    }
    IMU::updateOrientation();
    updateSensorData();
    //SensorDataMessenger::serialPrintSensorData(data);
    IMU::printQuaternion();
}