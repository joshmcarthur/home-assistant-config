// WallyNet.h

#ifndef WallyNet_h
#define WallyNet_h

#include <stdlib.h>
#include <Arduino.h>

void wallynet_send_beacon(String reading_name, String reading_value);
void wallynet_listen();

void wallynet_setup(int tx_pin, int led_pin, String station_name);
void wallynet_setup_rx(int tx_pin, int led_pin);
#endif
