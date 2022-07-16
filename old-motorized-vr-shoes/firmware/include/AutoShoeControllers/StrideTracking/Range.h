#ifndef Range_h
#define Range_h

class Range
{
    public:
    void setRange(float min, float max);

    /* inclusive */
    bool inRange(float number);
    bool belowMinimum(float number);
    bool aboveMaximum(float number);

    private:
    float min, max;
};

#endif