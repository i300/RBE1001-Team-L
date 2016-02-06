#include "Arduino.h"
#include "../Utilities.h"
#include <DFW.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

enum CONTROLLER_JOYSTICK { JS_LEFT, JS_RIGHT };

class Controller {

  DFW *dfw;

public:
  Controller(int8 ledDebugPin);

  void update();

  float getX(CONTROLLER_JOYSTICK joystick);
  float getY(CONTROLLER_JOYSTICK joystick);
};

#endif
