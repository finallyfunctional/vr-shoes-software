#include "../../include/AutoShoeControllers/AutoShoeController.h"

const char* AutoShoeController::SPEED_MULTIPLIER_KEY = "spm";

AutoShoeController::AutoShoeController(Sensors* sensors, int side)
{
    this->sensors = sensors;
    strideTracker = new StrideTracker(sensors->getMovementTracker());
    this->started = false;
    this->speedMultiplier = VrShoePreferences.getFloat(SPEED_MULTIPLIER_KEY, 1);
    this->side = side;
    this->movementState = ShoeMovementState::STOPPED;
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
    VrShoePreferences.putFloat(SPEED_MULTIPLIER_KEY, multipler);
}

float AutoShoeController::getSpeedMultiplier()
{
    return speedMultiplier;
}

int AutoShoeController::getMovementState()
{
    return movementState;
}

StrideTracker* AutoShoeController::getStrideTracker()
{
    return strideTracker;
}