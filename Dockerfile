FROM alpine:latest

# Obtained and modified from https://github.com/lukstep/raspberry-pi-pico-docker-sdk/blob/main/Dockerfile

# Install toolchain
RUN apk update && \
    apk upgrade && \
    apk add git \
    python3 \
    py3-pip \
    cmake \
    build-base \
    libusb-dev \
    bsd-compat-headers \
    newlib-arm-none-eabi \
    gcc-arm-none-eabi \
    g++-arm-none-eabi

# Raspberry Pi Pico SDK
ARG SDK_PATH=/usr/share/pico_sdk
RUN git clone --depth 1 https://github.com/raspberrypi/pico-sdk $SDK_PATH && \
    cd $SDK_PATH && \
    git submodule update --init

ENV PICO_SDK_PATH=$SDK_PATH

# Picotool installation
RUN git clone --depth 1 https://github.com/raspberrypi/picotool.git /home/picotool && \
    cd /home/picotool && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    cp /home/picotool/build/picotool /bin/picotool && \
    rm -rf /home/picotool

# Set workdir for app
WORKDIR /home/app