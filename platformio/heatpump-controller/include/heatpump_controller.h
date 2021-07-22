#define XSTR(x) #x
#define STR(x) XSTR(x)

const char* ssid     = STR(SSID_NAME);
const char* password = STR(SSID_PASSWORD);

// mqtt server settings
const char* mqtt_server   = "192.168.1.180";
const int mqtt_port       = 1883;


// mqtt client settings
// Note PubSubClient.h has a MQTT_MAX_PACKET_SIZE of 128 defined, so either raise it to 256 or use short topics
const char* client_id                   = "heatpump"; // Must be unique on the MQTT network
const char* heatpump_topic              = "heatpump";
const char* heatpump_set_topic          = "heatpump/set";
const char* heatpump_status_topic       = "heatpump/status";
const char* heatpump_timers_topic       = "heatpump/timers";

const char* heatpump_debug_topic        = "heatpump/debug";
const char* heatpump_debug_set_topic    = "heatpump/debug/set";

// pinouts
const int blueLedPin = 2; // Onboard LED = digital pin 2 (blue LED on adafruit ESP8266 huzzah)

// sketch settings
const unsigned int SEND_ROOM_TEMP_INTERVAL_MS = 60000;
