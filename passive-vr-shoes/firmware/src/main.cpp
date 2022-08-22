#include <Arduino.h>
#include "Wire.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
#include "imu.h"
#include "encoder.h"
#include "vrShoeSensorData.h"
#include "sensorDataMessenger.h"
#include "btSerial.h"
#include "logger.h"

#define ENCODER_A 16
#define ENCODER_B 17

bool core0TaskInitialized = false;
bool imuReady = false;
Logger logger;
BtSerial bt;
IMU imu;
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
    data.qw = imu.orientation.w;
    data.qx = imu.orientation.x;
    data.qy = imu.orientation.y;
    data.qz = imu.orientation.z;
    xSemaphoreGive(dataSemaphore);
}

void core0Loop(void * parameters)
{
    logger.println("Starting core 0 loop");
    unsigned long time = 0;
    while(true) {
        yield();
        if(time + 50 > millis()) {
            xSemaphoreTake(dataSemaphore, portMAX_DELAY );
            xSemaphoreGive(dataSemaphore);
            time = millis();
        }
    }
}

void initialzeCore0Task() {
    logger.println("Initializing core 0");
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
    bt.initialize();
    logger.setBluetooth(bt);
    logger.logTo(true, true);
    dataSemaphore = xSemaphoreCreateMutex();
    logger.println("Initializing components");
    Encoder::initialize(ENCODER_A, ENCODER_B);
    imuReady = imu.initialize(logger);
    Wire.setClock(400000);
    //initialzeCore0Task();
    if (!imuReady) {
        logger.println("ERROR - Initializing componentas failed");
    }
    else {
        logger.println("Ready");
        logger.logTo(false, true);
    }    
}

void loop() {
    if(core0TaskInitialized) {
        feedTheWatchdog();
    }
    if (!imuReady) {
        logger.println("Components did not initialize successfully");
        delay(10000);
        return;
    }
    imu.updateOrientation();
    updateSensorData();
    imu.printQuaternion();
}