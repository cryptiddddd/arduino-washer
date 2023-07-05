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

const int MAX_POSITION = 0x7FFFFFFF; // maximum of position we can set (long type)
const int MOVE_SPEED = 1000; // must be < max speed 4000?
const int REV_STEPS = 1600;

ezButton switchBottom(9);
ezButton switchTop(11);
ezButton startSwitch(startPin);

AccelStepper zStepper(AccelStepper::FULL2WIRE, 2, 3);
Stepper rStepper(REV_STEPS, 4, 5);


// OTHER OPS
/**
 * runs all buttons' loops.
*/
void buttonLoop() {
	switchBottom.loop();
	switchTop.loop();
	startSwitch.loop();
}

void go() {
	digitalWrite(fanPin, HIGH);
  	Serial.println("began moving!");

	zStepper.setSpeed(MOVE_SPEED);

	while (true) {
		buttonLoop();

		if (startSwitch.isPressed()) break;

		if (switchTop.isPressed()) zStepper.setSpeed(MOVE_SPEED);
		else if (switchBottom.isPressed()) zStepper.setSpeed(-MOVE_SPEED);

		rStepper.step(1);

		zStepper.runSpeed();
	}
	
	Serial.println("done moving!");
	digitalWrite(fanPin, LOW);
}

void rotate() {
	Serial.println("attempting rotation...");

	rStepper.step(REV_STEPS);
	Serial.println("DELAYING! FOR sAFETY?");
	delay(500);
	Serial.println("returning");
}


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

	rStepper.setSpeed(60);
	zStepper.setMaxSpeed(4000);
}

void loop() {
	buttonLoop();

	if (startSwitch.isPressed()) go();

	// if (bottomSwitch.isPressed()) rotate();
}
