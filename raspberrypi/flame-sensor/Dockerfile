FROM hypriot/rpi-alpine-scratch
RUN apk add musl-dev python3 python3-dev gcc mosquitto-clients bash --update

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app
ADD . /usr/src/app
RUN pip3 install -r requirements.txt
ENV MQTT_HOST localhost
ENV MQTT_TOPIC home/fireplace/active
ENV MQTT_WILL_TOPIC wills/home/fireplace/active

CMD ["bash", "run.sh"]
