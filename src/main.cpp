#include "Arduino.h"

#include "RobotMap.h"
#include "Utilities.h"

#include "Controller/Controller.hpp"
#include "Subsystems/DriveTrain/DriveTrain.hpp"
#include "Subsystems/Arm/Arm.hpp"
#include "Sensors/LineFollower/LineFollower.hpp"

// Controller
Controller *controller;

// Subsystems
DriveTrain *driveTrain;
Arm *arm;
LineFollower *lineFollower;

// Game variables
uint16 lastLoopTime = 0;
uint16 gameStartTime = 0;

bool16 autoCompleted = false;

const float autoDriveSpeed = 0.8;

void setup() {
  Serial.begin(9600);

  delay(100);

  // Initialize Controller
  controller = new Controller(PIN_LED_DEBUG);

  // Initialize Subsystems
  driveTrain = new DriveTrain(PIN_MOTOR_LEFT, PIN_MOTOR_RIGHT, INVERTED_RIGHT);
  arm = new Arm(PIN_MOTOR_ARM);
  lineFollower = new LineFollower(PIN_SENSOR_LINEFOLLOWER_LEFT, PIN_SENSOR_LINEFOLLOWER_RIGHT);
}

void autonomous(uint16 timeRunning) {
  LineFollowerReading lineFollowStatus = lineFollower->read();

  // Drive left if the left sensor is triggered,
  // drive right if the right sensor is triggered,
  // and drive straight if neither are triggered
  if (lineFollowStatus == DETECT_BOTH) {
    driveTrain->arcadeDrive(0, 0);
  } else if (lineFollowStatus == DETECT_LEFT) {
    driveTrain->arcadeDrive(autoDriveSpeed, 1);

    delay(25);
  } else if (lineFollowStatus == DETECT_RIGHT) {
    driveTrain->arcadeDrive(autoDriveSpeed, -1);

    delay(25);
  } else if (lineFollowStatus == DETECT_NONE) {
    driveTrain->arcadeDrive(autoDriveSpeed, 0);
  }
}

void teleop() {
  driveTrain->arcadeDrive(controller->getY(JS_LEFT), controller->getX(JS_RIGHT));
  arm->update();

  if (!arm->getPIDEnabled()) {
    if (controller->getButton(BTN_UP)) {
      arm->up(0.5);
    } else if (controller->getButton(BTN_DOWN)) {
      arm->down(0.5);
    } else {
      arm->stop();
    }
  }

  if (controller->getButton(BTN_TWO)) {
    arm->enablePID();
    arm->setAngle(arm->horizAngle);
  } else if (controller->getButton(BTN_ONE)) {
    arm->enablePID();
    arm->setAngle(arm->vertAngle);
  } else if (controller->getButton(BTN_THREE)) {
    arm->disablePID();
  }
}

void loop() {
  uint16 currentTime = millis();

  // Time between loops
  uint16 dt = currentTime - lastLoopTime;
  lastLoopTime = currentTime;

  // Time robot running
  uint16 timeRunning = currentTime - gameStartTime;

  Serial.print("dt: "); Serial.println(dt);

  // Run for 0 seconds in autonomous, then enter into teleop
  if (!autoCompleted) {
    autonomous(timeRunning);
  } else {
    controller->update();

    teleop();
  }

  delay(20);
}
