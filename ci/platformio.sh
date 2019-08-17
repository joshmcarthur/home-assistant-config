#!/bin/bash

set -eo

docker run -v $PWD/platformio/heatpump-controller:/project -w /project python:2.7 sh -c 'pip install -U platformio && platformio update && platformio run'
