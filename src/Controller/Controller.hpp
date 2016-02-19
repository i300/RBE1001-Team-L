#include "Arduino.h"
#include "../Utilities.h"
#include <DFW.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

enum CONTROLLER_JOYSTICK { JS_LEFT, JS_RIGHT };
enum CONTROLLER_BUTTON { BTN_ONE, BTN_TWO, BTN_THREE, BTN_FOUR,
                         BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT,
                         BTN_L1, BTN_L2, BTN_R1, BTN_R2,
                         BTN_START, BTN_SELECT };

class Controller {

  DFW *dfw;

public:
  Controller(int8 ledDebugPin);

  void update();

  float getX(CONTROLLER_JOYSTICK joystick);
  float getY(CONTROLLER_JOYSTICK joystick);
  bool16 getButton(CONTROLLER_BUTTON button);
};

#endif
