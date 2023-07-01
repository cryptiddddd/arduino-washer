// new code for automated parts washer.
// crane khavari 2023

/* PIN DEFINITIONS, see setup() for setup. */
const int dirPin = 2;
const int fanPin = 6;
const int stepPin = 3;


/* UNIT DEFINITIONS */
const int stepsPerRev = 1600;
const int rotPerRevolution = 1600;  // Initially `rot_stepsPerRevolution`


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
    int current = millis() - startTime;

    // Return goal time minus current time.
    return totalMS - current;
}


/*********** PROTOCOLS ***********/
/**
 * Updates the time screen with the current time remaining.
 * @param time The time remaining in ms.
*/
void updateTimeScreen(int time) {

}

/**
 * Checks the current time, updates time screen, and returns timer status.
 * @param startTime The time the cycle started, in ms.
 * @return True if the timer is still going (wash can continue). False if time is out.
*/
bool checkTime(int startTime) {
    int remainingTime = secondsRemaining(startTime);
    updateTimeScreen(remainingTime);

    return remainingTime > 0;
}


/**
 * Lowers arm into its lowest position.
*/
void lowerArm() {

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

}

/**
 * Rotates the arm.
 * @param direction Direction of rotation, accepts only 1 or -1.
 * @example `rotateArm(-1)` rotates counter-clockwise, `rotateArm(1)` rotates clockwise.
*/
void rotateArm(int direction) {

}

/**
 * Runs the wash cycle. 
*/
void washCycle() {
    lowerArm();

    // Local variable will track start time.
    int startTime = millis();

    while (checkTime(startTime)) {
        // wash cycle washy washy scrub scrub a dubby
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
	// pinMode(startSwitch, INPUT_PULLUP);
	pinMode(fanPin, OUTPUT);

    // Configure serial output
    Serial.begin(9600);

    // Return arm to park position.
    parkArm();
}

/**
 * Main loop. Awaits button input.
*/
void loop() {

    // check button input

    // on up button, increase total time variable by one minute

    // on down button, decrease total time variable by one minute.

    // on start button, call wash cycle.

}
