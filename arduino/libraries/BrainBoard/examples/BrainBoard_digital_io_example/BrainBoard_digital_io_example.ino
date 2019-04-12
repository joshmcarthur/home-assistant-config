/*

Module Name: 
BrainBoard digital I/O (input/output) example. 

Background:
Digital signals take one of two values: high or low, also called 1 or 0, true or false, and on or off. The point is there are only two possible values, 
although for electrical signals these states also correspond to voltages. BrainBoard signals are 5V high, and 0V low.
This example demonstrates how to read and write digital values to BrainBoard's digital pins.

Learning outcomes: 
After following this example you will be able to read and write digital signals on BrainBoard's I/O pins using the digitalRead() and digitalWrite() functions.

Author: Brody Radford
Version: 2014-04-05
License: GPLv3, http://gnu.org/licenses/gpl.html

*/

#include <BrainBoard.h>

// Create BrainBoard object. This automatically sets up the pin modes (input/output) for the buzzer, LEDs, and motor driver.

BrainBoard brainBoard; 

void setup() {  // The following code in setup() {...} just runs once at power up.

    // We must first set the pin mode (signal direction) to input or output. Digital pins have three possible states:
    // Input, and two Output states: high, and low. If we set the pin mode to output we can program the pin to be high or low 
    // (say to turn something on or off).
    
    // All pins are set to input by default so that they don't conflict with anything they're connected to (think of 
    // two people trying to steer one car -- bad things happen!).
    
    // To set a pin as output we use the pinMode() function, giving it the pin number with or without the 'D' and the mode:
    pinMode(10, OUTPUT); // Sets pin D10 to output. The pin value is now logical 0 (low, 0V) by default.

    // Now we're going to set the pin to logical 1 (high, 5V). If you have a multimeter you can probe the voltage between D10 and 0V 
    // and confirm it's 5V for 5 seconds after connecting the power, then it will go to digital 0 (0V).
    digitalWrite(10, HIGH); 
    brainBoard.leftLeds(HIGH); // Turn on the left LEDs to mirror D10, so we know which part of the program we're in.
    delay(5000); // Wait 5 seconds.
    digitalWrite(10, LOW); // Now set the output to low.
    brainBoard.leftLeds(LOW); // Turn off the left LEDs to mirror D10.

    // From here the program continues to loop() below and stays there forever.

}

void loop() { // The code below runs forever, looping back to the start when it gets to the end (unless we have stopped the program).

    // Now we're going to combine the digitalWrite() function we learned about in setup() with the digitalRead() function to check the input on 
    // pin D11 and mirror it to the front front (D12) LEDs. 
    
    // To connect pin 11 to a signal we'll use a ballpoint pen and carefully use the conductive metal tip to connect D11 to the pins either side.
    
    // Connecting D11 to the 5V pin will input a digital high signal and the front LEDs will turn on. Connecting D11 to D10 will turn off the LEDs,
    // because we previously set D10 to low. Note that the LEDs might flash on and off unpredictably when nothing is connected to D11 because the input state
    // isn't defined as high or low.

    // We use the "if" statement to make decisions:
    if( digitalRead(11) == HIGH){  // In plain English this says: IF digital pin D10's value equals high (digital 1, 5V) THEN turn on the front LEDs, ELSE turn them off. 
        brainBoard.frontLeds(HIGH);
    }else{
        brainBoard.frontLeds(LOW);
    }

    // Don't forget to use double equals (==) to check for equality! This is very important because a single equal sign is for assignment, meaning 
    // "set the variable on the left of the equals to the value on the right".
    
    // A simpler but less obvious way to do this feeds the input signal straigh to the output:
    brainBoard.frontLeds(digitalRead(11));

    // Now the program goes back to the start of the loop and repeats the cycle of checking the pin value and updating the LED.
}



