// WallyNet.cpp
//
// Wrapper for VirtualWire
//
// Author: Brenda Wallace <brenda@brenda.nz>
#include <stdlib.h>
#include <Arduino.h>

#include "WallyNet.h"
#include <VirtualWire.h>

const int wallynet_bps = 500;
int wallynet_tx_pin;
int wallynet_rx_pin;
int wallynet_tx_led_pin;
int wallynet_rx_led_pin;
String wallynet_station_name;

void wallynet_setup(int tx_pin, int led_pin, String station_name) {
    wallynet_tx_pin = tx_pin;
    wallynet_tx_led_pin = led_pin;
    wallynet_station_name = station_name;

    // Initialise the IO and ISR
    vw_set_tx_pin(wallynet_tx_pin);
    vw_set_ptt_inverted(true);    // Required for DR3100
    vw_setup(wallynet_bps);       // Bits per sec
    
    pinMode(wallynet_tx_pin, OUTPUT);    
}

void wallynet_setup_rx(int rx_pin, int led_pin) {
    wallynet_rx_pin = rx_pin;
    wallynet_rx_led_pin = led_pin;

    delay(1000);  

    vw_set_rx_pin(wallynet_rx_pin);
    vw_set_ptt_inverted(true);  // Required for DR3100
    vw_setup(wallynet_bps);     // Bits per sec
    vw_rx_start();              // Start the receiver PLL running

    pinMode(wallynet_rx_led_pin, OUTPUT);    
}

void wallynet_send_beacon(String reading_type, String reading_value) { 
  String message = "R>" + wallynet_station_name + ":" + reading_type + "=" + reading_value;

  int buf_len = message.length() +1;
  
  char buf[buf_len];
  message.toCharArray(buf, buf_len);
  
  digitalWrite(wallynet_tx_led_pin, HIGH); // Flash a light to show transmitting

  vw_send((uint8_t *)buf, buf_len);
  vw_wait_tx(); // Wait until the whole message is gone

  digitalWrite(wallynet_tx_led_pin, LOW);
  delay(2000);
}

void wallynet_listen() {
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    // Non-blocking
    if (vw_get_message(buf, &buflen)) {
       // Flash a light to show received good message
        digitalWrite(wallynet_rx_led_pin, HIGH);
        for (int i = 0; i < buflen; i++) {
            Serial.print((char)buf[i]);
        }
        Serial.println();
        digitalWrite(wallynet_rx_led_pin, LOW);
    }
    delay(500);  
}

