#include "../include/Vector2D.h"

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

float Vector2D::getX()
{
    return x;
}

float Vector2D::getY()
{
    return y;
}