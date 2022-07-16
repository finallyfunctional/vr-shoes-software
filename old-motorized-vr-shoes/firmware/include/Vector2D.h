#ifndef Vector2D_h
#define Vector2D_h

class Vector2D
{
    public:
    Vector2D(float x, float y);
    float getX();
    float getY();

    private:
    float x;
    float y;
};

#endif