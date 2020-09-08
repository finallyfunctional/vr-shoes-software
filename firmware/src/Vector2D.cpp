#include "../include/Vector2D.h"

Vector2D::Vector2D(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Vector2D::getX()
{
    return x;
}

int Vector2D::getY()
{
    return y;
}