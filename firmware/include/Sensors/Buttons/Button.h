#ifndef Button_h
#define Button_h

class Button
{
    public:
    virtual void initialize() = 0;
    virtual bool isPressed() = 0;
};

#endif