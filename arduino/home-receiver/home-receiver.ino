#include <SPI.h>
#include <EtherCard.h>
#include <Wire.h>

#include <RH_ASK.h>
#include <SPI.h>

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static BufferFiller bfill;  // used as cursor while filling the buffer

byte Ethernet::buffer[700];

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
    
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)  {
    Serial.println( "Failed to access Ethernet controller");
  }
  
  if (!ether.dhcpSetup()) {
    Serial.println("DHCP failed");
  }

  randomSeed(analogRead(0));

  ether.printIp("IP:  ", ether.myip); 
}

void storeStationValue(int stationId, char type, String value) {
  READINGS[stationId][READING_TYPE_IDENTIFIER] = (float) random();
  switch (type) {
    case 't':
      READINGS[stationId][READING_TYPE_TEMPERATURE] = value.toFloat();
      break;
    case 'h': 
      READINGS[stationId][READING_TYPE_HUMIDITY] = value.toFloat();
      break;
  }
}

void buildResponse(BufferFiller& buf) {

  buf.emit_p(
    PSTR("HTTP/1.0 200 OK\r\nContent-Type: application/json\r\n\r\n"
        "{"
         "\"bedroom\": {\"temperature\": $L, \"humidity\": $L, \"id\": $L},"
         "\"garage\": {\"temperature\": $L, \"humidity\": $L, \"id\": $L},"
         "\"study\": {\"temperature\": $L, \"humidity\": $L, \"id\": $L}"
         "}"
        
    ), 
    (long) READINGS[STATION_NAME_BEDROOM][READING_TYPE_TEMPERATURE], 
    (long) READINGS[STATION_NAME_BEDROOM][READING_TYPE_HUMIDITY],
    (long) READINGS[STATION_NAME_BEDROOM][READING_TYPE_IDENTIFIER],
    (long) READINGS[STATION_NAME_GARAGE][READING_TYPE_TEMPERATURE], 
    (long) READINGS[STATION_NAME_GARAGE][READING_TYPE_HUMIDITY],
    (long) READINGS[STATION_NAME_GARAGE][READING_TYPE_IDENTIFIER],
    (long) READINGS[STATION_NAME_STUDY][READING_TYPE_TEMPERATURE], 
    (long) READINGS[STATION_NAME_STUDY][READING_TYPE_HUMIDITY],
    (long) READINGS[STATION_NAME_STUDY][READING_TYPE_IDENTIFIER]
  );
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

    String stationName = receivedData.substring(2, receivedData.indexOf(':'));
    String readingType = receivedData.substring(receivedData.indexOf(':') + 1, receivedData.indexOf('='));
    String readingValue = receivedData.substring(receivedData.indexOf('=') + 1);

    if (stationName == "bedroom") {
        storeStationValue(STATION_NAME_BEDROOM, readingType[0], readingValue);
    }
    
    if (stationName == "garage") {
        storeStationValue(STATION_NAME_GARAGE, readingType[0], readingValue);
    }

    if (stationName == "study") {
      storeStationValue(STATION_NAME_STUDY, readingType[0], readingValue);
    }
  }

   if (ether.packetLoop(ether.packetReceive())) {
    bfill = ether.tcpOffset();
    buildResponse(bfill);
    ether.httpServerReply(bfill.position());
   }

  delay(500);
}
