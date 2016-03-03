#include "Arduino.h"
#include "../../RobotMap.h"
#include "../../Utilities.h"
#include "../../Motor/Motor.hpp"

#ifndef LOADER_H
#define LOADER_H

class Loader {
  Motor *motor;

  int8 switchPin;

public:
  Loader(int8 loaderPin, int8 foamSwitchPin);

  bool16 isFoamLoaded();

  void load();
  void unload();
  void stop();
};

#endif
