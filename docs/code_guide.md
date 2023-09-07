# Code Guide

> Completed as of 07/10/2023.

This file is a guide to [`main.ino`](../main.ino). It is advised to read about the machine's [design concept](./design.md) before reading notes on the code.


## General Notes:
- For Arduino beginners, get started [here](https://www.arduino.cc/en/Guide) and find the language reference [here](https://www.arduino.cc/reference/en/). Additional resources are linked throughout this file as well.
- For used libraries and their documentation, see [dependencies](./dependencies.md).
- The `AccelStepper` library uses a positive speed to turn clockwise, and a negative speed to turn counter-clockwise.
- All time is handled in milliseconds, unless explicitly noted otherwise.


# Program Structure

`main.ino` is structured in the following sections:
1. Library imports.
2. Variable/object definitions.
    - Define I/O pins.
    - Define button objects and motor objects.
    - Define motor speeds.
    - Define timer-related variables.
3. Function definitions.
    - Helper functions.
    - Protocols.
    - Arduino operations: `setup()` and `loop()`.

These sections are distinguished for the sake of clarity and organization. Subsections are noted in `main.ino`'s comments, with further explanation below.

Library imports are self-explanatory. For related information, see [dependencies](./dependencies.md).

## Variable and Object Definitions

### I/O Pins

Input and output pins correspond to the needed pins on the Arduino board. They are grouped and labelled as they are for the sake of clarity and easy adjustment.

*Note: I am not fully sure what each is. I haven't messed with them much from the original code. A few aren't used, and I plan to fix this.


### Button and Motor Objects

As explained in the machine's [design](./design.md), there are two motors. These are `AccelStepper` objects from the AccelStepper library. `verticalStep` refers to the Z-axis stepper in the back of the machine, and `rotationStep` refers to the rotating stepper on the front. These two run at different speeds and change directions at different times, but are called to step together.

There are three `ezButton` objects, only one (as of right now) that is a traditional input button, as the variable name denotes: `startBtn` begins the wash cycle, and ends the wash cycle early if needed.

The other two `ezButtons` are optical sensors the machine uses as reference points for the Z-axis. In the code, these are `topFlag` and `botFlag`. When `topFlag` is triggered, the `verticalStep` has moved as far upwards as it ought to, and when `botFlag` is triggered, `verticalStep` has moved as far downwards as it ought to.

See also: [dependencies](./dependencies.md).


### Motor Speeds

Each movement speed is declared as a constant, as these **should never** be changed throughout the code. In fact, they are declared unsigned for a reason. Instead, these speed definitions may be modified with a minus (`-`) when a motor needs to turn in the opposite direction. For example:

```ino
// Setting the arm to move downwards.
verticalStepper.setSpeed(MOVE_SPEED);

...

// Setting the arm to move upwards...
verticalStepper.setSpeed(-MOVE_SPEED); 
```

Different speeds are set for the different motors and operations. A quick guide:

| Variable name         | Suggested value  | Use            |
|-----------------------|:----------------:|----------------|
| `MAX_MOVE_SPEED`      | 4000             | This is the maximum moving speed of the motors. This NEEDS TO BE RECONFIGURED ACTUALLY, as this is not a limit, but what they are meant to accelerate to, in the AccelStepper library.
| `LOWERING_SPEED`      | -                | The speed at which the wash arm lowers.
| `MOVE_SPEED`          | -                | The speed at which the wash arm moves up and down during the wash cycle, AND the speed at which it parks.
| `ROTATION_SPEED`      | 3500             | The speed at which the basket is rotated.


### Timer Variables

Timer variables are a work in progress, as the configurable clock is still pending. Here is how I plan for it to work.

`minutes` will be a `short int` incremented/decremented by the user. It will be multiplied by 60,000 (a minute in milliseconds) to decide how long the wash cycle should run. `displaySeconds` will hold the current seconds to print to screen. It is declared in a global scale so that the new time may be compared to the previous time, and duplicate times will not print.

Work in progress...


## Function Definitions

### Helper Functions

Currently, there are four helper functions. These each perform a simple task or calculation.

`buttonLoops()` is an `inline` function that calls the `.loop()` (listening) method for each button. 

`toggleFan()` enables or disables the fan, depending on the given argument.

`updateTimeScreen()` updates the visual timer for the user.

`isRunning()` checks if the wash cycle should still be running, and returns a `bool`.


### Protocols

Functions in the protocol section are a little more advanced than helpers, and usually have an immediately physically evident side-effect. 

`parkArm()` and `lowerArm()` each run a loop to move the washer arm into the highest and lowest positions, respectfully.

`checkRotation()`, used in each loop of `washCycle()`, listens for when to reverse the direction of the basket's rotation.

`washCycle()` prepares and runs the main washing cycle. See the machine's [design](./design.md) for the flow of the wash cycle.


### Arduino Operations

Arduino's [required functions](https://docs.arduino.cc/learn/starting-guide/getting-started-arduino#program-structure).

During `setup()`, the board:
- Configures the default serial output. 
- Sets [debounce time](https://arduinogetstarted.com/tutorials/arduino-button-library#content_functions) for the buttons.
- Configures [pin modes](https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/).
- Sets stepper max speeds WHICH MAYBE I DON'T NEED TO DO...?
- Returns the arm to park position, if necessary.

The main loop waits for button input, calling the related function on button press (see: [protocols](#protocols)).
