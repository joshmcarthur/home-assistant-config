#!/usr/bin/env bash

ip=$(hostname -I | cut -d\  -f1)
docker run --privileged -d --restart=always -e MQTT_HOST=$ip joshmcarthur/fireplace-sensor
