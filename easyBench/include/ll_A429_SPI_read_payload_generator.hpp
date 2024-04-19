#ifndef LL_A429_SPI_READ_PAYLOAD_GENERATOR_HPP

#define LL_A429_SPI_READ_PAYLOAD_GENERATOR_HPP

#include <ll_A429_SPI_common.hpp>

//Opcodes SPI
#define OPCODE_READ_RDA 0xE000

/*
Name : convert_FA_frame_r
Goal : Create FAST ACCESS frame
Used in main : NO (test function)
Input(s) :
- unsigned int address : Address of fast access register
- unsigned char SPI_frame[] : SPI frame array 
- int SPI_frame_size : size of SPI frame array
Output(s) : NULL
Description :
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAA Faire la description à l'aide du chat AAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
*/
void convert_FA_frame_r(unsigned int address, unsigned char SPI_frame[], int SPI_frame_size);

void convert_RDA_frame_r(unsigned int address, unsigned char SPI_frame[], int SPI_frame_size);

void convert_MAP_frame_r(unsigned int address, unsigned char SPI_frame[], unsigned char SPI_frame2[], int SPI_frame_size);

void convert_FIFO_frame_r(unsigned char channel, unsigned char SPI_frame[], int SPI_frame_size);

void convert_address_to_channel_RX(unsigned int address, unsigned char *channel);

#endif