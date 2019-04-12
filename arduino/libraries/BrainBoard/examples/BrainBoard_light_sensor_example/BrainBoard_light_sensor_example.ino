/* 
 
 Module: BrainBoard light sensor example.
 
 Background: BrainBoard has a light sensor which outputs a voltage which increases as light increases. We can read the analogue voltage and convert this 
 to a digital value (see the analogue example for more details) and transmit the values over USB to your computer where you could save or graph them. 
 The range of values is from 0 (total darkness) to 1023 (bright lights).
 
 Learning outcomes: After following this example you will be able to read the analogue light sensor values using the 
 BrainBoard library lightRead() function.
 
 Open Tools > Serial Monitor and set the baud rate to 9600 to view the light sensor values.
 
 Author: Brody Radford
 Version: 2014-04-05
 License: GPLv3, http://gnu.org/licenses/gpl.html
 
 */

#include <BrainBoard.h>

BrainBoard brainBoard; // Create BrainBoard object. this automatically sets up the pin modes (input/output).

void setup() {
    // Put your setup code here, to run once
    Serial.begin(9600); // Open the serial port at 9,600 bps to transmit data values.
}

void loop() { // the code below runs forever

    Serial.print("Light sensor = ");         // Send some text to the coputer describing what the numbers mean. We have to send the text separate from values.
    Serial.println( brainBoard.lightRead() );  // Now we read the value using brainBoard.lightRead() and send the result to the computer.
    
    // Now check if the value is greater than 700 (about 68% of full scale) and turn on the front LEDs if it is, otherwise turn them off.    
    if( brainBoard.lightRead() > 700 ){
        brainBoard.frontLeds(HIGH);
    }else{
        brainBoard.frontLeds(LOW);
    }        
    delay(250); // Wait a little so the data is slow enough to read.
}



