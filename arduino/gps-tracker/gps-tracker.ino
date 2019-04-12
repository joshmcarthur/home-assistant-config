#include <SoftwareSerial.h>
#include <SD.h>
#include <SPI.h>
#include <avr/sleep.h>

/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;
static const int CHIP_SELECT_PIN = 10;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("GPS Tracker"));
  Serial.println();

  if (!SD.begin(CHIP_SELECT_PIN)) {
    cli();
    sleep_enable();
    sleep_cpu();
  } else {
    Serial.println("SD Card ready.");
  }

  Serial.flush();
}

void loop()
{
  delay(500);
  
  String dataString = "";
  
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    dataString += String("" + ss.read());
  }

  if (dataString.length() < 1) { return; }

  //File dataFile = SD.open("nmea.txt", FILE_WRITE);

  Serial.println(dataString);

//  if (dataFile) {
//    dataFile.print(dataString);
//    dataFile.close();
//    Serial.print(dataString);
//  } else {
//    Serial.println("Could not open nmea.txt");
//  }

  Serial.flush();
}


