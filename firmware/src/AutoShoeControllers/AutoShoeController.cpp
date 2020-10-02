#include "../../include/AutoShoeControllers/AutoShoeController.h"

AutoShoeController::AutoShoeController(Sensors* sensors, int side)
{
    this->sensors = sensors;
    this->started = false;
    this->speedMultiplier = 1;
    this->side = side;
}

void AutoShoeController::start()
{
    started = true;
}

void AutoShoeController::stop()
{
    started = false;
}

void AutoShoeController::setSpeedMultiplier(float multipler)
{
    speedMultiplier = multipler;
}

float AutoShoeController::getSpeedMultiplier()
{
    return speedMultiplier;
}