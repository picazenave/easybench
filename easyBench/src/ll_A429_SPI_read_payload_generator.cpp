#include "ll_A429_SPI_read_payload_generator.hpp"

void convert_FA_frame_r(unsigned int address, unsigned char SPI_frame[], int SPI_frame_size)
{
  SPI_frame[0] = (0xFF & address) << 2;
  for(int i=1;i<SPI_frame_size;i++) //Vérifier que le nb de bytes correspondent a ce que l'on veut (nb d'octets a lire sans les opcodes)
  {
    SPI_frame[i] = 0x00;
  }
}

void convert_RDA_frame_r(unsigned int address, unsigned char SPI_frame[], int SPI_frame_size)
{
  unsigned int convert_address =(0xFF & address) << 4; //0xE00 correspond à l'opcode RDA de lecture
  convert_address = OPCODE_READ_RDA | convert_address;
  SPI_frame[0] = convert_address >> 8;
  SPI_frame[1] = convert_address & 0xFF;
  for(int i=2;i<SPI_frame_size;i++)//Vérifier que le nb de bytes correspondent a ce que l'on veut (nb d'octets a lire sans les opcodes)
  {
    SPI_frame[i] = 0x00; 
  }
}

void convert_MAP_frame_r(unsigned int address, unsigned char SPI_frame[], unsigned char SPI_frame2[], int SPI_frame_size)
{
  SPI_frame[0] = OPCODE_WRITE_MAP;
  SPI_frame[1] = address >> 8;
  SPI_frame[2] = address & 0xFF;    

  SPI_frame2[0] = OPCODE_READ_REGISTER_AT_MAP;
  for(int i=1;i<SPI_frame_size;i++)
  {
    SPI_frame2[i] = 0x00;
  }
}

void convert_FIFO_frame_r(unsigned char channel, unsigned char SPI_frame[], int SPI_frame_size)
{
  SPI_frame[0] = 0xC0;
  SPI_frame[1] = channel << 4;
  for(int i=2;i<SPI_frame_size;i++)
  {
    SPI_frame[i] = 0x00;
  }
}

void convert_address_to_channel_RX(unsigned int address, unsigned char *channel)
{   
  *channel = (address & 0x0F00) >> 8;
}
