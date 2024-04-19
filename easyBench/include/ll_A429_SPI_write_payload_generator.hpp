#ifndef LL_A429_SPI_WRITE_PAYLOAD_GENERATOR_HPP

#define LL_A429_SPI_WRITE_PAYLOAD_GENERATOR_HPP

#include <ll_A429_SPI_common.hpp>

//Opcodes SPI
#define OPCODE_WRITE_RDA 0xD800
#define OPCODE_WRITE_TRANSMIT 0xA0

void convert_FA_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size);

void convert_RDA_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size);

void convert_MAP_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], unsigned char SPI_frame2[], int SPI_frame_size2);

void convert_TRANSMIT_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size);

#endif