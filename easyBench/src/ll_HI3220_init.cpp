#include "ll_HI3220_init.hpp"

void init_SPI_pins()
{
  pinMode(PIN_SPI_MRST, OUTPUT);
  pinMode(PIN_SPI_HCS, OUTPUT);
  pinMode(PIN_SPI_READY, INPUT);
  pinMode(PIN_SPI_RUN, OUTPUT);
}

int HI3220_set_state(int state)
{
  switch (state)
  {
  case HI3220_IDLE_STATE:
    digitalWrite(PIN_SPI_MRST, LOW);
    delay(1);
    digitalWrite(PIN_SPI_MRST, HIGH);
    delay(10); // TO MODIFY : replace 10 by 1
    if (digitalRead(PIN_SPI_READY) == HIGH)
    {
      return 0; // SUCCESS
    }
    else
    {
      return -1; // FAIL
    }
    break;

  case HI3220_ACTIVE_STATE:
    digitalWrite(PIN_SPI_RUN, HIGH);
    break;

  default:
    return -1;
    break;
  }
  return 0;
}

void init_SPI_settings(double clock_frequency, unsigned char data_order, unsigned char data_mode)
{
  SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI);
  SPI.beginTransaction(SPISettings(clock_frequency, data_order, data_mode));
  digitalWrite(PIN_SPI_HCS, HIGH);
}