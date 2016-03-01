#include "Arduino.h"
#include "../../RobotMap.h"
#include "../../Utilities.h"
#include "../../Motor/Motor.hpp"

#ifndef BOWLGRABBER_H
#define BOWLGRABBER_H

class BowlGrabber {
  Motor *motor;

  const float kP = 0.0075;

  const int16 minAngle = 250;
  const int16 maxAngle = 850;

  bool16 pidEnabled = false;
  float currentAngle = 0;
  float setpointAngle = 0;

protected:
  void writeToMotor(float speed);

public:
  BowlGrabber(int motorPin);

  const int16 horizAngle = 300;
  const int16 vertAngle = 725;

  void update();

  void enablePID();
  void disablePID();
  bool16 getPIDEnabled();
  void setAngle(float angle);
  bool16 isAtSetpoint();

  void up(float speed);
  void down(float speed);
  void lower();
  void raise();
  void stop();
};

#endif
