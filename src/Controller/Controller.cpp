#include "Controller.hpp"

/* Constructor
 *
 * ledDebugPin - Debug pin for DFW
 */
Controller::Controller(int8 ledDebugPin) {
  dfw = new DFW(ledDebugPin);
  dfw->begin(9600, 1);
  dfw->update();
  while (dfw->start() == 0) {
    Serial.println("DFW Initialized");
    dfw->update();
    delay(20);
  }
}

/* update
 *
 * Updates the DFW controller
 */
void Controller::update() {
  dfw->update();
}

/* getX - gets the X axis on a joystick
 *
 * joystick - Left or Right joystick
 */
float Controller::getX(CONTROLLER_JOYSTICK joystick) {
  if (joystick == JS_LEFT) {
    return map(dfw->joysticklh(), 0, 180, -100, 100) / 100.0;
  } else {
    return map(dfw->joystickrh(), 0, 180, -100, 100) / 100.0;
  }
}

/* getY - gets the Y axis on a joystick
 *
 * joystick - Left or Right joystick
 */
float Controller::getY(CONTROLLER_JOYSTICK joystick) {
  if (joystick == JS_LEFT) {
    return map(dfw->joysticklv(), 0, 180, -100, 100) / 100.0;
  } else {
    return map(dfw->joystickrv(), 0, 180, -100, 100) / 100.0;
  }
}
