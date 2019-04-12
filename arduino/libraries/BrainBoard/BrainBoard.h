/*
	BrainBoard.h
	
	BrainBoard Library for Arduino IDE. Provides convenient wrapper functions to access BrainBoard hardware. 
	Copy the entire folder to Arduino/libraries and restart the IDE.

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

#ifndef BrainBoard_h
#define BrainBoard_h

#include "Arduino.h"

#include <inttypes.h> 		// contains fixed-width types, e.g. uint8_t

// Digital inputs
#define SW1_PIN 2       	// left push button, active high
#define SW2_PIN 3         // right push button, active high

// Digital outputs
#define RIGHT_LEDS  8     // RHS blue leds
#define BUZZER      9     // pizeo buzzer. use tone() to drive the buzzer.
#define MOTOR1_A    4     // OUT1A, motor 1 (left) drive pin 1
#define MOTOR1_B    5     // OUT1B, motor 1 (left) drive pin 2, PWM capable with analogWrite()
#define MOTOR2_A    6     // OUT2A, motor 2 (right) drive pin 1, PWM capable with analogWrite()
#define MOTOR2_B    7     // OUT2B, motor 2 (right) drive pin 2

// Digital input/output
#define D10 				10  	// digital 10
#define D11 				11   	// digital 11
#define FRONT_LEDS  12    // white leds, also connected to ISP header
#define LEFT_LEDS   13    // LHS blue leds, also connected to ISP header

// Analog inputs & sensors
#define TEMP_SENSOR   A0  // mcp9700 temperature sensor
#define LDR       		A1  // light-dependent resistor / light sensor
#define ANALOG2       A2  // analog input 2, available as digital I/O
#define ANALOG3       A3  // analog input 3, use jumper to connect to variable resistor. available as digital I/O when not connected to vr.
#define ANALOG4       A4  // analog input 4, also I2C SDA and digital I/O
#define ANALOG5       A4  // analog input 5, also I2C SCL and digital I/O
#define ANALOG6       A6  // analog input 6
#define ANALOG7       A7  // analog input 7

#define ON						HIGH
#define OFF						LOW
#define TOGGLE				-1

class BrainBoard
{
public:
    BrainBoard(); //constructor
    void waitForButtonPress(void);
    void motorWrite(uint8_t motorNum, int16_t speed);
    void beep(uint16_t freq, uint16_t t);
		void leftLeds(uint8_t state);
		void rightLeds(uint8_t state);
    void frontLeds(uint8_t state);
    uint16_t lightRead();
    float tempRead();
    uint16_t vrRead();
    bool sw1Read();
    bool sw2Read();
    void getHelp(); 					//prints some help info to the serial port @ 9600 bps
    uint16_t quickSelfTest(); // flashes the LEDs
    uint16_t selfTest(); 			// also checks sensors and buzzer
    void busyWait(uint16_t ms);
    //char* helpText;
};

#endif