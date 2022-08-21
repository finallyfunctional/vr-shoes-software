#ifndef FloatUtils_h
#define FloatUtils_h

#include<Arduino.h>

class FloatUtils {
    public:
      static bool equals(float num1, float num2, float epsilon = 0.001f) {
          return fabs(num1 - num2) < epsilon;
      }

      static float roundFloatToTwoDecimalPlaces(float number) {
          return (float)(roundf(number * 100) / 100);
      }

      static bool inRangeInclusive(float num, float min, float max) {
          return num >= min && num <= max;
      }
};

#endif