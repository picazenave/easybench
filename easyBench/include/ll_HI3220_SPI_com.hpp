#ifndef LL_HI3220_SPI_COM_HPP

#define LL_HI3220_SPI_COM_HPP

#include <ll_HI3220_common.hpp>

struct TRX_struct
{
    uint32_t timestamp;
    uint32_t channel_number;
    uint32_t words;
}; 

#define NB_BYTES_PER_A429_WORD 4
#define NB_BITS_PER_BYTE 8

#define NB_TX_CHANNELS_MAX 8
#define NB_RX_CHANNELS_MAX 16

// Sizes Register address (in bytes)
#define SIZE_ADDRESS_REGISTER 2
#define SIZE_REGISTER 1

// R/W mode
#define FAST_ACCESS 100
#define FA 100
#define REGISTER_DIRECT_ADDRESSING 101
#define RDA 101
#define MEMORY_ADDRESS_POINTER 102
#define MAP 102
#define FIFO_RX 103
#define TRANSMIT_TX 104

//Opcodes SPI
#define OPCODE_READ_RDA 0xE000
#define OPCODE_WRITE_RDA 0xD800
#define OPCODE_READ_MAP 0x90 // Pas encore utilisé 
#define OPCODE_WRITE_MAP 0x98
#define OPCODE_READ_REGISTER_AT_MAP 0x80
#define OPCODE_WRITE_REGISTER_AT_MAP 0x88
#define OPCODE_WRITE_TRANSMIT_TX 0xA0

// Sizes opcodes SPI commands (in bytes)
#define SIZE_OPCODE_READ_FA 1
#define SIZE_OPCODE_WRITE_FA 1
#define SIZE_OPCODE_READ_RDA 2
#define SIZE_OPCODE_WRITE_RDA 2
#define SIZE_OPCODE_READ_MAP 1 // Pas encore utilisé 
#define SIZE_OPCODE_WRITE_MAP 1 // Taille de l'opcode qui charger la MAP 
#define SIZE_OPCODE_READ_MAP_MEMORY 1
#define SIZE_OPCODE_WRITE_MAP_MEMORY 1
#define SIZE_OPCODE_READ_FIFO 2
#define SIZE_OPCODE_WRITE_TRANSMIT 1

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

// Registers Addresses
#define ADD_MCR_W   0x8000  // MASTER CONTROL REGISTER ADDRESS WRITE  ->11XX0000
#define ADD_MCR_R   0x8001  // MASTER CONTROL REGISTER ADDRESS READ
#define ADD_MSR     0x8002  // MASTER STATUS REGISTER ADDRESS READ    ->XX110000
#define ADD_SELF_TEST_LOOPBACK 0x8049

/**
 * @name send_SPI_frame
 * @brief Send a frame of data over the SPI interface.
 * @details
 * 1. Asserting the HCS pin to LOW, indicating the beginning of an SPI communication session.
 * 2. Calling `SPI.transferBytes(SPI_frame, NULL, SPI_frame_size)` to send the data frame.
 * 3. De-asserting the HCS pin to HIGH after the data transmission is complete. This action signifies the end of the communication session.
 * @param {unsigned char[]} SPI_frame - An array containing the SPI frame data to be sended.
 * @param {unsigned int} SPI_frame_size - The size of the SPI frame in bytes.
 */
inline void write_register_value(unsigned char SPI_frame[], unsigned int frame_size);

void read_register_value(unsigned char SPI_frame[], unsigned char readed_values[], unsigned int SPI_frame_size, int nb_bytes_to_read);

void convert_FA_frame_r(unsigned int address, unsigned char SPI_frame[], int SPI_frame_size);

void convert_RDA_frame_r(unsigned int address, unsigned char SPI_frame[], int SPI_frame_size);

void convert_MAP_frame_r(unsigned int address, unsigned char SPI_frame[], unsigned char SPI_frame2[], int SPI_frame_size);

void convert_FIFO_frame_r(unsigned char channel, unsigned char SPI_frame[], int SPI_frame_size);

int read_register_FA(unsigned int address, unsigned char readed_values[]);

int read_register_RDA(unsigned int address, unsigned char readed_values[]);

int read_register_MAP(unsigned int address, unsigned char readed_values[], int nb_bytes_to_read);

int read_register_FIFO_RX(unsigned int address, unsigned char readed_values[]);

int read_register(unsigned int address, unsigned char readed_values[], int nb_bytes_to_read, int read_mode);

void convert_FA_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size);

void convert_RDA_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size);

void convert_MAP_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], unsigned char SPI_frame2[], int SPI_frame_size2);

void convert_TRANSMIT_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size);

int write_register_FA(unsigned int address, unsigned char data_values[]);

int write_register_RDA(unsigned int address, unsigned char data_values[]);

int write_register_MAP(unsigned int address, unsigned char data_values[], int nb_data_bytes);

int write_register_TX(unsigned int address, unsigned char data_values[]);

int write_register(unsigned int address, unsigned char data_values[], int nb_data_bytes, int write_mode);

#endif