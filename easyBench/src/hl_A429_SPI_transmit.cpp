#include "hl_A429_SPI_transmit.hpp"

int frame_uint32_t_to_char(unsigned char char_values[], uint32_t uint32_values[], unsigned int nb_words)
{
  for(int j=0; j < nb_words; j++)
  {
    for(int i=0; i < NB_BYTES_PER_FRAME;i++)
    {
      char_values[i+j*NB_BYTES_PER_FRAME] = uint32_values[j]>>NB_BITS_PER_BYTE*(3-i);
    }
  }
  return UINT32_TO_CHAR;
}

int frame_uint32_t_to_char2(unsigned char char_values[], uint32_t uint32_values, unsigned int nb_words)
{
  for(int i=0; i < NB_BYTES_PER_FRAME;i++)
  {
    char_values[i] = uint32_values >> NB_BITS_PER_BYTE*(3-i);
  }
  return UINT32_TO_CHAR;
}

int send_TX_channel(unsigned char channel, TRX_struct *TX_struct)
{
//Il manque la partie ou on récupère les données de la queue TX pour les mettre dans notre struct
  uint32_t time = 0;
  get_timestamp(&time);
  if(TX_struct->timestamp <= time)
  {
    unsigned int size_frame = (TX_struct->nb_words)*4;
    unsigned char frame_values[size_frame];
    frame_uint32_t_to_char(frame_values, TX_struct->words, TX_struct->nb_words);
    write_register2(channel, frame_values, size_frame, TRANSMIT);
    return 0;
  }
  return 0;
}

int send_TX_channel2(unsigned char channel, TRX_struct_test *TX_struct)
{
//Il manque la partie ou on récupère les données de la queue TX pour les mettre dans notre struct
  uint32_t time = 0;
  get_timestamp(&time);
  if(TX_struct->timestamp <= time)
  {
    unsigned int size_frame = NB_BYTES_PER_FRAME;
    unsigned char frame_values[size_frame];
    frame_uint32_t_to_char2(frame_values, TX_struct->words, 1);
    write_register2(channel, frame_values, size_frame, TRANSMIT);
    return 0;
  }
  return 0;
}

int send_multi_TX_channels(unsigned char nb_channel, TRX_struct TX_struct[])
{
  for(int i=0;i<nb_channel;i++)
  {
    send_TX_channel(i, &TX_struct[i]);
  }
  return 0;    
}

int send_multi_TX_channels2(unsigned char nb_channel, TRX_struct_test TX_struct[])
{
  for(int i=0;i<nb_channel;i++)
  {
    send_TX_channel2(i, &TX_struct[i]);
  }
  return 0;    
}
