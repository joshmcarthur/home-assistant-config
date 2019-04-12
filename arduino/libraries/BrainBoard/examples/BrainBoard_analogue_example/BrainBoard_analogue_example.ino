/* 
 
 Module: BrainBoard analogue input example. 
 
 Background: BrainBoard has is capable of measuring voltages between 0V and 5V, known as analogue (or analog, US spelling) signals. BrainBoard can convert analogue signals 
 in this range to a digital value between 0 and 1023, meaning that 0V = 0 digital, 5V = 1023 digital, and 2.5V would equal about 512. This process is called analogue to digital
 conversion, or digitisation (because we're converting real-world continuous values to discrete digital values) and we use the analogRead() function.
 
 Learning outcomes: After following this example you will be able to read analogue signals on BrainBoard's analogue input pins using the analogRead() function. 
 
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

    // Here we'll read the analogue value on the variable resistor (analogue pin 3 -- make sure there is a jumper connecting pin A3 to VR on JP3!) and use it to generate a tone 
    // and turn on a LED:
    tone(BUZZER, analogRead(3));
    
    if(analogRead(3) > 512){
        brainBoard.frontLeds(HIGH);
    }else{
        brainBoard.frontLeds(LOW);
    }

    // Now the program goes back to the start of the loop.
}




