#include <WallyNet.h>

const int led_pin = 11;
const int tx_pin = 1;

void setup() {
  wallynet_setup(tx_pin, led_pin, "sensor-one");
}

void loop() {
  float sensor_reading = 123.22; // get this from your sensors
  wallynet_send_beacon("temp", (String) sensor_reading);
  delay(63000);
}

