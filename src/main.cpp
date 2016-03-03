#include "Arduino.h"

#include "RobotMap.h"
#include "Utilities.h"

#include "Controller/Controller.hpp"
#include "Subsystems/DriveTrain/DriveTrain.hpp"
#include "Subsystems/BowlGrabber/BowlGrabber.hpp"
#include "Subsystems/Loader/Loader.hpp"
#include "Sensors/LineFollower/LineFollower.hpp"
#include "Sensors/Rangefinder/Rangefinder.hpp"

// Controller
Controller *controller;

// Subsystems
DriveTrain *driveTrain;
BowlGrabber *bowlGrabber;
Loader *loader;

// Sensors
LineFollower *lineFollower;
Rangefinder *rangefinder;

bool16 autoCompleted = false;

const float autoDriveSpeed = 0.8;

void setup() {
  Serial.begin(9600);

  delay(100);

  pinMode(22, INPUT_PULLUP);

  // Initialize Controller
  controller = new Controller(PIN_LED_DEBUG);

  // Initialize Subsystems
  driveTrain = new DriveTrain(PIN_MOTOR_LEFT, PIN_MOTOR_RIGHT, INVERTED_RIGHT);
  bowlGrabber = new BowlGrabber(PIN_MOTOR_BOWLGRABBER, PIN_SERVO_GRABBER);
  loader = new Loader(PIN_MOTOR_FOAMLOADER);

  // Initialize Sensors
  lineFollower = new LineFollower(PIN_SENSOR_LINEFOLLOWER_LEFT, PIN_SENSOR_LINEFOLLOWER_RIGHT);
  rangefinder = new Rangefinder(PIN_SENSOR_RANGEFINDER);
}

void autonomousFunction() {
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

void teleopFunction() {
  driveTrain->arcadeDrive(controller->getY(JS_LEFT), controller->getX(JS_RIGHT));
  bowlGrabber->update();

  if (!bowlGrabber->getPIDEnabled()) {
    if (controller->getButton(BTN_UP)) {
      bowlGrabber->up(1.0);
    } else if (controller->getButton(BTN_DOWN)) {
      bowlGrabber->down(1.0);
    } else {
      bowlGrabber->stop();
    }
  }

  if (controller->getButton(BTN_L1)) {
    bowlGrabber->openGrabber();
  } else if (controller->getButton(BTN_R1)) {
    bowlGrabber->closeGrabber();
  }

  // Load sequence
  if (controller->getButton(BTN_FOUR)) {
    driveTrain->stop();

    while (digitalRead(PIN_SENSOR_LOADER_LIMIT) != 1) {
      loader->load();

      delay(20);
    }

    bowlGrabber->disablePID();
    loader->stop();

    delay(1000);

    loader->load();

    delay(2000);
  } else if (controller->getButton(BTN_R2)){
    loader->load();
  } else {
    loader->stop();
  }

  if (controller->getButton(BTN_ONE)) {
    bowlGrabber->raise();
  } else if (controller->getButton(BTN_TWO)) {
    bowlGrabber->disablePID();
  } else if (controller->getButton(BTN_THREE)) {
    bowlGrabber->lower();
  }
}

/* autonomous
 *
 * time - Time (in seconds) to run the loop
 */
void autonomous(volatile unsigned long time) {
  // Wait to start autonomous loop until the start button is pressed
  while (controller->getButton(BTN_START) == 1) {
    Serial.println("waiting for start");
    controller->update();
    delay(20);
  }

  unsigned long startTime = millis();
  time = time * 1000;

  // Loops until autonomous has been running for the assigned time, or the select button is pressed
  while ((millis() - startTime <= time) && (controller->getButton(BTN_SELECT))) {
    // Update controller
    controller->update();

    // Run autonomous code
    autonomousFunction();

    // Delay for DFW and servo safety
    delay(20);
  }
}

/* teleop
 *
 * time - Time (in seconds) to run the loop
 */
void teleop(unsigned long time) {
  unsigned long startTime = millis();
  time = time * 1000;

  // Loops until teleop has been running for the assigned time
  //while (millis() - startTime <= time) {
  while (true) {
    // Update controller
    controller->update();

    // Run teleop code
    teleopFunction();

    // Delay for DFW and servo safety
    delay(20);

  }

  // Exit program after teleop period is completed
  exit(0);
}


void loop() {
  //autonomous(20); // Run autonomous for 20 seconds

  teleop(180);  // Run teleop for 20 seconds
}
