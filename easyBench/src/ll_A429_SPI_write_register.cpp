#include "ll_A429_SPI_write_register.hpp"

// void write_register_value2 - DONE
void write_register_value2(unsigned char SPI_frame[], unsigned int SPI_frame_size)
{
  digitalWrite(SPI2_HCS, LOW);
  SPI.transferBytes(SPI_frame, NULL, SPI_frame_size);
  digitalWrite(SPI2_HCS, HIGH);
} 

// void write_register2
    //Pour le FA, on ne peut ecrire que sur le MCR donc 1 seule adresse compatible et pas plusieurs données
int write_register2(unsigned int address, unsigned char data_values[], int nb_data_bytes, int write_mode)
{
  switch(write_mode)
  {
    case FAST_ACCESS :
      if(address == ADD_MCR_W)       //Check if fast access register 
      {
        if(nb_data_bytes == 1)
        {
          int SPI_frame_size = SIZE_OPCODE_WRITE_FA + nb_data_bytes;
          unsigned char SPI_frame[SPI_frame_size];
          convert_FA_frame_w(address, data_values, SPI_frame, SPI_frame_size);
          write_register_value2(SPI_frame, SPI_frame_size);
          return FA_W;
        }
        else
        {
          return NOT_NB_DATA_BYTE;
        } 
      }
      else
      {
        return NOT_FA_W;
      }
    case REGISTER_DIRECT_ADDRESSING :
      if(address <= 0x807F && address >= 0x8000)      //Check if RDA register
      {
        int SPI_frame_size = SIZE_OPCODE_WRITE_RDA + nb_data_bytes;
        unsigned char SPI_frame[SPI_frame_size];
        convert_RDA_frame_w(address, data_values, SPI_frame, SPI_frame_size);
        write_register_value2(SPI_frame, SPI_frame_size);
        return RDA_W; 
      }
      else
      {
        return NOT_RDA_W;
      }
    case MEMORY_ADDRESS_POINTER :
      if(address >= 0x0000 && address <= 0x8000 || address >= 0x8020 && address <= 0x8040 || address >= 0x8049 && address <= 0x8077 || address >= 0x807E && address <= 0x807F || address >=0x8088 && address <= 0xFFFF)      //Check if RDA register
      {
        int SPI_frame_size = SIZE_OPCODE_WRITE_MAP + SIZE_ADDRESS_REGISTER;
        int SPI_frame_size2 = SIZE_OPCODE_WRITE_MAP_MEMORY + nb_data_bytes;
        unsigned char SPI_frame[SPI_frame_size];
        unsigned char SPI_frame2[SPI_frame_size2];
        convert_MAP_frame_w(address, data_values, SPI_frame, SPI_frame2, SPI_frame_size2);
        transmit_SPI_frame(SPI_frame, SPI_frame_size);
        write_register_value2(SPI_frame2, SPI_frame_size2);
        return MAP_W;
      }
      else
      {
        return NOT_MAP_W;
      }
    case TRANSMIT :
      if(address >= 0 && address <= 7)      //Check if transmit channel (it's channel and not the address)
      {
        if((nb_data_bytes)%4 == 0 && nb_data_bytes !=0)
        {
          int SPI_frame_size = SIZE_OPCODE_WRITE_TRANSMIT + nb_data_bytes; 
          unsigned char SPI_frame[SPI_frame_size];
          convert_TRANSMIT_frame_w(address, data_values, SPI_frame, SPI_frame_size);
          write_register_value2(SPI_frame, SPI_frame_size);
          return TRANSMIT_W; 
        }
        else
        {
          return NOT_NB_DATA_BYTES;
        }
      }
      else
      {
        return NOT_TRANSMIT_W;
      }
    default :
      return NOT_WRITE_MODE;
  }
}