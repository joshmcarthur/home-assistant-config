// BrainBoard motor driver demo. 
// Plug a small 6-12V motor into one or both of the motor driver ports "Motor 1" and "Motor 2" and see it rotate.
// ** Caution! ** Make sure you have an external batter or supply connect because the USB port might not have enough power.
// Version 2014-03-31

#include <BrainBoard.h>

BrainBoard brainBoard; // Create BrainBoard object. this automatically sets up the pin modes (input/output).

void setup() {

    // Put your setup code here, to run once
}

void loop() { // the code below runs forever

    // The motorWrite() function takes two inputs, the channel (1 or 2) and the power (includes direction), a number from -255 (full reverse) to 255 (full forward). 0 = off.
    // Now we're going to test the motor drivers by turning on both channels to 128/255 = 50% power
    brainBoard.motorWrite(1, 128); // drive motor 1 forward
    brainBoard.motorWrite(2, -128); // drive motor 2 backward

    // Now wait half a second (500 ms)
    delay(500);

    // Now reverse the motors      
    brainBoard.motorWrite(1, -128); // Drive motor 1 forward
    brainBoard.motorWrite(2, 128); // Drive motor 2 backward

    // Wait another half second
    delay(500); 

    // Now turn off the motors
    brainBoard.motorWrite(1, 0); // Turn off motor 1
    brainBoard.motorWrite(2, 0); // Turn off motor 2

    // Stop the program -- do nothing forever. 
    while(1);    
}



