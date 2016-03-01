#include "BowlGrabber.hpp"

/* constructor:

 * motorPin - PWM for arm motor
 */
BowlGrabber::BowlGrabber(int motorPin) {
  motor = new Motor(motorPin, false);
}

/* update - Updates the Bowl Grabber's sensors and executes PID control
 *
 */
void BowlGrabber::update() {
  currentAngle = analogRead(PIN_SENSOR_ARM_POT);

  if (pidEnabled) {
    float error = setpointAngle - currentAngle;
    float speed = kP * error;

    speed = constrain(speed, -1, 1);

    writeToMotor(speed);
  }

}

void BowlGrabber::writeToMotor(float speed) {
  if ((currentAngle > maxAngle) && (speed < 0)) {
    motor->write(speed);
  } else if ((currentAngle < minAngle) && (speed > 0)) {
    motor->write(speed);
  } else if ((currentAngle < maxAngle) && (currentAngle > minAngle)) {
    motor->write(speed);
  } else {
    motor->write(0);
  }
}

/* enablePID - Enables PID control
 *
 */
void BowlGrabber::enablePID() {
  pidEnabled = true;
}

/* disablePID - Disables PID control
 *
 */
void BowlGrabber::disablePID() {
  pidEnabled = false;
}

bool16 BowlGrabber::getPIDEnabled() {
  return pidEnabled;
}

/* setAngle - Sets the Bowl Grabber's angle using PID
 *
 */
void BowlGrabber::setAngle(float angle) {
  setpointAngle = angle;
}

bool16 BowlGrabber::isAtSetpoint() {
  bool16 atSetpoint = (currentAngle < (setpointAngle * 1.2)) && (currentAngle > (setpointAngle * 0.8));
  Serial.println(atSetpoint);
  return atSetpoint;
}

/* up - Moves the bowl grabber up
 *
 * speed - the speed the motor moves up
 */
void BowlGrabber::up(float speed){
  //constrains the speed to the intended speed (0, 1)
  speed = constrain(speed, 0, 1);
  //sets the motor to the speed desired
  writeToMotor(speed);
}

/* down - Moves the bowl grabber down
 *
 * speed - the speed the motor moves down
 */
void BowlGrabber::down(float speed){
  //constrains the speed to the intended speed (0, 1)
  speed = constrain(speed, 0, 1);
  //sets the motor to the speed desired
  writeToMotor(-speed);
}

void BowlGrabber::lower() {
  enablePID();
  setAngle(horizAngle);
}

void BowlGrabber::raise() {
  enablePID();
  setAngle(vertAngle);
}

/* stop - Stops the motor
 *
 */
void BowlGrabber::stop(){
  motor->write(0);
}
