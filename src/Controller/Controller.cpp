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
    Serial.println("DFW Initializing");
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

/* getButton
 *
 * button- the button on the controller
 */
bool16 Controller::getButton(CONTROLLER_BUTTON button) {
  int8 buttonPressed = 0;

  if (button == BTN_ONE) {
    buttonPressed = dfw->one();
  } else if (button == BTN_TWO) {
    buttonPressed = dfw->two();
  } else if (button == BTN_THREE) {
    buttonPressed = dfw->three();
  } else if (button == BTN_FOUR) {
    buttonPressed = dfw->four();
  } else if (button == BTN_UP) {
    buttonPressed = dfw->up();
  } else if (button == BTN_DOWN) {
    buttonPressed = dfw->down();
  } else if (button == BTN_LEFT) {
    buttonPressed = dfw->left();
  } else if (button == BTN_RIGHT) {
    buttonPressed = dfw->right();
  } else if (button == BTN_L1) {
    buttonPressed = dfw->l1();
  } else if (button == BTN_L2) {
    buttonPressed = dfw->l2();
  } else if (button == BTN_R1) {
    buttonPressed = dfw->r1();
  } else if (button == BTN_R2) {
    buttonPressed = dfw->r2();
  } else if (button == BTN_START) {
    buttonPressed = dfw->start();
  } else if (button == BTN_SELECT) {
    buttonPressed = dfw->select();
  }

  // button is pressed if it returns 0
  return buttonPressed == 0;
}
