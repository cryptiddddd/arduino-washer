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

int startBtn = 1;
int motorDir = 1;
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

	zStepper.setMaxSpeed(4000);
	zStepper.setAcceleration(2000);
	zStepper.setSpeed(2000);
	zStepper.setCurrentPosition(0);

	rStepper.setSpeed(1000);
}

void loop() {
	switchBottom.loop();
	switchTop.loop();
	startSwitch.loop();

	if (switchTop.isPressed()) zStepper.move(-MAX_POSITION);
	else if (switchBottom.isPressed()) zStepper.move(MAX_POSITION);

	zStepper.run();
}/*
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

int startBtn = 1;
int motorDir = 1;
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

	zStepper.setMaxSpeed(4000);
	zStepper.setAcceleration(2000);
	zStepper.setSpeed(2000);
	zStepper.setCurrentPosition(0);

  	zStepper.move(MAX_POSITION);

	rStepper.setSpeed(1000);
}

void loop() {
	switchBottom.loop();
	switchTop.loop();
	startSwitch.loop();

	if (switchTop.isPressed()) zStepper.move(-MAX_POSITION);
	else if (switchBottom.isPressed()) zStepper.move(MAX_POSITION);

	zStepper.run();
}

