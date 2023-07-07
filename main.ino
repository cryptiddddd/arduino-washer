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
const unsigned long int TOTAL_MS = 60000; // This will be the variable used by the LCD screen and its buttons. The goal time.
const int TIME_PRINT_RES = 5000; // Every so many cycles, the timer screen will be updated.
unsigned int displayTime = TOTAL_MS / 1000;


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
	Serial.println("TOGGLING FAN");
	if (on)	digitalWrite(fanPin, HIGH);
	else digitalWrite(fanPin, LOW);
}

/**
 * Updates the time screen with the current time remaining.
 * @param time The time remaining in ms.
*/
void updateTimeScreen(unsigned long int goalTime) {
	displayTime = (goalTime - millis()) / 1000;
	Serial.println(displayTime);
}

/**
 * Checks the current time, updates time screen, and indicates whether the wash cycle should still be running or not.
 * @param goalTime The predicted time the cycle will end..
 * @return True if the timer is still going (wash can continue). False if time is out.
*/
bool isRunning(long unsigned int goalTime) {
	return goalTime > millis();
}

/**
 * Lowers arm into its lowest position.
*/
void lowerArm() {
	Serial.println("LOWERING ARM.");

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
	Serial.println("PARKING ARM.");

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
	Serial.println("BEGIN WASH CYCLE");

	toggleFan(true);

	// Find/define position 0.
	lowerArm();
	verticalStep.setCurrentPosition(0);

	// Set speeds.
	verticalStep.setSpeed(-MOVE_SPEED);
	rotationStep.setSpeed(MOVE_SPEED);

	// Local variable will track start time.
	long unsigned int goalTime = millis() + TOTAL_MS;

	// Cycle count will keep track of when to update the timer.
	unsigned int cycles = TIME_PRINT_RES;

	while (isRunning(goalTime)) {
		if (cycles >= TIME_PRINT_RES) {
			updateTimeScreen(goalTime);
			cycles = 0;
		}

		// Listen for "off" & bottom flag.
		buttonLoops();

		if (startBtn.isPressed()) {
			Serial.println("WASH ENDED EARLY!");
			break;
		}

		// Reverse vertical motion.
		if (botFlag.isPressed() || verticalStep.currentPosition() <= -8330) {
			verticalStep.setSpeed(-verticalStep.speed());
		}

		rotationStep.runSpeed();
		verticalStep.runSpeed();
		++cycles;
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

	// Set debounce times.
	topFlag.setDebounceTime(50);
	botFlag.setDebounceTime(50);
	startBtn.setDebounceTime(50);

	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(startPin, INPUT_PULLUP);
	pinMode(fanPin, OUTPUT);

	verticalStep.setMaxSpeed(MAX_MOVE_SPEED);
	rotationStep.setMaxSpeed(MAX_MOVE_SPEED);

	// Return arm to park position if needed.
	buttonLoops();
	if (topFlag.getStateRaw() == 1) parkArm(); // .getStateRaw() ignores debounce.

	Serial.println("THE BEAST AWAKENS");
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
