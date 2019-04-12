/**
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 *******************************
 *
 * REVISION HISTORY
 * Version 1.0 - Henrik Ekblad
 *
 * DESCRIPTION
 * Example sketch showing how to control physical relays.
 * This example will remember relay state after power failure.
 * http://www.mysensors.org/build/relay
 */

// Enable debug prints to serial monitor
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69

// Enable repeater functionality for this node
//#define MY_REPEATER_FEATURE

#include <MySensors.h>
#include <SPI.h>
#include <DHT.h>

// Set this to the pin you connected the DHT's data pin to
#define DHT_DATA_PIN 4

// Set this offset if the sensor has a permanent small offset to the real temperatures
#define SENSOR_TEMP_OFFSET 0

// Sleep time between sensor updates (in milliseconds)
// Must be >1000ms for DHT22 and >2000ms for DHT11
static const uint64_t UPDATE_INTERVAL = 10000;

// Force sending an update of the temperature after n sensor reads, so a controller showing the
// timestamp of the last update doesn't show something like 3 hours in the unlikely case, that
// the value didn't change since;
// i.e. the sensor would force sending an update every UPDATE_INTERVAL*FORCE_UPDATE_N_READS [ms]
static const uint8_t FORCE_UPDATE_N_READS = 10;

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_RELAY_STATE 2
#define CHILD_ID_GARAGE_DOOR_1_STATE 3
#define GARAGE_DOOR_1_CLOSED_RANGE 400 // cm

float lastTemp;
float lastHum;
uint8_t nNoUpdatesTemp;
uint8_t nNoUpdatesHum;
bool metric = true;
bool initialRelayStateSent = false;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
MyMessage msgMainDoorRelayStatus(CHILD_ID_RELAY_STATE, V_STATUS);
MyMessage msgMainDoorStatus(CHILD_ID_GARAGE_DOOR_1_STATE, V_STATUS);

DHT dht;


#define MAIN_DOOR_RELAY  3  // Arduino Digital I/O pin number for first relay (second on pin+1 etc)
#define RELAY_ON 1  // GPIO value to write to turn on attached relay
#define RELAY_OFF 0 // GPIO value to write to turn off attached relay
#define DOOR_OPEN 1
#define DOOR_CLOSED 0
#define RANGEFINDER_TRIG_PIN 5 // Arduino Digital I/O pin number for pin to trigger rangefinder
#define RANGEFINDER_ECHO_PIN 6 // Arduino Digital I/O pin number for pin to read rangefinder echo


void before()
{
  pinMode(MAIN_DOOR_RELAY, OUTPUT);
  pinMode(RANGEFINDER_TRIG_PIN, OUTPUT);
  pinMode(RANGEFINDER_ECHO_PIN, INPUT);
}

void setup()
{
  dht.setup(DHT_DATA_PIN); // set data pin of DHT sensor
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod()) {
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
  // Sleep for the time of the minimum sampling period to give the sensor time to power up
  // (otherwise, timeout errors might occure for the first reading)
  sleep(dht.getMinimumSamplingPeriod());
}

void presentation()
{
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("GarageBaseStation", "1.1");

   // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_HUM, S_HUM);
  present(CHILD_ID_TEMP, S_TEMP);
  present(CHILD_ID_RELAY_STATE, S_BINARY);
  present(CHILD_ID_GARAGE_DOOR_1_STATE, S_BINARY);
  
  metric = getControllerConfig().isMetric;
}


void loop()
{
  // Send initial relay state
  if (!initialRelayStateSent) {
    send(msgMainDoorRelayStatus.set(RELAY_OFF));
    request(CHILD_ID_RELAY_STATE, V_STATUS);
    wait(2000, C_SET, V_STATUS);
  }
  
  // Force reading sensor, so it works also after sleep()
  dht.readSensor(true);
  
  // Get temperature from DHT library
  float temperature = dht.getTemperature();
  if (isnan(temperature)) {
    Serial.println("Failed reading temperature from DHT!");
  } else if (temperature != lastTemp || nNoUpdatesTemp == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp = temperature;
    if (!metric) {
      temperature = dht.toFahrenheit(temperature);
    }
    // Reset no updates counter
    nNoUpdatesTemp = 0;
    temperature += SENSOR_TEMP_OFFSET;
    send(msgTemp.set(temperature, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature);
    #endif
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp++;
  }

  // Get humidity from DHT library
  float humidity = dht.getHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed reading humidity from DHT");
  } else if (humidity != lastHum || nNoUpdatesHum == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum = humidity;
    // Reset no updates counter
    nNoUpdatesHum = 0;
    send(msgHum.set(humidity, 1));
    
    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity);
    #endif
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum++;
  }

  // Get distance from rangefinder
  digitalWrite(RANGEFINDER_TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  digitalWrite(RANGEFINDER_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(RANGEFINDER_TRIG_PIN, LOW);
  float duration = pulseIn(RANGEFINDER_ECHO_PIN, HIGH);
  float distanceInCm = (duration/2) / 29.1;
  Serial.println("Rangefinder duration is: " + (String) duration);
  Serial.println("Distance to door is: " + (String) distanceInCm + "cm");

  // WILD GUESS
  if (distanceInCm > GARAGE_DOOR_1_CLOSED_RANGE) {
    send(msgMainDoorStatus.set(DOOR_OPEN));
  } else {
    send(msgMainDoorStatus.set(DOOR_CLOSED));
  }

  // Sleep for a while to save energy
  wait(UPDATE_INTERVAL); 
}

void receive(const MyMessage &message)
{
	// We only expect one type of message from controller. But we better check anyway.
	if (message.type==V_STATUS) {
    if (!initialRelayStateSent) {
      Serial.println("Receiving initial value from controller for main door relay!");
      initialRelayStateSent = true;
      return;
    } else {
      
  		// Change relay state
  		digitalWrite(MAIN_DOOR_RELAY, RELAY_ON);
      send(msgMainDoorRelayStatus.set(RELAY_ON));
      sleep(500);
      digitalWrite(MAIN_DOOR_RELAY, RELAY_OFF);
      send(msgMainDoorRelayStatus.set(RELAY_OFF));
  		
  		// Write some debug info
  		Serial.print("Incoming change for sensor:");
  		Serial.print(message.sensor);
  		Serial.print(", New status: ");
  		Serial.println(message.getBool());
    }
	}
}

