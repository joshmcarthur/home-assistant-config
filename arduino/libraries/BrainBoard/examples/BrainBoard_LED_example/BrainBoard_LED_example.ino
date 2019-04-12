// BrainBoard LED demo. 
// Blinks all LEDs.
// Version 2014-03-31

#include <BrainBoard.h>

BrainBoard bb; // Create BrainBoard object. this automatically sets up the pin modes (input/output).

void setup() {
    // put your setup code here, to run once
}

void loop() { // the code below runs forever
    
    // Flash the LEDs
    bb.leftLeds(ON);
    bb.rightLeds(ON);
    bb.frontLeds(ON);
    delay(5); // Wait 5 milliseconds
    
    bb.leftLeds(OFF);
    bb.rightLeds(OFF);
    bb.frontLeds(OFF);
    delay(200); // Wait 500 milliseconds
}


