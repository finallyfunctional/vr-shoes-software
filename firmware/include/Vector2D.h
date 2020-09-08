#ifndef Vector2D_h
#define Vector2D_h

class Vector2D
{
    public:
    Vector2D(int x, int y);
    int getX();
    int getY();

    private:
    int x;
    int y;
};

#endif