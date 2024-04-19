#include "ll_A429_SPI_pin_init.hpp"

void init_SPI2_MRST_pin()
{
  pinMode(SPI2_MRST, OUTPUT);
  // #if VERBOSE == 1
  //   Serial.printf("[init_SPI2_MRST_pin] Initializes the Master Reset (MRST) pin for the SPI2 interface as an output.\n");
  // #endif
}

void init_SPI2_HCS_pin()                             
{
  pinMode(SPI2_HCS, OUTPUT);
}

void init_SPI2_READY_pin()
{
  pinMode(SPI2_READY, INPUT);
}

void init_SPI2_RUN_pin()
{
  pinMode(SPI2_RUN, OUTPUT);
}

int init_SPI2_pins()
{
  init_SPI2_MRST_pin();
  init_SPI2_HCS_pin();
  init_SPI2_READY_pin();
  init_SPI2_RUN_pin();
  return SPI2_PINS_INIT;
}
