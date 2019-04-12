/**
   The MySensors Arduino library handles the wireless radio link and protocol
   between your home built sensors/actuators and HA controller of choice.
   The sensors forms a self healing radio network with optional repeaters. Each
   repeater and gateway builds a routing tables in EEPROM which keeps track of the
   network topology allowing messages to be routed to nodes.

   Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
   Copyright (C) 2013-2`015 Sensnology AB
   Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors

   Documentation: http://www.mysensors.org
   Support Forum: http://forum.mysensors.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   version 2 as published by the Free Software Foundation.

 *******************************

   REVISION HISTORY
   Version 1.0: Henrik EKblad
   Version 1.1 - 2016-07-20: Converted to MySensors v2.0 and added various improvements - Torben Woltjen (mozzbozz)

   DESCRIPTION
   This sketch provides an example of how to implement a humidity/temperature
   sensor using a DHT11/DHT-22.

   For more information, please visit:
   http://www.mysensors.org/build/humidity

*/

// Enable debug prints
#define MY_DEBUG

// Enable and select radio type attached
#define MY_RADIO_NRF24
//#define MY_RADIO_RFM69
//#define MY_RS485

#include <SPI.h>
#include <MySensors.h>
#include <DHT.h>
#include <IRremote2.h>

// Set this to the pin you connected the DHT's data pin to
#define DHT_DATA_PIN 4

// Set this offset if the sensor has a permanent small offset to the real temperatures
#define SENSOR_TEMP_OFFSET 0

// Sleep time between sensor updates (in milliseconds)
// Must be >1000ms for DHT22 and >2000ms for DHT11
static const uint64_t UPDATE_INTERVAL = 60000;

// Force sending an update of the temperature after n sensor reads, so a controller showing the
// timestamp of the last update doesn't show something like 3 hours in the unlikely case, that
// the value didn't change since;
// i.e. the sensor would force sending an update every UPDATE_INTERVAL*FORCE_UPDATE_N_READS [ms]
static const uint8_t FORCE_UPDATE_N_READS = 10;

#define CHILD_ID_HUM 0
#define CHILD_ID_TEMP 1
#define CHILD_ID_HEATPUMP 3

float lastTemp;
float lastHum;
uint8_t nNoUpdatesTemp;
uint8_t nNoUpdatesHum;
bool metric = true;

MyMessage msgHum(CHILD_ID_HUM, V_HUM);
MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);


MyMessage msgHeatpumpHeating(CHILD_ID_HEATPUMP, V_HVAC_FLOW_STATE);
MyMessage msgHeatpumpFanSpeed(CHILD_ID_HEATPUMP, V_HVAC_SPEED);
MyMessage msgHeatpumpSetpointCool(CHILD_ID_HEATPUMP, V_HVAC_SETPOINT_COOL);



// Possibly not supported by HA
MyMessage msgHeatpumpOff(CHILD_ID_HEATPUMP, V_STATUS);
MyMessage msgHeatpumpVaneMode(CHILD_ID_HEATPUMP, V_STATUS);
MyMessage msgHeatpumpFlowMode(CHILD_ID_HEATPUMP, V_HVAC_FLOW_MODE);
MyMessage msgHeatpumpSetpointHeat(CHILD_ID_HEATPUMP, V_HVAC_SETPOINT_HEAT);
MyMessage msgHeatpumpTemp(CHILD_ID_HEATPUMP, V_TEMP);

bool heatpumpOff = false;
int  heatpumpTemperature = 24;
HvacMode heatpumpMode = HVAC_HOT;
HvacFanMode  heatpumpFanSpeed = FAN_SPEED_AUTO;
HvacVanneMode heatpumpVaneMode = VANNE_AUTO_MOVE;
String heatpumpFlowMode = "Auto";

DHT dht;
IRsend irsend;


void presentation()
{
  // Send the sketch version information to the gateway
  sendSketchInfo("HallClimate", "3.0");
  metric = getControllerConfig().isMetric;

  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_HUM, S_HUM);
  wait(500);
  present(CHILD_ID_TEMP, S_TEMP);
  wait(500);
  present(CHILD_ID_HEATPUMP, S_HVAC, "Heatpump");
  wait(1000);


}


void setup()
{
  Serial.begin(115200);
  dht.setup(DHT_DATA_PIN); // set data pin of DHT sensor
  if (UPDATE_INTERVAL <= dht.getMinimumSamplingPeriod()) {
    Serial.println("Warning: UPDATE_INTERVAL is smaller than supported by the sensor!");
  }
  // Sleep for the time of the minimum sampling period to give the sensor time to power up
  // (otherwise, timeout errors might occure for the first reading)
  sleep(dht.getMinimumSamplingPeriod());

  Serial.println("In setup");
  updateHeatpumpState();
}


void loop()
{
  send(msgHeatpumpHeating.set(heatpumpMode == HVAC_HOT ? "HeatOn" : "CoolOn"));
  send(msgHeatpumpFanSpeed.set(heatpumpFanSpeed == FAN_SPEED_AUTO ? "Auto" : "Min"));
  send(msgHeatpumpSetpointCool.set(heatpumpTemperature));
  //send(msgHeatpumpSetpointHeat.set(heatpumpTemperature));
  //send(msgHeatpumpTemp.set(heatpumpTemperature));
  
  // Possibly not supported by HA
  //send(msgHeatpumpOff.set(heatpumpOff ? 1 : 0));
  //send(msgHeatpumpFlowMode.set(heatpumpFlowMode));


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

  // Wait for a while
  // TODO: to save energy, this should use sleep() with an interrupt on the radio
  wait(UPDATE_INTERVAL);
}

void receive(const MyMessage msg) {
 String msgData = msg.data;
 msgData.trim();
 bool changed = false

 switch (msg.type) {
   case V_STATUS:
     bool isOff = msgData == "Off";
     if (heatpumpOff != isOff) {
      changed = true
      heatpumpOff = ifOff;
     }
     
     break;
   case V_HVAC_SETPOINT_COOL:
     if (heatpumpMode != HVAC_COLD) {
      changed = true
     }
     heatpumpMode = HVAC_COLD;
     heatpumpTemperature = msgData.toInt();
     break;
   case V_HVAC_SETPOINT_HEAT:
     heatpumpMode = HVAC_HOT;
     heatpumpTemperature = msgData.toInt();
     break;
   case V_HVAC_FLOW_STATE:
     if (msgData == "HeatOn") {
       heatpumpMode = HVAC_HOT;
     }
     else if (msgData == "CoolOn") {
       heatpumpMode = HVAC_COLD;
     }
     break;
   case V_HVAC_SPEED:
     if (msgData == "Auto") {
       heatpumpFanSpeed = FAN_SPEED_AUTO;
     }
     else if (msgData == "Min") {
       heatpumpFanSpeed = FAN_SPEED_1;
     }
     else if (msgData == "Normal") {
       heatpumpFanSpeed = FAN_SPEED_3;
     }
     else if (msgData == "Max") {
       heatpumpFanSpeed = FAN_SPEED_5;
     }
     break;
 }

 updateHeatpumpState();
}

void updateHeatpumpState() {
  irsend.sendHvacMitsubishi(heatpumpMode, heatpumpTemperature, heatpumpFanSpeed, heatpumpVaneMode, heatpumpOff);
  //send(msgHeatpumpTemp.set(heatpumpTemperature));
}

