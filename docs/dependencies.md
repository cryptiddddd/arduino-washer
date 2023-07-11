# Dependencies

This program uses the AccelStepper Arduino library, built by Mike McCauley. [View documentation here.](http://www.airspayce.com/mikem/arduino/AccelStepper/index.html)

This program also uses the ezButton Arduino library. [View documentation here.](https://arduinogetstarted.com/tutorials/arduino-button-library)

## AccelStepper Use

Currently, this program has a very simple use of the AccelStepper library, which excludes use of acceleration.

`.setSpeed()` is used to set which direction (and of course, how fast) a motor will turn. Then `.runSpeed()` is called in a loop to move the motor.

It's important to note that the position data of each `AccelStepper` is simulated, and is not always proven. This is why the ezButton flags are used. However, `.setCurrentPosition()` will set the current position to 0, which is a great reference*. The code currently uses this in combination with `.currentPosition()` to coordinate the machine's movement (see [`checkRotation()` and `washCycle()`](../main.ino)).

> *Important note: `.setCurrentPosition()` has the side effect of resetting the motor's speed to 0, as noted [in the documentation](http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a9d917f014317fb9d3b5dc14e66f6c689).


## ezButton Use

Use of the ezButton library is fairly standard. The one thing worthy of note is the use of `.getStateRaw()` in `setup()` -- this is vital to ensure that the arm *only* attempts to return to the "park" position if it is not already in the park position.
