#include <Arduino.h>
#include <Preferences.h>
#include "../include/VrShoeGlobals.h"
#include "../include/VrShoeConfiguration.h"
#include "../include/Timer.h"
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"

TaskHandle_t communicationTask;
VrShoeConfiguration config;
Preferences VrShoePreferences;
Timer loopTimer(50);

void core0Loop(void * parameters)
{
  Timer timer = Timer(30);
  while(true)
  {
    timer.start();
    config.getCommunicator()->update();
    config.getShoeController()->update();
    if(timer.timeIsUp())
    {
      Serial.print("Core 0 loop timout. Loop took ");
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
  VrShoePreferences.begin("VR-Shoe");
  config.initialize();

  xTaskCreatePinnedToCore(
    core0Loop,
    "core0Loop",
    10000,
    NULL,
    1,
    &communicationTask,
    0);                                 
}

int prevSensor1Value;
int prevSensor2Value;

void loop() 
{
  TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed=1;
  TIMERG0.wdt_wprotect=0;
  
  yield();
  loopTimer.start();
  config.getSensors()->updateSensors();
  if(loopTimer.timeIsUp())
  {
    Serial.print("Main loop timeout. Loop took ");
    Serial.print(loopTimer.getElapsedTime());
    Serial.println("ms");
  }
}