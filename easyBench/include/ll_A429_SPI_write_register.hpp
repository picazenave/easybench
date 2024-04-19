#ifndef LL_A429_SPI_WRITE_REGISTER_HPP

#define LL_A429_SPI_WRITE_REGISTER_HPP

#include "ll_A429_SPI_write_payload_generator.hpp"

void write_register_value2(unsigned char SPI_frame[], unsigned int SPI_frame_size);

int write_register2(unsigned int address, unsigned char data_values[], int nb_data_bytes, int write_mode);

#endif