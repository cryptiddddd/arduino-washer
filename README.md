# 3D Printed Part Washer



---
note that seconds handled in ms

all the different operations for lower/lift/park arm. rotate arm.

notes on the initial code and ignored lines that didn't mean anything?

view docs for design/maintanence information

accelstepper [zstepper var in program] docs:
http://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a96685e0945b7cf75d5959da679cd911e
this is the thing that goes up and down!

IMPORTANT: 
when buttons exist, manual control for moving the thing up and down. this will make it much easier for debugging stuff. there also needs to be an emergency "stop moving" button? also just. the issue here is trying to get it into park position on startup. annyoing.

---
the logic of a wash cycle where rotation is concurrent with up/down movement

setup: 
- enable fan
- lower arm
- raise speed of both motors

loop: 
- run button loop
- if bottom switch is pressed, reverse speed.
- if currentposition is <= [whatever value is ~1.5 above the bottom] [note: negative value], reverse speed.
- ?? logic for turning
- runspeed on both motors
