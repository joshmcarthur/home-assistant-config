// BrainBoard demo. 
// Push button 1 (SW1) to test motor drivers.
// Push button 2 (SW2) to read sensor values and transmit over serial to Arduino Serial Monitor.
// Open terminal monitor at 9600 bps and push button 2 to display sensor readings.
// Version 2014-03-25

#include <BrainBoard.h>

BrainBoard bb; // Create BrainBoard object. this automatically sets up the pin modes (input/output).

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600); // start serial communications
}

void loop() { // the code below runs forever

    if(bb.sw1Read()){ // Read push button 1 and test the motor drivers and LEDs
        bb.motorWrite(1, 255); // Full forward
        bb.motorWrite(1, -255); // Full reverse
        bb.leftLeds(ON);
        bb.rightLeds(ON);
        bb.frontLeds(ON);
        delay(500);
        bb.motorWrite(1, -255);
        bb.motorWrite(1, 255);
        bb.leftLeds(OFF);
        bb.rightLeds(OFF);
        bb.frontLeds(OFF);
        delay(500);
        bb.motorWrite(1, 0);
        bb.motorWrite(1, 0);
    }

    if(bb.sw2Read()){ // Read push button 2 and transmit sensor values overial serial to terminal monitor

        Serial.print("Light sensor = ");
        Serial.println(bb.lightRead());
        Serial.print("Temperature sensor counts = ");
        Serial.println(bb.tempRead());
        Serial.print("Variable resistor (A3) = ");
        Serial.println(bb.vrRead());
        Serial.println();
    }
    
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


