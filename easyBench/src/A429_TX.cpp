#include "A429_TX.hpp"
#include <A429_timestamp.hpp>

void frame_uint32_t_to_char(unsigned char char_values[], uint32_t uint32_values)
{
  for(int i=0; i < NB_BYTES_PER_A429_WORD;i++)
  {
    char_values[i] = uint32_values >> NB_BITS_PER_BYTE*(3-i);
  }
}

void send_TX_channel(unsigned char channel, TRX_struct *TX_struct)
{
  unsigned char frame_values[NB_BYTES_PER_A429_WORD];
  frame_uint32_t_to_char(frame_values, TX_struct->words);
  write_register_TX(channel, frame_values);
}

void send_multi_TX_channels(unsigned char nb_channel, TRX_struct TX_struct[])
{
  for(int i=0;i<nb_channel;i++)
  {
    send_TX_channel(i, &TX_struct[i]);
  } 
}