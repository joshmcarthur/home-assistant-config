/*
	BrainBoard.h
	
	BrainBoard Library for Arduino IDE. Provides convenient wrapper functions to access BrainBoard hardware. 
	Copy the entire folder to Arduino/libraries and restart the IDE. See examples in File > Examples > BrainBoard.

	Current status: Work in progress!
	
	Version: 2014-04-06

	Author: Brody Radford
	
	Licensed under GPLv3, http://gnu.org/licenses/gpl.html

	BrainBoard Usage Notes
	----------------------

	1.  Maximum voltage on any pin is 5V.
	
	2.  Power BrainBoard with USB or 5-9V DC (centre +ve) using the barrel socket or JP10. Note the '+' on JP10. 
			Connecting reversed power will probably let out all the smoke and void your warranty. 
	
	3.  Make sure you have external power (from barrel socket or JP10) before turning on motors in your code so you don't overload your USB port.
	
	4.  There's a handy Arduino Nano pin reference at http://forum.arduino.cc/index.php?topic=147582.0 -- the board layout is different 
			but the pin functions (and chip) are the same.
	
	5.  Some useful pin definitions follow.
*/

#include <avr/pgmspace.h>
#include "BrainBoard.h"


// BrainBoard class constructor. Configures output pins on creation.
BrainBoard::BrainBoard()
{
  pinMode(FRONT_LEDS, OUTPUT);
  pinMode(LEFT_LEDS, OUTPUT);  
  pinMode(RIGHT_LEDS, OUTPUT);  
  pinMode(BUZZER, OUTPUT);  
  pinMode(MOTOR1_A, OUTPUT);  
  pinMode(MOTOR1_B, OUTPUT);  
  pinMode(MOTOR2_A, OUTPUT);  
  pinMode(MOTOR2_B, OUTPUT);  

	this->quickSelfTest(); 
}

// Waits for a button to be pressed. Useful for waiting for user confirmation before driving off (useful on robots).
void BrainBoard::waitForButtonPress(void)
{
	while( !this->sw1Read() && !(this->sw2Read()) );
}

// Power up the motor drivers. There are two channels corresponding to motors 1 and 2. The first input is the motor number, 1 or 2, or 'L' or 'R', the second is speed.
// Speed values go from -255 (full reverse), to 0 (off) to 255 (full forward).
void BrainBoard::motorWrite(uint8_t motorNum, int16_t speed)
{
	uint8_t pwm1, pwm2;
	bool dir1, dir2;
	
	speed = constrain(speed, -255, 255); // enforce bounds

	pwm1 = 255*(speed > 0) - speed;
	dir1 = speed > 0;
	
	pwm2 = 255*(speed <= 0) + speed;
	dir2 = speed <= 0;
	
	
	if( (motorNum & 1) || (motorNum == 'L') ){
		digitalWrite(MOTOR1_A, dir1); // set direction pin: 1 -> + = +, 0 -> inverted
		analogWrite(MOTOR1_B, pwm1);		
/*			
		if(speed > 0)
		{
			analogWrite(MOTOR1_B, 255 - speed);		
			Serial.print("\tpwm ");			
			Serial.println(255-speed);
		}else if(speed < 0)
		{
			Serial.print("\tpwm ");			
			Serial.println(-speed);			
			analogWrite(MOTOR1_B, - speed);		
		}else
		{
			Serial.print("\tpwm ");			
			Serial.println(speed);
			analogWrite(MOTOR1_B, speed);				
		}
		*/
	}
	
	if( (motorNum & 2) || (motorNum == 'R') ){
		digitalWrite(MOTOR2_B, dir2); // set direction pin: 1 -> + = +, 0 -> inverted
		analogWrite(MOTOR2_A, pwm2);	
	}
	/*

	Serial.print("speed in = ");
	Serial.print(speed);	
	Serial.print("\tdir1 = ");
	Serial.print(dir1);	
	Serial.print("\tpwm1 = ");
	Serial.print(pwm1);	
	Serial.print("\tdir2 = ");
	Serial.print(dir2);	
	Serial.print("\tpwm2 = ");
	Serial.print(pwm2);		

	
	Serial.println();

	*/
}

