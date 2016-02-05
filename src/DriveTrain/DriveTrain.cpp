#include "DriveTrain.hpp"

/* constructor
 *
 * leftMotorPin - pwm pin for left motor
 * rightMotorPin - pwm pin for right motor
 * inversion - which side of the robot is inverted
 */
DriveTrain::DriveTrain(int leftMotorPin, int rightMotorPin, DriveTrainInvertedSide inversion) {
  leftMotor.attach(leftMotorPin, 1000, 2000);
  rightMotor.attach(rightMotorPin, 1000, 2000);

  invertedSide = inversion;
}

/* speedToMotorValue - converts a speed into a motor value
 *
 * speed - speed (-1 to 1)
 */
int DriveTrain::speedToMotorValue(float speed) {
  int realSpeed = speed * 100;
  return (int)map(realSpeed, -100, 100, MOTOR_MIN_VALUE, MOTOR_MAX_VALUE);
}

/* writeToMotors
 *
 * left - left motor value
 * right - right motor value
 */
void DriveTrain::writeToMotors(int left, int right) {
  if (invertedSide == INVERTED_LEFT) {
    leftMotor.write(MOTOR_MAX_VALUE - left);
    rightMotor.write(right);
  }

  if (invertedSide == INVERTED_RIGHT) {
    leftMotor.write(MOTOR_MAX_VALUE - left);
    rightMotor.write(right);
  }
}

/* arcadeDrive - drives robot based on a forward speed and rotation value
 *
 * speed - speed robot will drive at
 * rotation - value from -1 (left) to 1 (right)
 */
void DriveTrain::arcadeDrive(float speed, float rotation) {
  // constrain speed and roation to intended values (-1 to 1)
  speed = constrain(speed, -1, 1);
  rotation = constrain(rotation, -1, 1);

  float leftMotorSpeed, rightMotorSpeed;

  // credit to Joel Gallant
  // https://gist.github.com/joelg236/a294a03a1094167ff49f
  if (speed > 0.0) {
    if (rotation > 0.0) {
      leftMotorSpeed = speed - rotation;
      rightMotorSpeed = max(speed, rotation);
    } else {
      leftMotorSpeed = max(speed, -rotation);
      rightMotorSpeed = speed + rotation;
    }
  } else {
    if (rotation > 0.0) {
      leftMotorSpeed = -max(-speed, rotation);
      rightMotorSpeed = speed + rotation;
    } else {
      leftMotorSpeed = speed - rotation;
      rightMotorSpeed = -max(-speed, -rotation);
    }
  }

  // map calculated speeds to motor values
  float leftMotorValue = (float)speedToMotorValue(leftMotorSpeed);
  float rightMotorValue = (float)speedToMotorValue(rightMotorSpeed);

  // drive robot
  writeToMotors(leftMotorValue, rightMotorValue);
}

/* tankDrive - drive robot based on left values and right speeds
 *
 * left - left speed
 * right - right speed
 */
void DriveTrain::tankDrive(float left, float right) {
  // constrain speeds to intended values (-1 to 1)
  left = constrain(left, -1, 1);
  right = constrain(right, -1, 1);

  // convert to motor values and write to motors
  writeToMotors(speedToMotorValue(left), speedToMotorValue(right));
}
