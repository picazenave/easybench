#ifndef LL_A429_SPI_COMMON_HPP

#define LL_A429_SPI_COMMON_HPP

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>

// Verbose level (0 = ERROR, 1 = INFO, 2 = WARNING, 3 = DEBUG)
#define VERBOSE 0

// SPI2 Pins
#define SPI2_MOSI 11
#define SPI2_MISO 13
#define SPI2_SCK 12
#define SPI2_HCS 10
#define SPI2_MRST 5
#define SPI2_READY 6
#define SPI2_RUN 7

// R/W mode
#define FAST_ACCESS 100
#define FA 100
#define REGISTER_DIRECT_ADDRESSING 101
#define RDA 101
#define MEMORY_ADDRESS_POINTER 102
#define MAP 102
#define FIFO 103
#define TRANSMIT 104

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

// Opcodes for MAP
#define OPCODE_READ_MAP 0x90 // Pas encore utilisé 
#define OPCODE_WRITE_MAP 0x98
#define OPCODE_READ_REGISTER_AT_MAP 0x80
#define OPCODE_WRITE_REGISTER_AT_MAP 0x88

// Sizes Register address (in bytes)
#define SIZE_ADDRESS_REGISTER 2

// Registers Addresses
#define ADD_MCR_W   0x8000  // MASTER CONTROL REGISTER ADDRESS WRITE  ->11XX0000
#define ADD_MCR_R   0x8001  // MASTER CONTROL REGISTER ADDRESS READ
#define ADD_MSR     0x8002  // MASTER STATUS REGISTER ADDRESS READ    ->XX110000
#define ADD_SELF_TEST_LOOPBACK 0x8049

// Receive control registers Addresses
#define ADD_CTR_RX0 0x8020  
#define ADD_CTR_RX1 0x8021  
#define ADD_CTR_RX2 0x8022  
#define ADD_CTR_RX3 0x8023  
#define ADD_CTR_RX4 0x8024  
#define ADD_CTR_RX5 0x8025  
#define ADD_CTR_RX6 0x8026  
#define ADD_CTR_RX7 0x8027  
#define ADD_CTR_RX8 0x8028  
#define ADD_CTR_RX9 0x8029  
#define ADD_CTR_RX10 0x802A  
#define ADD_CTR_RX11 0x802B  
#define ADD_CTR_RX12 0x802C  
#define ADD_CTR_RX13 0x802D  
#define ADD_CTR_RX14 0x802E  
#define ADD_CTR_RX15 0x802F 

// Transmit control registers Addresses
#define ADD_CTR_TX0 0x8030  
#define ADD_CTR_TX1 0x8031  
#define ADD_CTR_TX2 0x8032  
#define ADD_CTR_TX3 0x8033  
#define ADD_CTR_TX4 0x8034  
#define ADD_CTR_TX5 0x8035  
#define ADD_CTR_TX6 0x8036  
#define ADD_CTR_TX7 0x8037

#define NB_RX_CHANNELS 16
#define NB_TX_CHANNELS 8

#define NB_BYTES_PER_FRAME 4
#define NB_BITS_PER_BYTE 8

struct TRX_struct
{
    uint32_t words[32];
    uint32_t timestamp;
    uint32_t nb_words;
    uint32_t channel_number;
};

struct TRX_struct_test
{
    uint32_t timestamp;
    uint32_t channel_number;
    uint32_t words;
};


// Verbosity initialization[-1 -> -100]
// Verbosity reading [-101 -> -200]
// Verbosity writing [-201 -> -300]
// Errors initialization[-1001 -> -2000]
// Errors reading [-1101 -> -1200]
// Errors writing [-1201 -> -1300]
enum return_value{
  // Verbosity
    // Initialization
  SPI2_PINS_INIT = -1,
  READY = -2,
  SPI2_SETTINGS_INIT = -3,
  RUN = -4,
  CONFIG_MCR_REG = -5,
  CONFIG_TX_CTR_REG = -6,
  CONFIG_RX_CTR_REG = -7,
  CONFIG_LOOK_UP_REG = -8,
  CONFIG_SPI2 = -9,
  CONFIG_TRX = -10,
  
    // Reading
  FA_R = -101,
  RDA_R = -102,
  MAP_R = -103,
  FIFO_R = -104,
  READ_RX_CHANNEL = -105,
  FIFO_R2 = -106,
  READ_RX_FIFO_COUNT = 107,
  CHAR_TO_UINT32 = -108,

    // Writing
  FA_W = -201,
  RDA_W = -202,
  MAP_W = -203,
  TRANSMIT_W = -204,
  WRITE_RX_CHANNEL = -205,
  UINT32_TO_CHAR = -206,

  // Errors
    // Initialization
  NOT_READY = -1001,
  NOT_CONFIG_MCR_REG = -1002,
  NOT_CONFIG_TX_CTR_REG = -1003,
  NOT_NB_TX_CHANNEL = -1004,
  NOT_CONFIG_RX_CTR_REG = -1005,
  NOT_NB_RX_CHANNEL = -1006,
  NOT_CONFIG_LOOK_UP_REG = -1007,
  NOT_NB_RX_CHANNEL2 = -1008,
  NOT_TRX_MODE = -1009,
  NOT_CONFIG_STRUCT = -1010,

    // Reading
  NOT_READ_MODE = -1101,
  NOT_FA_R = -1102,
  NOT_RDA_R = -1103,
  NOT_MAP_R = -1104,
  NOT_FIFO_R = -1105,
  NOT_NB_BYTES_TO_READ = -1106,
  NOT_READ_RX_CHANNEL = -1107,
  NOT_NB_BYTES_TO_READ2 = -1108,
  NOT_NB_RX_CHANNEL3 = -1109,

    // Writing
  NOT_WRITE_MODE = -1201,
  NOT_NB_DATA_BYTE = -1202,
  NOT_FA_W = -1203,
  NOT_RDA_W = -1204,
  NOT_MAP_W = -1205,
  NOT_TRANSMIT_W = -1206,
  NOT_NB_DATA_BYTES = -1207,
  NOT_WRITE_RX_CHANNEL = -1208,


  NOT_FAST_ACCESS_W = -8,
  NOT_REGISTER_DIRECT_ADDRESSING_W = -9,
  NOT_DATA_SIZE = -10
};

/**
 * @name transmit_SPI_frame
 * @brief Transmits a frame of data over the SPI2 interface.
 * @details
 * 1. Asserting the SPI2_HCS pin to LOW, indicating the beginning of an SPI communication session.
 * 2. Calling `SPI.transferBytes(SPI_frame, NULL, SPI_frame_size)` to transmit the data frame.
 * 3. De-asserting the SPI2_HCS pin to HIGH after the data transmission is complete. This action signifies the end of the communication session.
 * @param {unsigned char[]} SPI_frame - An array containing the SPI frame data to be transmitted.
 * @param {unsigned int} SPI_frame_size - The size of the SPI frame in bytes.
 */
void transmit_SPI_frame(unsigned char SPI_frame[], unsigned int frame_size);

#endif