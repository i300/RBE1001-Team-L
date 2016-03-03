#include "Rangefinder.hpp"

Rangefinder::Rangefinder(int8 sensorPin) {
  ir = new SharpIR(sensorPin, RANGEFINDER_TYPE);
}

int16 Rangefinder::read() {
  return ir->distance();
}
