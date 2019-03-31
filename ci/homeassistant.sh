#!/bin/bash

set -ev

cd ../homeassistant

docker build . -t config_checker

docker run -v $PWD:/config -v $PWD/secrets.example.yaml:/config/secrets.yaml config_checker python -m homeassistant --config /config --script check_config
