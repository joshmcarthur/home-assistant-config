# wallynet

#### installation

```
  cd [arduino-instalation]/libraries
  git clone https://github.com/Br3nda/wallynet.git
```

Then restart you arduino ide

#### Hardware

* reciever and transmitters are currently USD1.99 for a pair at [dx.com](http://www.dx.com/p/433mhz-rf-transmitter-module-receiver-module-link-kit-for-arduino-arm-mcu-wl-green-220194)
* transmtters are current USD1.33 at [dx.com](http://www.dx.com/p/433mhz-wireless-transmitter-module-superregeneration-for-arduino-green-149254)



#### Usage

2 methods, for sending beaconed messages over ultra cheap 433MHz transmitters.

In the arduino ide, in setup(), tell the library:

 * which pin the 433MHz transmitter was wired to, 
 * which led to light to indicate transmission (Uno have a handy led on pin 11)
 * what your station is called. 

`wallynet_setup(int transmit_pin, int led_pin, String station_name);`

Example 

```arduino
wallynet_setup(1, 11, "laundry");
```


Use `wallynet_send_beacon`  to transmit. 

```arduino
wallynet_send_beacon(String reading_name, String reading_value)
```

e.g.

```arduino
  float humidity = DHT.humidity;  
  wallynet_send_beacon("humidity", (String) humidity);
```

```arduino
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
```

#### Recieving messages

```arduino
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
```


### Dependencies:

* [Virtual Wire](https://www.pjrc.com/teensy/td_libs_VirtualWire.html)

### Notes:
 
* Messages are un-encrypted and trivially intercepted or jammed
* Nothing checks that the message was recieved
* Your own transmitters may all transmit over the top of each other, and garble each other.
