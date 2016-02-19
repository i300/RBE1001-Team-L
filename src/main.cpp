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
uint16 framesSeenPerpendicularLine = 0;

bool16 autoCompleted = false;

bool16 armInitiallyRaised = false;

uint8 perpendicularLinesDetected = 0;
uint8 timeLastSeenLine = 0;
bool16 onPerpendicularLine = false;

float autoDriveSpeed = 0.8;

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
  arm->update();

  // If arm isn't raised initially, raise it and then end this loop
  if (!armInitiallyRaised) {
    gameStartTime = millis();

    arm->raise();
    armInitiallyRaised = true;

    return;
  }

  // Let the robot raise the arm for 500ms before driving
  if (timeRunning < 500) {
    return;
  } else if (timeRunning >= 8000 && timeRunning <= 8500) {
    autoDriveSpeed = 0.5;
  }

  // Drive left if the left sensor is triggered,
  // drive right if the right sensor is triggered,
  // and drive straight if neither are triggered
  if (lineFollowStatus == DETECT_BOTH) {
    framesSeenPerpendicularLine++;

    if ((timeRunning >= 8850) && (framesSeenPerpendicularLine > 2)) {
      if (onPerpendicularLine == false) {
        onPerpendicularLine = true;

        if (millis() - timeLastSeenLine > 2000) {
          if ((timeRunning >= 8850) && (timeRunning <= 11500)) {
            perpendicularLinesDetected += 1;
            timeLastSeenLine = millis();
          } else if (timeRunning >= 15000) {
            perpendicularLinesDetected += 1;
            timeLastSeenLine = millis();
          }
        }

        if (perpendicularLinesDetected == 1) {
          arm->lower();
          autoDriveSpeed = 0;
          driveTrain->arcadeDrive(0, 0);
        } else if (perpendicularLinesDetected == 2) {
          autoCompleted = true;
          autoDriveSpeed = 0;
          driveTrain->arcadeDrive(0, 0);
        }
      } else {
        if (timeRunning >= 11500) {
          autoDriveSpeed = 0.70;
          driveTrain->arcadeDrive(autoDriveSpeed, 0);
        } else {
          driveTrain->arcadeDrive(0, 0);
        }
      }
    }
  } else if (lineFollowStatus == DETECT_LEFT) {
    if (millis() - timeLastSeenLine > 500) {
      if (autoDriveSpeed == 0) driveTrain->arcadeDrive(0, 0);
      else driveTrain->arcadeDrive(autoDriveSpeed, 1);
    }

    delay(25);

    onPerpendicularLine = false;
    framesSeenPerpendicularLine = 0;
  } else if (lineFollowStatus == DETECT_RIGHT) {
    if (millis() - timeLastSeenLine > 500) {
      if (autoDriveSpeed == 0) driveTrain->arcadeDrive(0, 0);
      else driveTrain->arcadeDrive(autoDriveSpeed, -1);
    }

    delay(25);

    onPerpendicularLine = false;
    framesSeenPerpendicularLine = 0;
  } else if (lineFollowStatus == DETECT_NONE) {
    driveTrain->arcadeDrive(autoDriveSpeed, 0);

    onPerpendicularLine = false;
    framesSeenPerpendicularLine = 0;
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

  //Serial.print("dt: "); Serial.println(dt);

  // Run for 0 seconds in autonomous, then enter into teleop
  if (!autoCompleted) {
    autonomous(timeRunning);
  } else {
    /*controller->update();

    teleop();*/

    driveTrain->arcadeDrive(0, 0);
  }

  delay(20);
}
