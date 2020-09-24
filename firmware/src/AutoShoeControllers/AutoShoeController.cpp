#include "../../include/AutoShoeControllers/AutoShoeController.h"

AutoShoeController::AutoShoeController(Sensors* sensors)
{
    this->sensors = sensors;
    this->started = false;
}

void AutoShoeController::start()
{
    started = true;
}

void AutoShoeController::stop()
{
    started = false;
}