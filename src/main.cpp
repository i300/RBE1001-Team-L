#include "Arduino.h"

#include "RobotMap.h"
#include "Utilities.h"

#include "Controller/Controller.hpp"
#include "Subsystems/DriveTrain/DriveTrain.hpp"
#include "Subsystems/BowlGrabber/BowlGrabber.hpp"
#include "Subsystems/Loader/Loader.hpp"
#include "Subsystems/CubeFlipper/CubeFlipper.hpp"
#include "Sensors/LineFollower/LineFollower.hpp"
#include "Sensors/Rangefinder/Rangefinder.hpp"

// Controller
Controller *controller;

// Subsystems
DriveTrain *driveTrain;
BowlGrabber *bowlGrabber;
Loader *loader;
CubeFlipper *flipper;

// Sensors
LineFollower *lineFollower;
Rangefinder *rangefinder;

// State
typedef enum  {
  DRIVE_FORWARD,
  LOAD_FOAM,
  FIND_LINE,
  LINE_FOLLOW,
  PICKUP_BOWL,
  STOP
} AutonomousState;

AutonomousState state = DRIVE_FORWARD;

void setup() {
  Serial.begin(9600);

  delay(100);

  pinMode(PIN_SENSOR_LOADER_LIMIT, INPUT_PULLUP);

  // Initialize Controller
  controller = new Controller(PIN_LED_DEBUG);

  // Initialize Subsystems
  driveTrain = new DriveTrain(PIN_MOTOR_LEFT, PIN_MOTOR_RIGHT, INVERTED_RIGHT);
  bowlGrabber = new BowlGrabber(PIN_MOTOR_BOWLGRABBER, PIN_SERVO_GRABBER);
  loader = new Loader(PIN_MOTOR_FOAMLOADER, PIN_SENSOR_LOADER_LIMIT);
  flipper = new CubeFlipper(PIN_MOTOR_CUBEFLIPPER);

  // Initialize Sensors
  lineFollower = new LineFollower(PIN_SENSOR_LINEFOLLOWER_LEFT, PIN_SENSOR_LINEFOLLOWER_RIGHT);
  rangefinder = new Rangefinder(PIN_SENSOR_RANGEFINDER);
}

void autonomousFunction(unsigned long startTime, volatile unsigned long beginTime) {
  LineFollowerReading lineFollowStatus = lineFollower->read();
  bool16 foamLoaded = loader->isFoamLoaded();
  int16 currentDistance = rangefinder->read();

  Serial.println(currentDistance);

  bowlGrabber->update();

  if (state == DRIVE_FORWARD) {
    // Lower the Bowl Grabber and begin driving forward;
    bowlGrabber->lower();
    driveTrain->arcadeDrive(1, 0);

    // Drive forward until the sensor detects very close objects
    if (currentDistance <= 8) {
      driveTrain->stop();

      // If a foam isn't loaded, load a form. Otherwise, grab the bowl.
      if (!foamLoaded) {
        state = LOAD_FOAM;
      } else {
        state = PICKUP_BOWL;
      }
    }
  } else if (state == LOAD_FOAM) {
    // Begin loading the foam
    driveTrain->stop();
    loader->load();

    bowlGrabber->raise();
    bowlGrabber->update();
    while (!bowlGrabber->isAtSetpoint()) {
      bowlGrabber->update();
    }

    // Drive forward slightely, then stop driving
    driveTrain->arcadeDrive(1, 0);
    delay(750);
    driveTrain->stop();

    // Wait until a foam is loaded
    while (!foamLoaded) {
      foamLoaded = loader->isFoamLoaded();
      loader->load();
      bowlGrabber->update();
      driveTrain->stop();

      if (millis() - startTime >=  beginTime) return;
    }

    loader->stop();

    // Drive backward slightely
    driveTrain->arcadeDrive(-1, 0);
    delay(1000);
    driveTrain->stop();

    state = STOP;
  } else if (state == PICKUP_BOWL) {
    bowlGrabber->closeGrabber();
    bowlGrabber->raise();
  } else if (state == STOP) {
    driveTrain->stop();
    bowlGrabber->update();
    loader->stop();
  }

  /*
  // Drive left if the left sensor is triggered,
  // drive right if the right sensor is triggered,
  // and drive straight if neither are triggered
  if (lineFollowStatus == DETECT_BOTH) {
    driveTrain->arcadeDrive(0, 0);
  } else if (lineFollowStatus == DETECT_LEFT) {
    driveTrain->arcadeDrive(1, 1);

    delay(25);
  } else if (lineFollowStatus == DETECT_RIGHT) {
    driveTrain->arcadeDrive(1, -1);

    delay(25);
  } else if (lineFollowStatus == DETECT_NONE) {
    driveTrain->arcadeDrive(1, 0);
  }
  */
}

void teleopFunction() {
  driveTrain->arcadeDrive(controller->getY(JS_LEFT), controller->getX(JS_RIGHT));
  bowlGrabber->update();

  if (controller->getButton(BTN_UP)) {
    flipper->up();
  } else if (controller->getButton(BTN_DOWN)) {
    flipper->down();
  } else {
    flipper->stop();
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
  } else if (controller->getButton(BTN_R2)) {
    loader->load();
  } else if (controller->getButton(BTN_L2)) {
    loader->unload();
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
  while (!controller->getButton(BTN_START)) {
    Serial.println("waiting for start");
    controller->update();
    delay(20);
  }

  unsigned long startTime = millis();
  time = time * 1000;

  // Loops until autonomous has been running for the assigned time, or the select button is pressed
  while ((millis() - startTime <= time) && (!controller->getButton(BTN_SELECT))) {
    // Update controller
    controller->update();

    // Run autonomous code
    autonomousFunction(startTime, time);

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
  while (millis() - startTime <= time) {
  //while (true) {
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
  autonomous(20); // Run autonomous for 20 seconds

  teleop(180);  // Run teleop for 20 seconds
}
