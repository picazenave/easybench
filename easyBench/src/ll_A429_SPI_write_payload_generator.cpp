#include "ll_A429_SPI_write_payload_generator.hpp"

// void convert_FA_frame_w
void convert_FA_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size)
{
  SPI_frame[0] = (0xFF & address) << 2;
  SPI_frame[1] = 0xFF & data_values[0];
}
// void convert_RDA_frame_w
void convert_RDA_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size)
{
  unsigned int convert_address =(0xFF & address) << 4;
  convert_address = OPCODE_WRITE_RDA | convert_address;
  SPI_frame[0] = convert_address >> 8;
  SPI_frame[1] = convert_address & 0xFF;
  for(int i=2;i<SPI_frame_size;i++)
  {
    SPI_frame[i] = data_values[i-2]; 
  }
}
// void convert_MAP_frame_w
void convert_MAP_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], unsigned char SPI_frame2[], int SPI_frame_size2)
{
  SPI_frame[0] = OPCODE_WRITE_MAP;
  SPI_frame[1] = address >> 8;
  SPI_frame[2] = address & 0xFF;    

  SPI_frame2[0] = OPCODE_WRITE_REGISTER_AT_MAP;
  for(int i=1;i<SPI_frame_size2;i++)
  {
    SPI_frame2[i] = data_values[i-1];
  }
}

void convert_TRANSMIT_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size)
{
  unsigned int convert_address = OPCODE_WRITE_TRANSMIT | (address & 0b111);
  SPI_frame[0] = convert_address;
  for(int i=1;i<SPI_frame_size;i++)
  {
    SPI_frame[i] = data_values[i-1]; 
  }
}
