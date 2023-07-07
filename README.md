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


CONCERN: 
machine only operates when plugged into power And my laptop?

07/06/2023 ACHIEVEMENTS
- returns to park position on startup ONLY WHEN NEEDED, no more redundancy and scary screeching noise.
- the wash cycle now runs at the appropriate pace!
- the timer updates much more sparsely [related to the above]

THINGS I LEARNED
- specifying "unsigned long int" actually is really important!
- the wash cycle loop runs >2400 times per second!

NEXT STEPS:
i am out of compilations for today, but i think i will work on reorganizing the code of the wash cycle, in preparation for making the rotation stepper move back and forth.
