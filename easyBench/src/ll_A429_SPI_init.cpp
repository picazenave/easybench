#include "ll_A429_SPI_init.hpp"

void init_SPI2_start_up_state()
{
  digitalWrite(SPI2_MRST, LOW);
  delay(1);
  digitalWrite(SPI2_MRST, HIGH);
  delay(10);
}

int init_SPI2_idle_state()
{
  init_SPI2_start_up_state();
  if(digitalRead(SPI2_READY) == HIGH)
  {
    return READY;
  }
  else
  {
    return NOT_READY;
  }
}

int init_SPI2_settings(double clock_frequency, unsigned char data_order, unsigned char data_mode)
{
  SPI.begin(SPI2_SCK,SPI2_MISO,SPI2_MOSI);
  SPI.beginTransaction(SPISettings(clock_frequency, data_order, data_mode));
  digitalWrite(SPI2_HCS, HIGH);
  return SPI2_SETTINGS_INIT;
}

int init_SPI2_active_state()
{
  digitalWrite(SPI2_RUN, HIGH);
  return RUN;
}