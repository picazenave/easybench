#ifndef LL_HI3220_COMMON_HPP

#define LL_HI3220_COMMON_HPP

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>


// SPI Pins
#define PIN_SPI_MOSI 11
#define PIN_SPI_MISO 13
#define PIN_SPI_SCK 12
#define PIN_SPI_HCS 10
#define PIN_SPI_MRST 5
#define PIN_SPI_READY 6
#define PIN_SPI_RUN 7

enum HI3220_STATES{
    HI3220_IDLE_STATE,
    HI3220_ACTIVE_STATE
};

#define QUEUE_LEN 800 // must be integer when divided by 32

#define TX_RING_SIZE QUEUE_LEN
#define RX_RING_SIZE QUEUE_LEN

#endif