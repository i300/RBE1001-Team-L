#include "CubeFlipper.hpp"

/* constructor
 *
 * motorPin - pin of the motor
 */
CubeFlipper::CubeFlipper(int8 motorPin) {
  motor = new Motor(motorPin, true);
}

void CubeFlipper::writeToMotor(float speed) {
  speed = constrain(speed, -1, 1);

  motor->write(speed);
}

void CubeFlipper::up() {
  writeToMotor(1);
}
void CubeFlipper::down() {
  writeToMotor(-1);
}
void CubeFlipper::move(float speed) {
  writeToMotor(speed);
}
void CubeFlipper::stop() {
  writeToMotor(0);
}
