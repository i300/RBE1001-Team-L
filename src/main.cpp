#include "Arduino.h"

#include "RobotMap.h"
#include "Utilities.h"

#include "Controller/Controller.hpp"
#include "Subsystems/DriveTrain/DriveTrain.hpp"
#include "Sensors/LineFollower/LineFollower.hpp"

// Controller
Controller *controller;

// Subsystems
DriveTrain *driveTrain;
LineFollower *lineFollower;

// Game variables
uint16 lastLoopTime = 0;
uint16 gameStartTime = 0;

void setup() {
  Serial.begin(9600);

  // Initialize Controller
  controller = new Controller(PIN_LED_DEBUG);

  // Initialize Subsystems
  driveTrain = new DriveTrain(PIN_MOTOR_LEFT, PIN_MOTOR_RIGHT, INVERTED_RIGHT);
  lineFollower = new LineFollower(PIN_SENSOR_LINEFOLLOWER_LEFT, PIN_SENSOR_LINEFOLLOWER_RIGHT);
}

void autonomous() {
  LineFollowerReading lineFollowStatus = lineFollower->read();

  // Drive left if the left sensor is triggered,
  // drive right if the right sensor is triggered,
  // and drive straight if neither are triggered
  if (lineFollowStatus == DETECT_LEFT) {
    driveTrain->arcadeDrive(5/8, -0.5);
  } else if (lineFollowStatus == DETECT_RIGHT) {
    driveTrain->arcadeDrive(5/8, 0.5);
  } else if (lineFollowStatus == DETECT_NONE) {
    driveTrain->arcadeDrive(0.625, 0);
  }
}

void teleop() {
  //driveTrain->tankDrive(controller->getY(JS_LEFT), controller->getY(JS_RIGHT));
  driveTrain->arcadeDrive(controller->getY(JS_LEFT), controller->getX(JS_RIGHT));
}

void loop() {
  uint16 currentTime = millis();

  // Time between loops
  uint16 dt = currentTime - lastLoopTime;
  lastLoopTime = currentTime;

  // Time robot running
  uint16 timeRunning = currentTime - gameStartTime;

  Serial.print("dt: "); Serial.println(dt);

  // Run for (10^10) seconds in autonomous, then enter into teleop
  if (timeRunning <= pow(10, 10)) {
    autonomous();
  } else {
    controller->update();

    teleop();
  }

  // Delay 20ms for dfw and servo safety
  delay(20);
}
