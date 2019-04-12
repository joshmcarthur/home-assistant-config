#include <RH_ASK.h>
#include <SPI.h>
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

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
const int TRANSMIT_PIN = 12;
const int BUILTIN_LED = 13;
const int GARAGE_DOOR_SWITCH_BUTTON_PIN = 5;
const int GARAGE_DOOR_PROXIMITY_PIN = 4;
const int GARAGE_DOOR_RELAY_PIN = 6;
const String stationName = "garage";
int garageDoorProximityPreviousState = NULL;
RH_ASK driver(500, TRANSMIT_PIN, TRANSMIT_PIN);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(GARAGE_DOOR_PROXIMITY_PIN, INPUT_PULLUP);
  pinMode(GARAGE_DOOR_SWITCH_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GARAGE_DOOR_RELAY_PIN, OUTPUT);
  
  if (!driver.init()) {
    Serial.println("Failed to init transmit!");
  }
}

void send_message(String readingType, String readingValue) {
  String message = "R>" + stationName + ":" + readingType + "=" + readingValue;

  Serial.println("TRANSMIT: " + message);

  int buf_len = message.length();

  char buf[buf_len];
  message.toCharArray(buf, buf_len);

  digitalWrite(BUILTIN_LED, HIGH); // Flash a light to show transmitting


  driver.send((uint8_t *)buf, buf_len);
  driver.waitPacketSent();

  digitalWrite(BUILTIN_LED, LOW);
  delay(200);
}

void loop() {
  // Wait 5 seconds between measurements.
  delay(5000);

  // Immediately measure the state of the door proximity
  int garageDoorProximityState = digitalRead(GARAGE_DOOR_PROXIMITY_PIN); // Read the state of the switch
  //if (garageDoorProximityState != garageDoorProximityPreviousState) {
  //  garageDoorProximityPreviousState = garageDoorProximityState;
    send_message("primary_garage_door", garageDoorProximityState == LOW ? "CLOSED" : "OPEN");
  //}

  

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  send_message("temperature", (String) temperature);
  send_message("humidity", (String) humidity);

  int garageDoorSwitchPressed = !digitalRead(GARAGE_DOOR_SWITCH_BUTTON_PIN);
  if (garageDoorSwitchPressed) { 
    Serial.println("Button pressed!"); 
    digitalWrite(GARAGE_DOOR_RELAY_PIN, HIGH);
    delay(200);
    digitalWrite(GARAGE_DOOR_RELAY_PIN, LOW);
  }
}