// Sound the buzzer at frequency freq (Hz) for time t (millisceconds)
void BrainBoard::beep(uint16_t freq, uint16_t t)
{
	if( freq ){
		tone(BUZZER, freq);
		delay(t);
		if( t ){ //set t=0 to leave it buzzing forever
			noTone(BUZZER);
		}
	}else{
		noTone(BUZZER);
	}
}

// Set the left LED state to on or off.
void BrainBoard::leftLeds(uint8_t state)
{
	if( state == TOGGLE){
		digitalWrite(LEFT_LEDS, !digitalRead(LEFT_LEDS));		
	}else{
		digitalWrite(LEFT_LEDS, state);
	}
}

// Set the right LED state to on or off.
void BrainBoard::rightLeds(uint8_t state)
{
	if( state == TOGGLE){
		digitalWrite(RIGHT_LEDS, !digitalRead(RIGHT_LEDS));		
	}else{
		digitalWrite(RIGHT_LEDS, state);
	}
}

// Set the front LED state to on or off.
void BrainBoard::frontLeds(uint8_t state)
{
	if( state == TOGGLE) {
		if(digitalRead(FRONT_LEDS) == HIGH){
			digitalWrite(FRONT_LEDS, LOW);		
		}else {
			digitalWrite(FRONT_LEDS, HIGH);		
		}
	}else{
		digitalWrite(FRONT_LEDS, state);
	}
}

// Return the current state of push button 1 (high = pushed).
bool BrainBoard::sw1Read()
{
	return digitalRead(SW1_PIN);
}

// Return the current state of push button 2 (high = pushed).
bool BrainBoard::sw2Read()
{
	return digitalRead(SW2_PIN);
}

// Return the value of the light sensor. Values range from 0 (dark) to 1023 (bright).
uint16_t BrainBoard::lightRead()
{
	return analogRead(LDR);
}

// Return the current temperature in degrees C.
float BrainBoard::tempRead()
{
//	return 5.0 * ((float)analogRead(TEMP_SENSOR) - 1023.0 / 10) / 1023.0 / 0.01; 
	return 500.0 * (float)analogRead(TEMP_SENSOR) / 1023.0 - 50.0;
}

// Return the value of the variable resistor. Values range from 0 (CCW limit) to 1023 (CW limit).
uint16_t BrainBoard::vrRead()
{
	return analogRead(ANALOG3);
}

// Print some help text to the serial port. Could be more self-documenting.
void BrainBoard::getHelp()
{
	char* helpText = \
"Hello! I'm BrainBoard.\n\
\n\
See http://www.mindkits.co.nz/store/kits/the-mindkits-brainboard for demo code and more documentation.\n\
\n\
-end transmission-\n";

	Serial.begin(9600);

	Serial.println(helpText);
}

// Do a quick hardware test to show that I'm alive.
uint16_t BrainBoard::quickSelfTest() 
{
  digitalWrite(FRONT_LEDS, HIGH);
  digitalWrite(LEFT_LEDS, HIGH);
  digitalWrite(RIGHT_LEDS, HIGH);
	busyWait(50);
  digitalWrite(FRONT_LEDS, LOW);
  digitalWrite(LEFT_LEDS, LOW);
  digitalWrite(RIGHT_LEDS, LOW);
}

// Do a longer self-test to show that I'm more alive.
uint16_t BrainBoard::selfTest()
{
	int val;

	quickSelfTest();
	
	val = analogRead(TEMP_SENSOR);
	
	if( (val > 200) || (val < 100) ){ // ~150 is normal for room temp and functioning sensor
		this->beep(500,100);
	}else{
		this->beep(1000,50);
	}
	//TODO: test analog input ranges, buttons
}

// Apparently global objects are created before delay() works, so we need to roll our own for use in the constructor. Accurate...ish.
void BrainBoard::busyWait(uint16_t ms)
/* 
 	magic number of 1987 found with:
    uint16_t t, T;
    t = millis();
    bb.BusyWait(1000);
    T=millis()-t;
    Serial.println(T);
 */
{
	for( uint16_t i=0; i<ms; i++ ){
		for( uint16_t j=0; j<1987; j++ ){
			asm("nop");
		}
	}
}

