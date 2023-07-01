
/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-stepper-motor-and-limit-switch
 */

#include <AccelStepper.h>
#include <ezButton.h>
#include <Stepper.h>

// Define stepper motor connections and steps per revolution:
#define dirPin 2
#define stepPin 3

#define rot_dirPin 4
#define rot_stepPin 5
#define stepsPerRevolution 1600
#define rot_stepsPerRevolution 1600


#define DIRECTION_CCW 1
#define DIRECTION_CW  -1

#define STATE_CHANGE_DIR 1
#define STATE_MOVE       2
#define STATE_MOVING     3
#define ROT_MOTOR_START  4


ezButton switchBottom(9); // create ezButton object
ezButton switchTop(11); // create ezButton object
ezButton startSwitch(8); // create ezButton object

AccelStepper zStepper(AccelStepper::FULL2WIRE, 2, 3);
//AccelStepper rot_stepper(AccelStepper::FULL2WIRE, rot_dirPin,rot_stepPin);
Stepper rot_stepper(rot_stepsPerRevolution, 4, 5);

int stepperState = STATE_MOVE;
int direction = DIRECTION_CCW;
long targetPos = 0;


#define MAX_POSITION 0x7FFFFFFF // maximum of position we can set (long type)


int rCount = 0;
int startBtn = 1;
int motorDir = 1;
int cycles = 5;
int i = 0;
int button = 0;
int limitSwitch = 1;
//bool isStopped = false;
const int fanPin = 6;

void setup() {
	Serial.begin(9600);

	switchBottom.setDebounceTime(50); // set debounce time to 50 milliseconds
	switchTop.setDebounceTime(50); // set debounce time to 50 milliseconds
	startSwitch.setDebounceTime(50);
	rot_stepper.setSpeed(60);
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	// pinMode(startSwitch, INPUT_PULLUP);
	pinMode(fanPin, OUTPUT);

	//maybe set max speed 6000,

	zStepper.setMaxSpeed(6000.0);   // set the maximum speed
	zStepper.setAcceleration(2000); // set acceleration
	zStepper.setSpeed(2000);         // set initial speed
	zStepper.setCurrentPosition(0); // set position
	// rot_stepper.setMaxSpeed(2000.0); // set the maximum speed
	// rot_stepper.setAcceleration(1000); // set acceleration
	// rot_stepper.setSpeed(1000);         // set initial speed
	// rot_stepper.setCurrentPosition(0); // set position
}

void loop() {
	switchBottom.loop(); // MUST call the loop() function first
	switchTop.loop(); // MUST call the loop() function first
	startSwitch.loop();

	// Starts the process
	if (startSwitch.isPressed() && button == 0){
		Serial.println(F("Start Switched has been pressed."));
		button = 1;
		i = 0;
		rCount = 0;
		digitalWrite(fanPin, HIGH);
	}

	else if (i < cycles && button == 1) {
		MotorRun();

		if (switchBottom.isPressed()){
			Serial.println(("The limit switch Bottom: TOUCHED"));
			rot_motor();
		}

		if (switchTop.isPressed()){
			Serial.println(("The limit switch Top: TOUCHED"));
			Serial.print(F("COUNT: "));
			i++;
			Serial.println(i);
		}

		if (i == cycles){
			button = 0;
			Serial.println(F("MAX CYCLE COUNT"));
			digitalWrite(fanPin, LOW);
		}

		if (startSwitch.isPressed()) {
			button = 0;
			Serial.println(F("Cycle has been stopped."));
			digitalWrite(fanPin, LOW);
		}
	}
}

void MotorRun() {
	if (switchBottom.isPressed()) {
			stepperState = STATE_CHANGE_DIR;
			direction = DIRECTION_CW;
		}

	else if (switchTop.isPressed()) {
		stepperState = STATE_CHANGE_DIR;
		// Serial.println(("The limit switch Top: TOUCHED"));
		direction = DIRECTION_CCW;
	}

	switch (stepperState) {
		case STATE_CHANGE_DIR:
			 Serial.print(("The direction -> "));
			if (direction == DIRECTION_CW)
				Serial.println(("Z MOTOR: CLOCKWISE"));

			else if (direction == DIRECTION_CCW)
				Serial.println(("Z MOTOR: ANTI-CLOCKWISE"));

			 stepperState = STATE_MOVE; // after changing direction, go to the next state to move the motor
			break;

		case STATE_MOVE:
			targetPos = direction * MAX_POSITION;
			zStepper.setCurrentPosition(0); // set position
			zStepper.moveTo(targetPos);
			stepperState = STATE_MOVING; // after moving, go to the next state to keep the motor moving infinity

			break;

		case STATE_MOVING: // without this state, the move will stop after reaching maximum position
			if (zStepper.distanceToGo() == 0) { // if motor moved to the maximum position
				zStepper.setCurrentPosition(0);   // reset position to 0
				zStepper.moveTo(targetPos);       // move the motor to maximum position again
			}
			break;
	}

	zStepper.run(); // MUST be called in loop() function
}

void rot_motor() {
	Serial.println("Rot Motor: clockwise");
	rot_stepper.step(rot_stepsPerRevolution);
	delay(50);

	// step one revolution in the other direction:
	Serial.println("Rot Motor: counterclockwise");
	rot_stepper.step(-rot_stepsPerRevolution);
	delay(50);

	Serial.println("Rot Motor: clockwise");
	rot_stepper.step(rot_stepsPerRevolution);
	delay(50);

	// step one revolution in the other direction:
	Serial.println("Rot Motor: counterclockwise");
	rot_stepper.step(-rot_stepsPerRevolution);
	delay(50);
}
