/**
 * Arduino 3D-printed part washer.
 * See attached documentation for design notes.
 * Crane Khavari, 2023
*/
#include <AccelStepper.h>
#include <ezButton.h>

/* PIN DEFINITIONS, see setup() for setup. */
const int dirPin = 2;
const int fanPin = 6;
const int stepPin = 3;
const int startPin = 8;

/* OBJECTS */
AccelStepper verticalStep(AccelStepper::FULL2WIRE, 2, 3);
// todo: define rotation stepper, and write its logic.

ezButton topFlag(11);
ezButton botFlag(9);
ezButton startBtn(startPin);

/* UNIT DEFINITIONS */
const int MOVE_SPEED = 1000; // must be < max speed 4000?
const int REV_STEPS = 1600;

/*********** TIMER VARS ***********/
int totalMS; // This will be the variable set by the LCD screen and its buttons. The goal time.


/*********** CALCS ***********/
/**
 * Calculates seconds remaining for the cycle at the given start time.
 * @param startTime The time the cycle started, in MS.
 * @return Time remaining in ms.
 * @example If the cycle began at 5ms, and it is now 10,000ms, and the total time is 60,000ms, approximately 10 seconds of a minute-long wash has passed. 
 *  In that circumstance, `secondsRemaining()` will return `50,005`, as 50,005ms remain in the wash cycle. 
*/
int secondsRemaining(int startTime) {
    // Return goal time minus current time.
    return totalMS - (millis() - startTime);
}


/*********** PROTOCOLS ***********/
/**
 * Runs all button loops.
*/
void buttonLoops() {
    startBtn.loop();
    topFlag.loop();
    botFlag.loop();
}

/**
 * Updates the time screen with the current time remaining.
 * @param time The time remaining in ms.
*/
void updateTimeScreen(int time) {
    Serial.println(time); // TEMP
}

/**
 * Checks the current time, updates time screen, and indicates whether the wash cycle should still be running or not.
 * @param startTime The time the cycle started, in ms.
 * @return True if the timer is still going (wash can continue). False if time is out.
*/
bool isRunning(int startTime) {
    int remainingTime = secondsRemaining(startTime);
    updateTimeScreen(remainingTime);

    return remainingTime > 0;
}

/**
 * Lowers arm into its lowest position.
*/
void lowerArm() {
    verticalStep.setSpeed(MOVE_SPEED);
    while (!botFlag.isPressed()) {
        botFlag.loop();
        verticalStep.runSpeed();
    }
}

/**
 * Lifts arm, NOT into park position, but upwards in the glass.
*/
void liftArm() {

}

/**
 * Returns the arm to park position.
*/
void parkArm() {
	verticalStep.setSpeed(-MOVE_SPEED);
    while (!topFlag.isPressed()) {
        topFlag.loop();
        verticalStep.runSpeed();
    }
}

/**
 * Rotates the arm.
 * @param direction Direction of rotation, accepts only 1 or -1.
 * @example `rotateArm(-1)` rotates counter-clockwise, `rotateArm(1)` rotates clockwise.
*/
void rotateArm(int direction) {

}

/**
 * Runs the wash cycle. Can be stopped by pressing start button.
*/
void washCycle() {
    lowerArm();
    verticalStep.setSpeed(-MOVE_SPEED);

    // Local variable will track start time.
    int startTime = millis();

    while (isRunning(startTime)) {
        // wash cycle washy washy scrub scrub a dubby
        buttonLoops();

		if (startBtn.isPressed()) {
            Serial.println("WASH ENDED"); // TEMP: later print indication to screen.
            break;
        }

		if (topFlag.isPressed()) verticalStep.setSpeed(MOVE_SPEED);
		else if (botFlag.isPressed()) verticalStep.setSpeed(-MOVE_SPEED);
        // todo: this might need to be changed, as the thing should stay in the liquid.

		verticalStep.runSpeed();
    }

    parkArm();
}


/*********** ARDUINO OPS ***********/

/**
 * Sets/resets variables and makes connections.
*/
void start() {
    pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(startPin, INPUT_PULLUP);
	pinMode(fanPin, OUTPUT);

    // Configure serial output
    Serial.begin(9600);

    totalMS = 20000; // TEMP: set time to 20 seconds.

    // Return arm to park position.
    parkArm();
}

/**
 * Main loop. Awaits button input.
*/
void loop() {
    // check button input
    buttonLoops();

    // on up button, increase total time variable by one minute
    // on down button, decrease total time variable by one minute.

    // on start button, call wash cycle.
    if (startBtn.isPressed()) washCycle();
}
