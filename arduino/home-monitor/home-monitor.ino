#include <RH_NRF24.h>
#include <RHReliableDatagram.h>
#include <SPI.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
const int BUILTIN_LED = 13;
const int BASE_STATION_ADDRESS = 0;
const int MY_STATION_ADDRESS = 10;
const String stationName = "bedroom";
RH_NRF24 driver;
RHReliableDatagram radio(driver, MY_STATION_ADDRESS);


void setup() {
  Serial.begin(115200);
  if (!driver.init()) {
    Serial.println("Failed to init transmit!");
  }
  
  dht.begin();
}

void send_message(String readingType, String readingValue) {
  String message = "R>" + stationName + ":" + readingType + "=" + readingValue;

  Serial.println("TRANSMIT: " + message);

    int buf_len = message.length();
  
    char buf[buf_len];
    message.toCharArray(buf, buf_len);
  
    digitalWrite(BUILTIN_LED, HIGH); // Flash a light to show transmitting
  
  
    radio.sendtoWait((uint8_t *)buf, buf_len, BASE_STATION_ADDRESS); 
  
    digitalWrite(BUILTIN_LED, LOW);
    delay(200);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(10000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  send_message("temperature", (String) t);
  send_message("humidity", (String) h);
}
