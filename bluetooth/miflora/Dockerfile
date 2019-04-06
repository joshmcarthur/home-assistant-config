FROM resin/rpi-raspbian

RUN apt-get update &&\
    apt-get install -y git python3 python3-pip bluetooth bluez &&\
    git clone https://github.com/ThomDietrich/miflora-mqtt-daemon.git /opt/miflora-mqtt-daemon &&\
    mkdir /config

WORKDIR /opt/miflora-mqtt-daemon
RUN pip3 install -r requirements.txt
CMD [ "python3", "./miflora-mqtt-daemon.py", "--config_dir", "/config" ]


