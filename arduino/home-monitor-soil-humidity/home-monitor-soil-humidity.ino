#include <RH_ASK.h>
#include <SPI.h>

const int TRANSMIT_PIN = 12;
const int BUILTIN_LED=13;
const int SOIL_HUMIDITY_PIN = A0;
const String stationName = "kitchen";
RH_ASK driver(500, TRANSMIT_PIN, TRANSMIT_PIN);

void setup() {
  Serial.begin(115200);
  pinMode(SOIL_HUMIDITY_PIN, INPUT);
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
  int humidityReading = analogRead(SOIL_HUMIDITY_PIN);
  send_message("soil-humidity", (String) humidityReading);
  
  // Wait a few seconds between measurements.
  delay(10000);
}
