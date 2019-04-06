#!/usr/bin/env bash
python3 -u flame.py | mosquitto_pub -h $MQTT_HOST -r -t $MQTT_TOPIC -l -k 5 --will-topic $MQTT_WILL_TOPIC --will-payload "died" --will-retain
