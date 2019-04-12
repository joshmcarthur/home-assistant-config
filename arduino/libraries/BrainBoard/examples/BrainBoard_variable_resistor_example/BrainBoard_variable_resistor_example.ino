/* 
 
 Module: BrainBoard variable resistor example.
 
 Background: BrainBoard has a variable resistor (potentiometer) with an easy to turn knob which makes it very easy to adjust the behaviour of your program when it's running.
 The variable resistor is an analogue sensor (see analogue input example for more info), so it outputs values increasing from 0 at the counter-clockwise limit to 1023 when turned all 
 the way to the right. Using the BrainBoard library function we can easily get the digital value and transmit the values over USB to your computer where you could save or graph them. 
 
 ** Important note ** : The variable resistor shares analogue input 3 with JP3, so the VR and A3 pins must be connected with a jumper! Otherwise the variable resistor will be disconnected.
 
 Learning outcomes: After following this example you will be able to read the variable resistor v alues using the BrainBoard library vrRead() function.
 
 Open Tools > Serial Monitor and set the baud rate to 9600 to view the variable resistor values.
 
 Author: Brody Radford
 Version: 2014-04-05
 License: GPLv3, http://gnu.org/licenses/gpl.html
 
 */

#include <BrainBoard.h>

BrainBoard brainBoard; // Create BrainBoard object. This automatically sets up the pin modes (input/output).

void setup() {
    // Put your setup code here, to run once
    Serial.begin(9600); // Open the serial port at 9,600 bps to transmit data values.
}

void loop() { // the code below runs forever

    Serial.print("Variable resistor = ");  // Send some text to the coputer describing what the numbers mean. We have to send the text separate from values.
    Serial.println( brainBoard.vrRead() ); // Now we read the value and send the result to the computer.
    
    // Now check if the value is greater than 512 (knob in the middle) and turn on the front LEDs if it is, otherwise turn them off.    
    if( brainBoard.vrRead() > 512 ){
        brainBoard.frontLeds(HIGH);
    }else{
        brainBoard.frontLeds(LOW);
    }        
    delay(250); // Wait a little so the data is slow enough to read.
}



