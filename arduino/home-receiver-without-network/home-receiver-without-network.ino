#include <RH_ASK.h>
#include <SPI.h>


#define STATION_COUNT 3
#define READING_TYPE_COUNT 3
#define STATION_NAME_BEDROOM 0
#define STATION_NAME_GARAGE 1
#define STATION_NAME_STUDY 2
#define READING_TYPE_TEMPERATURE 0
#define READING_TYPE_HUMIDITY 1
#define READING_TYPE_IDENTIFIER 2

static float READINGS[STATION_COUNT][READING_TYPE_COUNT];

RH_ASK driver(500, 2, NULL, NULL, false);

void setup() {
  // Start the serial connection at 1152500 baud
  Serial.begin(115200);

  // Try and init the driver.
  if (!driver.init()) {
    Serial.println("433mhz init failed");
  } else {
    Serial.println("433mhz Ready!");
  }
}




void loop() {
  delay(25);
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen))
  {
    String receivedData = "";

    for (int i = 0; i < buflen; i++) {
      receivedData += (char) buf[i];
    }

    Serial.println(receivedData);
  }

  delay(500);
}
