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
const int dirPin = 2;
const int stepPin = 3;
const int rDirPin = 4; // `r` stands for `rot_`
const int rStepPin = 5;
const int fanPin = 6;
const int startPin = 8;

const int stepsPerRev = 1600;

const int DIRECTION_CCW = 1;
const int DIRECTION_CW = -1;

const int STATE_CHANGE_DIR = 1;
const int STATE_MOVE = 2;
const int STATE_MOVING = 3;
const int ROT_MOTOR_START = 4;

const int MAX_POSITION = 0x7FFFFFFF; // maximum of position we can set (long type)


ezButton switchBottom(9);
ezButton switchTop(11);
ezButton startSwitch(startPin);

AccelStepper zStepper(AccelStepper::FULL2WIRE, 2, 3);
Stepper rStepper(stepsPerRev, 4, 5);

int stepperState = STATE_MOVE;
int direction = DIRECTION_CCW;

int rCount = 0;
int startBtn = 1;
int motorDir = 1;
int cycles = 5;
int i = 0;
int button = 0;
int limitSwitch = 1;
bool isStopped = false;
long targetPos = 0;


// ARDUINO OPS!
void setup() {
	Serial.begin(9600);

	// units in ms
	switchBottom.setDebounceTime(50);
	switchTop.setDebounceTime(50);
	startSwitch.setDebounceTime(50);
	
	pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
	pinMode(startPin, INPUT_PULLUP);
	pinMode(fanPin, OUTPUT);

	zStepper.setMaxSpeed(6000.0);
	zStepper.setAcceleration(2000);
	zStepper.setSpeed(2000);
	zStepper.setCurrentPosition(0);

	// rStepper.setSpeed(1000); // 1000 or 60??
}

void loop() {
	switchBottom.loop();
	switchTop.loop();
	startSwitch.loop();

	// Starts the process
	if (startSwitch.isPressed() && button == 0) {
		Serial.println("Start switch has been pressed.");
		button = 1;
		i = 0;
		rCount = 0;
		digitalWrite(fanPin, HIGH);
	}

	else if (i < cycles && button == 1) {
		MotorRun();

		if (switchBottom.isPressed()) {
			Serial.println("The limit switch Bottom: TOUCHED");
			rot_motor();
		}

		if (switchTop.isPressed()) {
			Serial.println("The limit switch Top: TOUCHED");
			Serial.print("COUNT: ");
			i++;
			Serial.println(i);
		}

		if (i == cycles) {
			button = 0;
			Serial.println("MAX CYCLE COUNT");
			digitalWrite(fanPin, LOW);
		}

		if (startSwitch.isPressed()) {
			button = 0;
			Serial.println("Cycle has been stopped.");
			digitalWrite(fanPin, LOW);
		}
	}
}


// OTHER ROUTINES!
void MotorRun() {
	if (switchBottom.isPressed()) {
			stepperState = STATE_CHANGE_DIR;
			direction = DIRECTION_CW;
		}

	else if (switchTop.isPressed()) {
		stepperState = STATE_CHANGE_DIR;
		Serial.println("The limit switch Top: TOUCHED");
		direction = DIRECTION_CCW;
	}

	switch (stepperState) {
		case STATE_CHANGE_DIR:
			Serial.print("The direction -> ");
			
			if (direction == DIRECTION_CW) {
				Serial.println("Z MOTOR: CLOCKWISE");
			}

			else if (direction == DIRECTION_CCW) {
				Serial.println("Z MOTOR: ANTI-CLOCKWISE");
			}

			stepperState = STATE_MOVE; // after changing direction, go to the next state to move the motor

			break;

		case STATE_MOVE:
			targetPos = direction * MAX_POSITION;
			zStepper.setCurrentPosition(0);
			zStepper.moveTo(targetPos);
			stepperState = STATE_MOVING; // after moving, go to the next state to keep the motor moving infinity

			break;

		case STATE_MOVING: // without this state, the move will stop after reaching maximum position
			if (zStepper.distanceToGo() == 0) { // if motor moved to the maximum position
				zStepper.setCurrentPosition(0);
				zStepper.moveTo(targetPos);
			}

			break;
	}

	zStepper.run(); // MUST be called in loop() function
}

void rot_motor() {
	Serial.println("Rot Motor: clockwise");
	rStepper.step(stepsPerRev);
	delay(50);

	// step one revolution in the other direction:
	Serial.println("Rot Motor: counterclockwise");
	rStepper.step(-stepsPerRev);
	delay(50);

	Serial.println("Rot Motor: clockwise");
	rStepper.step(stepsPerRev);
	delay(50);

	// step one revolution in the other direction:
	Serial.println("Rot Motor: counterclockwise");
	rStepper.step(-stepsPerRev);
	delay(50);
}
