#include "ll_A429_SPI_received_frame_timestamp.hpp"

void get_timestamp(uint32_t *time)
{
    *time = micros();
}