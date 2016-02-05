#include "Arduino.h"
#include "../../Utilities.h"

#ifndef LINEFOLLOWER_H
#define LINEFOLLOWER_H

enum LineFollowerReading { DETECT_LEFT, DETECT_RIGHT, DETECT_NONE };

class LineFollower {
  int8 leftSensorPin, rightSensorPin;
  
  static const int8 LEFT_SENSOR = (1 << 0);
  static const int8 RIGHT_SENSOR = (1 << 1);

protected:
  bool16 readSensor(int8 sensor);

public:
  LineFollower(int8 leftPin, int8 rightPin);
  LineFollowerReading read();
};

#endif
