/* 
 
 Module: BrainBoard temperature sensor example.
 
 Background: BrainBoard has a temperature sensor which outputs a voltage that increases with temperature. The BrainBoard library has a function to read the analogue voltage and convert it to 
 temperature in degrees celcius: tempRead(). We can transmit the temperatures over USB to a computer where you could save or graph them. 
 The temperature sensor is next to the reset button. Try pressing it with your finger and watching the temperature measurements rise in the serial monitor.
 
 Learning outcomes: After following this example you will be able to read the temperature using the BrainBoard library tempRead() function.
 
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

    Serial.print("Temperature = ");         // Send some text to the coputer describing what the numbers mean. We have to send the text separate from values.
    Serial.println(brainBoard.tempRead());  // Now we read the value using brainBoard.tempRead() and send the result to the computer.
    
    // Now check if the value is less greater than 28 degrees and turn on the front LEDs if it is, otherwise turn them off.    
    if(brainBoard.tempRead() > 28){
        brainBoard.frontLeds(HIGH);
    }else{
        brainBoard.frontLeds(LOW);
    }        
    delay(250); // Wait a little so the data is slow enough to read.
}



