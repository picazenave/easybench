#include "ll_A429_SPI_common.hpp"

void transmit_SPI_frame(unsigned char SPI_frame[], unsigned int SPI_frame_size)
{
    digitalWrite(SPI2_HCS, LOW);
    SPI.transferBytes(SPI_frame, NULL, SPI_frame_size);
    digitalWrite(SPI2_HCS, HIGH);
}
