#!/usr/bin/python
import RPi.GPIO as GPIO
import time

# GPIO SETUP
channel = 21
GPIO.setmode(GPIO.BCM)
GPIO.setup(channel, GPIO.IN)


def callback(channel):
    print("ON" if GPIO.input(channel) == GPIO.HIGH else "OFF")


print("OFF")
GPIO.add_event_detect(channel, GPIO.BOTH,
                      bouncetime=300, callback=callback)

# infinite loop
while True:
    time.sleep(1)

