#include "Arduino.h"
#include "../../Utilities.h"

#include <SharpIR.h>

#ifndef RANGEFINDER_H
#define RANGEFINDER_H

#define RANGEFINDER_TYPE 1080 // Model number for 2Y0A21 sensor

class Rangefinder {
  SharpIR *ir;

public:
  Rangefinder(int8 sensorPin);
  int16 read();
};

#endif
