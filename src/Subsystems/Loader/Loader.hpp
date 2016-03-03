#include "Arduino.h"
#include "../../RobotMap.h"
#include "../../Utilities.h"
#include "../../Motor/Motor.hpp"

#ifndef LOADER_H
#define LOADER_H

class Loader {
  Motor *motor;

public:
  Loader(int loaderPin);

  void load();
  void unload();
  void stop();
};

#endif
