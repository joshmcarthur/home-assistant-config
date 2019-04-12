#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  ss.begin(GPSBaud);

  if (!SD.begin()) {
    while(true) {};
  }
}

void loop() {
  File dataFile = SD.open("nmea.txt", FILE_WRITE);
  if (!dataFile) { return; }

  while (ss.available()) {
    dataFile.write(ss.read());
  }

  dataFile.flush();
  dataFile.close();
}

