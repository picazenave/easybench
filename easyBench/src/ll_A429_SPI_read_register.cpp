#include "ll_A429_SPI_read_register.hpp"

void read_register_value2(unsigned char SPI_frame[], unsigned char readed_values[], unsigned int SPI_frame_size, int nb_bytes_to_read)
{
  unsigned char read_data[SPI_frame_size];
  digitalWrite(SPI2_HCS, LOW);
  SPI.transferBytes(SPI_frame, read_data, SPI_frame_size);
  digitalWrite(SPI2_HCS, HIGH);
  for(int i=SPI_frame_size - nb_bytes_to_read; i<SPI_frame_size; i++)
  {
    readed_values[i-(SPI_frame_size - nb_bytes_to_read)] = read_data[i];
  }
}

int read_register2(unsigned int address, unsigned char readed_values[], int nb_bytes_to_read, int read_mode)
{
  switch(read_mode)
  {
    case FAST_ACCESS :
      if(address <= 0x801F && address >= 0x8000)       //Check if fast access register 
      {
        int SPI_frame_size = SIZE_OPCODE_READ_FA + nb_bytes_to_read;
        unsigned char SPI_frame[SPI_frame_size];
        convert_FA_frame_r(address, SPI_frame, SPI_frame_size);
        read_register_value2(SPI_frame, readed_values, SPI_frame_size, nb_bytes_to_read);
        return FA_R; 
      }
      else
      {
        return NOT_FA_R;
      }
    case REGISTER_DIRECT_ADDRESSING :
      if(address <= 0x8087 && address >= 0x8000)      //Check if RDA register
      {
        int SPI_frame_size = SIZE_OPCODE_READ_RDA + nb_bytes_to_read;
        unsigned char SPI_frame[SPI_frame_size];
        convert_RDA_frame_r(address, SPI_frame, SPI_frame_size);
        read_register_value2(SPI_frame, readed_values, SPI_frame_size, nb_bytes_to_read);
        return RDA_R; 
      }
      else
      {
        return NOT_RDA_R;
      }
    case MEMORY_ADDRESS_POINTER :
      if(address <= 0xFFFF && address >= 0x0000)      //Check if RDA register
      {
        int SPI_frame_size = SIZE_OPCODE_WRITE_MAP + SIZE_ADDRESS_REGISTER;
        int SPI_frame_size2 = SIZE_OPCODE_READ_MAP_MEMORY + nb_bytes_to_read;
        unsigned char SPI_frame[SPI_frame_size];
        unsigned char SPI_frame2[SPI_frame_size2];
        convert_MAP_frame_r(address, SPI_frame, SPI_frame2, SPI_frame_size2);
        transmit_SPI_frame(SPI_frame, SPI_frame_size); // Changer le nom pour write_SPI_frame_MAP ??
        read_register_value2(SPI_frame2, readed_values, SPI_frame_size2, nb_bytes_to_read);
        return MAP_R;
      }
      else
      {
        return NOT_MAP_R;
      }
    case FIFO :
      if((address & 0xF0FF) == 0x7000)
      {
        if((nb_bytes_to_read)%4 == 0 && nb_bytes_to_read !=0)
        {
          int SPI_frame_size = SIZE_OPCODE_READ_FIFO + nb_bytes_to_read; 
          unsigned char SPI_frame[SPI_frame_size];
          unsigned char channel;
          convert_address_to_channel_RX(address, &channel);
          convert_FIFO_frame_r(channel, SPI_frame, SPI_frame_size);
          read_register_value2(SPI_frame, readed_values, SPI_frame_size, nb_bytes_to_read);
          return FIFO_R; 
        }
        else
        {
          return NOT_NB_BYTES_TO_READ;
        }
      }
      else
      {
        return NOT_FIFO_R;
      }
    default :
      return NOT_READ_MODE;
  }
}
