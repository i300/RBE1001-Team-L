#include "BowlGrabber.hpp"

/* constructor:

 * motorPin - PWM for arm motor
 */
BowlGrabber::BowlGrabber(int motorPin, int grabberPin) {
  motor = new Motor(motorPin, false);
  grabberServo.attach(grabberPin, 750, 2250);
}

/* update - Updates the Bowl Grabber's sensors and executes PID control
 *
 */
void BowlGrabber::update() {
  currentAngle = analogRead(PIN_SENSOR_ARM_POT);

  if (pidEnabled) {
    float error = setpointAngle - currentAngle;
    float speed = -kP * error;

    speed = constrain(speed, -1, 1);

    /*Serial.print("Error: " ); Serial.print(error);
    Serial.print("Angle: "); Serial.print(currentAngle);
    Serial.print(" | Speed: "); Serial.println(speed);*/

    writeToMotor(speed);
  }

}

void BowlGrabber::writeToMotor(float speed) {
  // If the motor is commanded to go up and it is greater than the bottom angle
  // it writes to the motor. If the motor is commanded to go down and it is
  // above the top angle, it writes to the motor. Otherwise, if within the safe
  // zone, it writes to the motor. If none of these are true it doesn't drive the motor.
  if ((currentAngle >= bottomAngle) && (speed > 0)) {
    motor->write(speed);
  } else if ((currentAngle <= topAngle) && (speed < 0)) {
    motor->write(speed);
  } else if ((currentAngle > topAngle) && (currentAngle < bottomAngle)) {
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

/* move - moves the bowl grabber with a speed
 *
 * speed - the speed (-1 to 1)
 */
void BowlGrabber::move(float speed) {
  speed = constrain(speed, -1, 1);

  writeToMotor(speed);
}

void BowlGrabber::lower() {
  enablePID();
  setAngle(downAngle);
}

void BowlGrabber::raise() {
  enablePID();
  setAngle(upAngle);
}

/* stop - Stops the motor
 *
 */
void BowlGrabber::stop(){
  motor->write(0);
}

/* openGrabber - opens the four bar's finger
 *
  */
void BowlGrabber::openGrabber(){
  grabberServo.write(grabberOpenAngle);
}

void BowlGrabber::moveGrabber(float angle) {
  int moveAngle = map(angle * 100, -100, 100, 0, 180);
  grabberServo.write(moveAngle);
}

/* closeGrabber - closes the four bar's finger
 *
 */
void BowlGrabber::closeGrabber(){
  grabberServo.write(grabberClosedAngle);
}
