#ifndef ArrayUtils_h
#define ArrayUtils_h

#include<Arduino.h>

class ArrayUtils {
    public:
      static void setDefaultValues(int array[], int arraySize, int defaultValue) {
          for(int i = 0; i < arraySize; i++) {
              array[i] = defaultValue;
          }
      }
};

#endif