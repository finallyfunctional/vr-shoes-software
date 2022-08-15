#include <Arduino.h>

class Encoder {
    public:
     static void initialize(int pinA, int pinB);
     static void pinAInterrupt();
     static void pinBInterrupt();
     static int ticks;

    private:
     static int pinA, pinB;
};