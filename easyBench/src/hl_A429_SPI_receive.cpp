#include "hl_A429_SPI_receive.hpp"

int frame_char_to_uint32_t(unsigned char char_values[], uint32_t uint32_values[], unsigned int nb_words)
{
  for(int j=0; j < nb_words; j++)
  {
    uint32_values[j] = 0;
    for(int i=0; i < NB_BYTES_PER_FRAME;i++)
    {
      uint32_values[j] = uint32_values[j] + (char_values[NB_BYTES_PER_FRAME*j+i]<<NB_BITS_PER_BYTE*(3-i));
    }
  }
  return CHAR_TO_UINT32;
}

int frame_char_to_uint32_t2(unsigned char char_values[], uint32_t *uint32_value, unsigned int nb_words)
{
  *uint32_value = 0;
  for(int i=0; i < NB_BYTES_PER_FRAME;i++)
  {
    *uint32_value = *uint32_value | (char_values[i]<<NB_BITS_PER_BYTE*(3-i));
  }
  return CHAR_TO_UINT32;
}

int read_FIFO_channel_RX(unsigned char channel, unsigned char readed_values[], int nb_bytes_to_read)
{
  if((nb_bytes_to_read)%4 == 0)
  {
    int SPI_frame_size = SIZE_OPCODE_READ_FIFO + nb_bytes_to_read; 
    unsigned char SPI_frame[SPI_frame_size];
    convert_FIFO_frame_r(channel, SPI_frame, SPI_frame_size);
    read_register_value2(SPI_frame, readed_values, SPI_frame_size, nb_bytes_to_read);
    return FIFO_R2;  
  }
  else
  {
    return NOT_NB_BYTES_TO_READ2; //ADAPTER MESSAGE D'ERREUR : lecteur par paquet de 4 octets
  }
}

int read_FIFO_count_RX(unsigned char channel, uint32_t *value)
{
  if(channel <= 15 && channel >=0)
  {
    unsigned char readed_value[1];
    read_register2(ADD_FIFO_COUNT_RX0 + channel, readed_value,1,MAP);
    *value = readed_value[0];
    return READ_RX_FIFO_COUNT;
  }
  else
  {
    return NOT_NB_RX_CHANNEL3;
  }
}

int read_channel_RX(unsigned char channel, TRX_struct *RX_struct)
{
  memset(RX_struct, 0, sizeof(TRX_struct));
  read_FIFO_count_RX(channel, &RX_struct->nb_words);
  if(RX_struct->nb_words !=0)
  {
    get_timestamp(&RX_struct->timestamp);
    RX_struct->channel_number = channel;
    unsigned int size_values = (RX_struct->nb_words)*4;
    unsigned char readed_values[size_values];
    read_FIFO_channel_RX(channel, readed_values, size_values);
    frame_char_to_uint32_t(readed_values,RX_struct->words, RX_struct->nb_words);
    return 0; //ATTENTION A LA VALEUR DE RETURN
  }
  else
  {
    return 0;
  }
}

int read_channel_RX2(unsigned char channel, TRX_struct_test *RX_struct)
{
  memset(RX_struct, 0, sizeof(TRX_struct_test));
  uint32_t nb_words = 0;
  read_FIFO_count_RX(channel, &nb_words);
  if(nb_words != 0)
  {
    get_timestamp(&RX_struct->timestamp);
    RX_struct->channel_number = channel;
    unsigned int size_values = NB_BYTES_PER_FRAME;
    unsigned char readed_values[size_values];
    read_FIFO_channel_RX(channel, readed_values, size_values);
    frame_char_to_uint32_t2(readed_values,&RX_struct->words, 1);
    return 0; //ATTENTION A LA VALEUR DE RETURN
  }
  else
  {
    return 0;
  }
}

int read_multi_RX_channels(unsigned char nb_channel, TRX_struct RX_struct[])
{
  for(int i=0;i<nb_channel;i++)
  {
    read_channel_RX(i, &RX_struct[i]);
  }
  return 0;
}

int read_multi_RX_channels2(unsigned char nb_channel, TRX_struct_test RX_struct[])
{
  for(int i=0;i<nb_channel;i++)
  {
    read_channel_RX2(i, &RX_struct[i]);
  }
  return 0;
}