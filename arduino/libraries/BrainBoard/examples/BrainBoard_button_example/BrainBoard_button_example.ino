/*

 Module Name: 
 BrainBoard button example. 
 
 Background:
 BrainBoard has two user programmable buttons named SW1 and SW2. These can be programmed to control BrainBoard.
 
 Learning outcomes: 
 After following this example you will be able to read BrainBoard's buttons with the sw1Read() and sw2Read() functions and use their states to control your program.
 
 Author: Brody Radford
 Version: 2014-04-05
 License: GPLv3, http://gnu.org/licenses/gpl.html
 
 */

#include <BrainBoard.h>

// Create BrainBoard object. This automatically sets up the pin modes (input/output) for the buzzer, LEDs, and motor driver.

BrainBoard brainBoard; 

void setup() {  // The following code in setup() {...} just runs once at power up.

    // From here the program continues to loop() below and stays there forever.
}

void loop() { // The code below runs forever, looping back to the start when it gets to the end (unless we have stopped the program).

    // Read button 1 and turn on the left LEDs if it is pushed, or turn them off if not. We use the "if" statement to read the buttons and make a decision:
    if( brainBoard.sw1Read() == HIGH ){ 
        brainBoard.leftLeds(HIGH);
    }
    else{
        brainBoard.leftLeds(LOW);
    }

    // Read button 2 and turn on the right LEDs:
    if( brainBoard.sw2Read() == HIGH ){ 
        brainBoard.rightLeds(HIGH);
    }
    else{
        brainBoard.rightLeds(LOW);
    }

    // If both buttons are pushed, also turn on the front LEDs:
    if( (brainBoard.sw1Read() == HIGH) && (brainBoard.sw2Read() == HIGH) ){ 
        brainBoard.frontLeds(HIGH);
    }
    else{
        brainBoard.frontLeds(LOW);
    }

    // Now the program goes back to the start of the loop and repeats the cycle of checking the pin value and updating the LED.
}




