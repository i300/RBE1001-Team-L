#include "Arm.hpp"
/* constructor:

 * armMotorPin - PWM for arm motor
 */
Arm::Arm(int armMotorPin) {
  armMotor = new Motor(armMotorPin, false);
}

/* update - Updates the arm's sensors and executes PID control
 *
 */
void Arm::update() {
  currentAngle = analogRead(PIN_SENSOR_ARM_POT);
  Serial.println(currentAngle);

  if (pidEnabled) {
    float error = setpointAngle - currentAngle;
    float speed = kP * error;

    speed = constrain(speed, -1, 1);

    writeToMotor(speed);
  }

}

void Arm::writeToMotor(float speed) {
  if ((currentAngle > maxAngle) && (speed < 0)) {
    armMotor->write(speed);
  } else if ((currentAngle < minAngle) && (speed > 0)) {
    armMotor->write(speed);
  } else if ((currentAngle < maxAngle) && (currentAngle > minAngle)) {
    armMotor->write(speed);
  } else {
    armMotor->write(0);
  }
}

/* enablePID - Enables PID control
 *
 */
void Arm::enablePID() {
  pidEnabled = true;
}

/* disablePID - Disables PID control
 *
 */
void Arm::disablePID() {
  pidEnabled = false;
}

bool16 Arm::getPIDEnabled() {
  return pidEnabled;
}

/* setAngle - Sets the Arm's angle using PID
 *
 */
void Arm::setAngle(float angle) {
  setpointAngle = angle;
}

bool16 Arm::isAtSetpoint() {
  bool16 atSetpoint = (currentAngle < (setpointAngle * 1.2)) && (currentAngle > (setpointAngle * 0.8));
  Serial.println(atSetpoint);
  return atSetpoint;
}

/* up
 *
 * speed - the speed the motor moves up
 */
void Arm::up(float speed){
  //constrains the speed to the intended speed (0, 1)
  speed = constrain(speed, 0, 1);
  //sets the motor to the speed desired
  writeToMotor(speed);
}

/* down
 *
 * speed - the speed the motor moves down
 */
void Arm::down(float speed){
  //constrains the speed to the intended speed (0, 1)
  speed = constrain(speed, 0, 1);
  //sets the motor to the speed desired
  writeToMotor(-speed);
}

void Arm::lower() {
  enablePID();
  setAngle(horizAngle);
}

void Arm::raise() {
  enablePID();
  setAngle(vertAngle);
}

/* stop
 *
 */
void Arm::stop(){
  armMotor->write(0);
}
