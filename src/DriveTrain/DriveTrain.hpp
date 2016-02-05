#include "Arduino.h"
#include "../Utilities.h"
#include <Servo.h>

#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

enum DriveTrainInvertedSide { INVERTED_LEFT, INVERTED_RIGHT };

class DriveTrain {
  Servo leftMotor, rightMotor;
  DriveTrainInvertedSide invertedSide;

  static const int MOTOR_MAX_VALUE = 180;
  static const int MOTOR_MIN_VALUE = 0;

protected:
  int speedToMotorValue(float speed);
  void writeToMotors(int left, int right);

public:
  DriveTrain(int leftMotorPin, int rightMotorPin, DriveTrainInvertedSide inversion);
  void arcadeDrive(float speed, float rotation);
  void tankDrive(float left, float right);
};

#endif
