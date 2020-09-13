#include <Arduino.h>
#include <Preferences.h>
#include "../include/VrShoeConfiguration.h"
#include "../include/Timer.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

TaskHandle_t communicationTask;
VrShoeConfiguration config;
Preferences preferences;
const int LOOP_TIMOUT = 30;
Timer loopTimer(LOOP_TIMOUT);

void communicationLoop(void * parameters)
{
  Timer timer = Timer(LOOP_TIMOUT);
  while(true)
  {
    timer.start();
    config.getCommunicator()->update();
    if(timer.timeIsUp())
    {
      Serial.print("Communication loop timout. Loop took ");
      Serial.print(timer.getElapsedTime());
      Serial.println("ms");
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(921600, SERIAL_8N1, 26, 27);
  Serial2.begin(921600, SERIAL_8N1, 16, 17);
  preferences.begin("VR-Shoe");
  config.initialize(preferences);

  xTaskCreatePinnedToCore(
    communicationLoop,
    "communicationLoop",
    10000,
    NULL,
    1,
    &communicationTask,
    0);                                 
}

void loop() 
{
  TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed=1;
  TIMERG0.wdt_wprotect=0;

  loopTimer.start();
  config.getSensors()->updateSensors();
  // if(loopTimer.timeIsUp())
  // {
  //   Serial.print("Main loop timeout. Loop took ");
  //   Serial.print(loopTimer.getElapsedTime());
  //   Serial.println("ms");
  // }
}
