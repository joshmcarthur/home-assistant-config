FROM resin/rpi-raspbian

# Base layer
ENV ARCH=arm
ENV CROSS_COMPILE=/usr/bin/

# Install some packages
# #1:   20160803 - Added net-tools and nmap for https://home-assistant.io/components/device_tracker.nmap_scanner/
# #3:   20161021 - Added ssh for https://home-assistant.io/components/device_tracker.asuswrt/
# #8:   20170313 - Added ping for https://home-assistant.io/components/switch.wake_on_lan/
# #10:  20170328 - Added libffi-dev, libpython-dev and libssl-dev for https://home-assistant.io/components/notify.html5/
# #11: 20180118 - Add ffmpeg for streaming security cameras
RUN apt-get update && \
    apt-get install --no-install-recommends \
      build-essential \
      libffi-dev libpython-dev libssl-dev libsqlite3-dev  \
      curl \
      ca-certificates \
      net-tools nmap \
      iputils-ping \
      libav-tools \
      ssh && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*


RUN curl https://www.python.org/ftp/python/3.6.1/Python-3.6.1.tgz > python.tgz &&\
    tar -xvzf python.tgz &&\
    cd Python-3.6.1 &&\
    ./configure -enable-loadable-sqlite-extensions &&\
    make &&\
    make install &&\
    curl https://bootstrap.pypa.io/get-pip.py > get-pip.py &&\
    python3.6 get-pip.py

# Mouting point for the user's configuration
VOLUME /config


# Start Home Assistant
CMD [ "python3.6", "-m", "homeassistant", "--config", "/config" ]

# Trust more
RUN curl https://curl.haxx.se/ca/cacert.pem > /etc/ssl/certs/cacert.pem && update-ca-certificates

# Install Home Assistant 
RUN pip3 install homeassistant pywebpush
