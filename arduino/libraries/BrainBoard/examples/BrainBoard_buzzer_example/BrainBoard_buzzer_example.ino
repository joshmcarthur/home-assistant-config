// BrainBoard buzzer demo.
// ta-daaa! 
// Version 2014-03-31

#include <BrainBoard.h>

BrainBoard brainBoard; // Create BrainBoard object. this automatically sets up the pin modes (input/output).

void setup() {
    
    // put your setup code here, to run once
}

void loop() { // the code below runs forever

    // The beep function has two inputs: the first is frequency in Hz, the second is the tone duration in milliseconds.
    brainBoard.beep(2500, 40); // 2.5 kHz for 40 millisec

    // Silence for 40 ms
    delay(40);
    
    // Another beep
    brainBoard.beep(2500, 500); // 2.5 kHz for 500 millisec

    // Stop the program -- do nothing forever. 
    while(1);    
}


