#ifndef Button_h
#define Button_h

class Button
{
    public:
    virtual void initialize() = 0;
    virtual bool isPressed() = 0;
    virtual void setPressedValue() = 0;
    virtual int getPressedValue() = 0;
    virtual void setMaxDifferencePercentage(int diffPercentage) = 0;
    virtual int getMaxDifferencePercentage() = 0;

    static const char* FRONT_BUTTON_PRESSED_VALUE_KEY;
    static const char* REAR_BUTTON_PRESSED_VALUE_KEY;
    static const char* MAX_DIFF_VALUE_KEY;
};

#endif