#include "Arduino.h"

#include <DFW.h>
#include "Utilities.h"
#include "RobotMap.h"
#include "Subsystems/DriveTrain/DriveTrain.hpp"
#include "Sensors/LineFollower/LineFollower.hpp"

// DFW
DFW *dfw;

// Subsystems
DriveTrain *driveTrain;
LineFollower *lineFollower;

// Game variables
uint16 gameStartTime = 0;

void setup() {
  // Initialize DFW
  dfw = new DFW(PIN_LED_DEBUG);
  dfw->begin(9600, 1);
  dfw->update();
  while (dfw->start() == 0) {
    Serial.println("DFW Initialized");
    dfw->update();
    delay(20);
  }

  driveTrain = new DriveTrain(PIN_MOTOR_LEFT, PIN_MOTOR_RIGHT, INVERTED_LEFT);
  lineFollower = new LineFollower(PIN_SENSOR_LINEFOLLOWER_LEFT, PIN_SENSOR_LINEFOLLOWER_LEFT);
}

void autonomous() {
  LineFollowerReading lineFollowStatus = lineFollower->read();

  if (lineFollowStatus == DETECT_LEFT) {
    driveTrain->arcadeDrive(0.5, -1);
  } else if (lineFollowStatus == DETECT_RIGHT) {
    driveTrain->arcadeDrive(0.5, 1);
  }
}

void teleop() {
  dfw->update();
  driveTrain->tankDrive(dfw->joysticklv(), dfw->joystickrv());
}

void loop() {
  uint16 timeRunning = gameStartTime - millis();

  // Run for 20 seconds in autonomous, then enter into teleop
  if (timeRunning <= 20000) {
    autonomous();
  } else {
    teleop();
  }

  // Delay 20ms for dfw and servo safety
  delay(20);
}
