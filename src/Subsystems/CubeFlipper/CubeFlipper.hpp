#include "Arduino.h"
#include "../../Motor/Motor.hpp"
#include "../../Utilities.h"

#ifndef CUBEFLIPPER_H
#define CUBEFLIPPER_H

class CubeFlipper {
  Motor *motor;

protected:
  void writeToMotor(float speed);

public:
  CubeFlipper(int8 motorPin);
  void up();
  void down();
  void move(float speed);
  void stop();
};

#endif
