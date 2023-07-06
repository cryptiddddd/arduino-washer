/**
 * Arduino 3D-printed part washer.
 * See attached documentation for design notes.
 * Crane Khavari, 2023
*/
#include <AccelStepper.h>
#include <ezButton.h>

/* PIN DEFINITIONS. */
const unsigned short int dirPin = 2;
const unsigned short int fanPin = 6;
const unsigned short int stepPin = 3;
const unsigned short int startPin = 8;

/* OBJECTS */
AccelStepper verticalStep(AccelStepper::FULL2WIRE, 2, 3);
AccelStepper rotationStep(AccelStepper::FULL2WIRE, 4, 5); // this may need to be 

ezButton topFlag(11);
ezButton botFlag(9);
ezButton startBtn(startPin);

/* UNIT DEFINITIONS */
const int MAX_MOVE_SPEED = 3000; 
const int MOVE_SPEED = 2000; // must be < max speed 4000
const int REV_STEPS = 1600;

/* TIMER VARS */
const int TOTAL_MS = 20000; // This will be the variable used by the LCD screen and its buttons. The goal time.


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
    return TOTAL_MS - (millis() - startTime);
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
 * Enable/disable the fan.
*/
void toggleFan(bool on) {
    if (on)	digitalWrite(fanPin, HIGH);
    else digitalWrite(fanPin, LOW);
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
 * Runs the wash cycle. Can be stopped by pressing start button.
*/
void washCycle() {
    toggleFan(true);

    // Set position 0.
    lowerArm();
    verticalStep.setCurrentPosition(0);

    // Set speeds.
    verticalStep.setSpeed(-MOVE_SPEED);
    rotationStep.setSpeed(MOVE_SPEED);

    // Local variable will track start time.
    int startTime = millis();

    while (isRunning(startTime)) {
        // Listen for "off".
        buttonLoops();

		if (startBtn.isPressed()) {
            Serial.println("WASH ENDED EARLY!"); // TEMP: later print indication to screen.
            break;
        }

        // Reverse vertical motion.
        if (botFlag.isPressed() || verticalStep.currentPosition() <= -8330) {
            verticalStep.setSpeed(-verticalStep.speed());
        }

        // rotationStep.runSpeed(); // temp -- run only the vertcal stepper, determine what is slowing the cycle so badly.
		verticalStep.runSpeed();
    }

    // Reset.
    parkArm();
    toggleFan(false);
}


/*********** ARDUINO OPS ***********/

/**
 * Sets/resets variables and makes connections.
*/
void setup() {
    // Configure serial output
    Serial.begin(9600);

    topFlag.setDebounceTime(1);

    pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(startPin, INPUT_PULLUP);
	pinMode(fanPin, OUTPUT);

    verticalStep.setMaxSpeed(MAX_MOVE_SPEED);
    rotationStep.setMaxSpeed(MAX_MOVE_SPEED);

    // Return arm to park position if needed.
    buttonLoops();
    if (!topFlag.isPressed()) parkArm();

    // Set official debounce after, to be sure that arm parks smartly.
    topFlag.setDebounceTime(50);
    botFlag.setDebounceTime(50);
    startBtn.setDebounceTime(50);
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
