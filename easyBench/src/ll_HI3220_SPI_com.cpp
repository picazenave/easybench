#include "ll_HI3220_SPI_com.hpp"

inline void write_register_value(unsigned char SPI_frame[], unsigned int SPI_frame_size)
{
    digitalWrite(PIN_SPI_HCS, LOW);
    SPI.transferBytes(SPI_frame, NULL, SPI_frame_size);
    digitalWrite(PIN_SPI_HCS, HIGH);
}

void read_register_value(unsigned char SPI_frame[], unsigned char readed_values[], unsigned int SPI_frame_size, int nb_bytes_to_read)
{
  unsigned char read_data[SPI_frame_size];
  digitalWrite(PIN_SPI_HCS, LOW);
  SPI.transferBytes(SPI_frame, read_data, SPI_frame_size);
  digitalWrite(PIN_SPI_HCS, HIGH);
  for(int i=SPI_frame_size - nb_bytes_to_read; i<SPI_frame_size; i++)
  {
    readed_values[i-(SPI_frame_size - nb_bytes_to_read)] = read_data[i];
  }
}

void convert_FA_frame_r(unsigned int address, unsigned char SPI_frame[], int SPI_frame_size)
{
  SPI_frame[0] = (0xFF & address) << 2;
  for(int i=1;i<SPI_frame_size;i++) 
  {
    SPI_frame[i] = 0x00;
  }
}

void convert_RDA_frame_r(unsigned int address, unsigned char SPI_frame[], int SPI_frame_size)
{
  unsigned int convert_address =(0xFF & address) << 4;
  convert_address = OPCODE_READ_RDA | convert_address;
  SPI_frame[0] = convert_address >> 8;
  SPI_frame[1] = convert_address & 0xFF;
  for(int i=2;i<SPI_frame_size;i++)
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

int read_register_FA(unsigned int address, unsigned char readed_values[])
{
  if(address <= 0x801F && address >= 0x8000) 
  {
    int SPI_frame_size = SIZE_OPCODE_READ_FA + SIZE_REGISTER;
    unsigned char SPI_frame[SPI_frame_size];
    convert_FA_frame_r(address, SPI_frame, SPI_frame_size);
    read_register_value(SPI_frame, readed_values, SPI_frame_size, SIZE_REGISTER);
    return 0; //SUCCESS
  }
  else
  {
    return -1; //FAIL
  }
}

int read_register_RDA(unsigned int address, unsigned char readed_values[])
{
  if(address <= 0x8087 && address >= 0x8000)
  {
    int SPI_frame_size = SIZE_OPCODE_READ_RDA + SIZE_REGISTER;
    unsigned char SPI_frame[SPI_frame_size];
    convert_RDA_frame_r(address, SPI_frame, SPI_frame_size);
    read_register_value(SPI_frame, readed_values, SPI_frame_size, SIZE_REGISTER);
    return 0;   //SUCCESS 
  }
  else
  {
    return -1;   //FAIL
  }
}

int read_register_MAP(unsigned int address, unsigned char readed_values[], int nb_bytes_to_read)
{
  if(address <= 0xFFFF && address >= 0x0000)
  {
    int SPI_frame_pointer_size = SIZE_OPCODE_WRITE_MAP + SIZE_ADDRESS_REGISTER;
    int SPI_frame_read_size = SIZE_OPCODE_READ_MAP_MEMORY + nb_bytes_to_read;
    unsigned char SPI_frame_pointer[SPI_frame_pointer_size];
    unsigned char SPI_frame_read[SPI_frame_read_size];
    convert_MAP_frame_r(address, SPI_frame_pointer, SPI_frame_read, SPI_frame_read_size);
    write_register_value(SPI_frame_pointer, SPI_frame_pointer_size);
    read_register_value(SPI_frame_read, readed_values, SPI_frame_read_size, nb_bytes_to_read);
    return 0;   //SUCCESS
  }
  else
  {
    return -1;  //FAIL
  }
}

int read_register_FIFO_RX(unsigned int address, unsigned char readed_values[])
{
  if((address & 0xF0FF) == 0x7000)
  {

    int SPI_frame_size = SIZE_OPCODE_READ_FIFO + NB_BYTES_PER_A429_WORD; 
    unsigned char SPI_frame[SPI_frame_size];
    unsigned char channel;
    channel = (address & 0x0F00) >> 8;
    convert_FIFO_frame_r(channel, SPI_frame, SPI_frame_size);
    read_register_value(SPI_frame, readed_values, SPI_frame_size, NB_BYTES_PER_A429_WORD);
    return 0; //SUCCESS
  }
  else
  {
    return -1;  //FAIL
  }
}

int read_register(unsigned int address, unsigned char readed_values[], int nb_bytes_to_read, int read_mode)
{
  switch(read_mode)
  {
    case FAST_ACCESS :
      read_register_FA(address, readed_values);
      break;
    case REGISTER_DIRECT_ADDRESSING :
      read_register_RDA(address, readed_values);
      break;
    case MEMORY_ADDRESS_POINTER :
      read_register_MAP(address, readed_values, nb_bytes_to_read);
      break;
    case FIFO_RX :
      read_register_FIFO_RX(address, readed_values);
      break;
    default :
      return -1;    //FAIL
  }
  return 0;
}

void convert_FA_frame_w(unsigned int address, unsigned char data_values[], unsigned char SPI_frame[], int SPI_frame_size)
{
  SPI_frame[0] = (0xFF & address) << 2;
  SPI_frame[1] = 0xFF & data_values[0];
}

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
  unsigned int convert_address = OPCODE_WRITE_TRANSMIT_TX | (address & 0b111);
  SPI_frame[0] = convert_address;
  for(int i=1;i<SPI_frame_size;i++)
  {
    SPI_frame[i] = data_values[i-1]; 
  }
}

