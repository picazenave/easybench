#ifndef LL_A429_SPI_READ_REGISTER_HPP

#define LL_A429_SPI_READ_REGISTER_HPP

#include "ll_A429_SPI_read_payload_generator.hpp"

// Receive FIFO count registers Addresses
#define ADD_FIFO_COUNT_RX0 0x8068
#define ADD_FIFO_COUNT_RX1 0x8069
#define ADD_FIFO_COUNT_RX2 0x806A
#define ADD_FIFO_COUNT_RX3 0x806B
#define ADD_FIFO_COUNT_RX4 0x806C
#define ADD_FIFO_COUNT_RX5 0x806D
#define ADD_FIFO_COUNT_RX6 0x806E
#define ADD_FIFO_COUNT_RX7 0x806F
#define ADD_FIFO_COUNT_RX8 0x8070
#define ADD_FIFO_COUNT_RX9 0x8071
#define ADD_FIFO_COUNT_RX10 0x8072
#define ADD_FIFO_COUNT_RX11 0x8073
#define ADD_FIFO_COUNT_RX12 0x8074
#define ADD_FIFO_COUNT_RX13 0x8075
#define ADD_FIFO_COUNT_RX14 0x8076
#define ADD_FIFO_COUNT_RX15 0x8077


void read_register_value2(unsigned char SPI_frame[], unsigned char readed_values[], unsigned int SPI_frame_size, int nb_bytes_to_read);

int read_register2(unsigned int address, unsigned char readed_values[], int nb_bytes_to_read, int read_mode);

#endif