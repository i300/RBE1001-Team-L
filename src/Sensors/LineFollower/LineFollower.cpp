#include "LineFollower.hpp"

/* constructor
 *
 * leftPin - Digital pin for the left sensor
 * leftPin - Digital pin for the right sensor
 */
LineFollower::LineFollower(int8 leftPin, int8 rightPin) {
  leftSensorPin = leftPin;
  rightSensorPin = rightPin;

  pinMode(leftSensorPin, INPUT);
  pinMode(rightSensorPin, INPUT);
}

/* readSensor - Reads a value directly from a sensor
 *
 * sensor - Flag to choose what sensor to read from
 */
bool16 LineFollower::readSensor(int8 sensor) {
  if (sensor == LEFT_SENSOR) {
    return digitalRead(leftSensorPin);
  } else if (sensor == RIGHT_SENSOR) {
    return digitalRead(rightSensorPin);
  } else {
    return false;
  }
}

/* read - Reads from both sensors and returns a useful value
 *
 * The sensors return 0 if they detect a line and
 * 1 if they don't detect a line
 */
LineFollowerReading LineFollower::read() {
  bool16 leftSensor = readSensor(LEFT_SENSOR) == 0;
  bool16 rightSensor = readSensor(RIGHT_SENSOR) == 0;

  if (!leftSensor && !rightSensor) {
    return DETECT_NONE;
  } else if (leftSensor && rightSensor) {
    return DETECT_BOTH;
  } else if (leftSensor) {
    return DETECT_LEFT;
  } else if (rightSensor) {
    return DETECT_RIGHT;
  } else {
    return DETECT_NONE;
  }
}