int write_register_FA(unsigned int address, unsigned char data_values[])
{
  if(address == ADD_MCR_W)       //Check if fast access register 
  {
      int SPI_frame_size = SIZE_OPCODE_WRITE_FA + SIZE_REGISTER;
      unsigned char SPI_frame[SPI_frame_size];
      convert_FA_frame_w(address, data_values, SPI_frame, SPI_frame_size);
      write_register_value(SPI_frame, SPI_frame_size);
      return 0; //SUCCESS
  }
  else
  {
    return -1;  //FAIL
  }
}

int write_register_RDA(unsigned int address, unsigned char data_values[])
{
  if(address <= 0x807F && address >= 0x8000)      //Check if RDA register
  {
    int SPI_frame_size = SIZE_OPCODE_WRITE_RDA + SIZE_REGISTER;
    unsigned char SPI_frame[SPI_frame_size];
    convert_RDA_frame_w(address, data_values, SPI_frame, SPI_frame_size);
    write_register_value(SPI_frame, SPI_frame_size);
    return 0; //SUCCESS 
  }
  else
  {
    return -1;  //FAIL
  }
}

int write_register_MAP(unsigned int address, unsigned char data_values[], int nb_data_bytes)
{
  if(address >= 0x0000 && address <= 0x8000 || address >= 0x8020 && address <= 0x8040 || address >= 0x8049 && address <= 0x8077 || address >= 0x807E && address <= 0x807F || address >=0x8088 && address <= 0xFFFF)      //Check if RDA register
  {
    int SPI_frame_size = SIZE_OPCODE_WRITE_MAP + SIZE_ADDRESS_REGISTER;
    int SPI_frame_size2 = SIZE_OPCODE_WRITE_MAP_MEMORY + nb_data_bytes;
    unsigned char SPI_frame[SPI_frame_size];
    unsigned char SPI_frame2[SPI_frame_size2];
    convert_MAP_frame_w(address, data_values, SPI_frame, SPI_frame2, SPI_frame_size2);
    write_register_value(SPI_frame, SPI_frame_size);
    write_register_value(SPI_frame2, SPI_frame_size2);
    return 0; //SUCCESS
  }
  else
  {
    return -1;  //FAIL
  }
}

int write_register_TX(unsigned int address, unsigned char data_values[])
{
  if(address >= 0 && address <= 7)      //Check if transmit channel (it's channel and not the address)
  {
    int SPI_frame_size = SIZE_OPCODE_WRITE_TRANSMIT + NB_BYTES_PER_A429_WORD; 
    unsigned char SPI_frame[SPI_frame_size];
    convert_TRANSMIT_frame_w(address, data_values, SPI_frame, SPI_frame_size);
    write_register_value(SPI_frame, SPI_frame_size);
    return 0;   //SUCCESS
  }
  else
  {
    return -1;  //FAIL
  }  
}

int write_register(unsigned int address, unsigned char data_values[], int nb_data_bytes, int write_mode)
{
  switch(write_mode)
  {
    case FAST_ACCESS :
      write_register_FA(address, data_values);
      break;
    case REGISTER_DIRECT_ADDRESSING :
      write_register_RDA(address, data_values);
      break;
    case MEMORY_ADDRESS_POINTER :
      write_register_MAP(address, data_values, nb_data_bytes);
      break;
    case TRANSMIT_TX :
      write_register_TX(address, data_values);
      break;
    default :
      return -1;  //FAIL
  }
  return 0;
}