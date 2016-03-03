#include "Arduino.h"
#include <Servo.h>
#include "../../RobotMap.h"
#include "../../Utilities.h"
#include "../../Motor/Motor.hpp"

#ifndef BOWLGRABBER_H
#define BOWLGRABBER_H

class BowlGrabber {
  Motor *motor;
  Servo grabberServo;

  const float kP = 0.01;

  const int16 topAngle = 830;
  const int16 bottomAngle = 950;

  const int8 grabberOpenAngle = 100;
  const int8 grabberClosedAngle = 180;

  bool16 pidEnabled = false;
  float currentAngle = 0;
  float setpointAngle = 0;

protected:
  void writeToMotor(float speed);

public:
  BowlGrabber(int motorPin, int grabberPin);

  const int16 upAngle = 880;
  const int16 downAngle = 950;

  void update();

  void enablePID();
  void disablePID();
  bool16 getPIDEnabled();
  void setAngle(float angle);
  bool16 isAtSetpoint();

  void up(float speed);
  void down(float speed);
  void move(float speed);
  void lower();
  void raise();
  void stop();

  void openGrabber();
  void moveGrabber(float angle);
  void closeGrabber();
};

#endif
