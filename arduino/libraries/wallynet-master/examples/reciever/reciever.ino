#include <WallyNet.h>

const int led_pin = 13;
const int rx_pin = 2;

void setup() {
  Serial.begin(115200);  // What speed to output recieved messages at.
  wallynet_setup_rx(rx_pin, led_pin);
}

void loop() {
  wallynet_listen();
}