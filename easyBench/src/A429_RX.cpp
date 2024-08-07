#include "A429_RX.hpp"
#include <A429_timestamp.hpp>

inline void frame_char_to_uint32_t(unsigned char char_values[], uint32_t *uint32_value)
{
  *uint32_value = 0;
  for (int i = 0; i < NB_BYTES_PER_A429_WORD; i++)
  {
    *uint32_value = *uint32_value | (char_values[i] << NB_BITS_PER_BYTE * (3 - i));
  }
}

void read_FIFO_channel_RX(unsigned char channel, unsigned char readed_values[])
{
  int SPI_frame_size = SIZE_OPCODE_READ_FIFO + NB_BYTES_PER_A429_WORD;
  unsigned char SPI_frame[SPI_frame_size];
#if 0
    convert_FIFO_frame_r(channel, SPI_frame, SPI_frame_size);
    read_register_value(SPI_frame, readed_values, SPI_frame_size, NB_BYTES_PER_A429_WORD);
#else
  SPI_frame[0] = 0xC0;
  SPI_frame[1] = channel << 4;
  for (int i = 2; i < SPI_frame_size; i++)
  {
    SPI_frame[i] = 0x00;
  }

  unsigned char read_data[SPI_frame_size];
  digitalWrite(PIN_SPI_HCS, LOW);
  SPI.transferBytes(SPI_frame, read_data, SPI_frame_size);
  digitalWrite(PIN_SPI_HCS, HIGH);
  for (int i = SPI_frame_size - NB_BYTES_PER_A429_WORD; i < SPI_frame_size; i++)
  {
    readed_values[i - (SPI_frame_size - NB_BYTES_PER_A429_WORD)] = read_data[i];
  }
#endif
}

IRAM_ATTR int read_FIFO_count_RX(unsigned char channel, uint32_t *value)
{
  if (channel <= 15 && channel >= 0)
  {
    unsigned char readed_value[1];
#if 0
    read_register_MAP(ADD_FIFO_COUNT_RX0 + channel, readed_value, 1);
#else
    // 2 opcode + 1 fifo
    uint8_t SPI_frame_tx[3] = {0};
    uint8_t SPI_frame_rx[3] = {0};

    SPI_frame_tx[0] = (uint8_t)0xE6 + (channel & 0xF0);
    SPI_frame_tx[1] = ((0x8 + (channel & 0x0F)) << 4) & 0xF0; // & operation to ensure 0b0000

    digitalWrite(PIN_SPI_HCS, LOW);
    SPI.transferBytes(SPI_frame_tx, SPI_frame_rx, 3);
    digitalWrite(PIN_SPI_HCS, HIGH);
#endif
    *value = SPI_frame_rx[2];
    // if (SPI_frame_rx[2] != 0)
    // {
    //   for (int i = 0; i < 3; i++)
    //   {
    //     Serial0.printf("SPI_frame_tx[%d]=0x%02X |", i, SPI_frame_tx[i]);
    //   }
    //   for (int i = 0; i < 3; i++)
    //   {
    //     Serial0.printf("SPI_frame_rx[%d]=0x%02X |", i, SPI_frame_rx[i]);
    //   }
    //   Serial0.println("");
    // }
    return 0; // SUCCESS
  }
  else
  {
    return -1; // FAIL
  }
}

void receive_A429_word(unsigned char channel, TRX_struct *RX_struct)
{
  memset(RX_struct, 0, sizeof(TRX_struct));
  uint32_t nb_words = 0;
  read_FIFO_count_RX(channel, &nb_words);
  if (nb_words != 0)
  {
    get_timestamp(&RX_struct->timestamp);
    RX_struct->channel_number = channel;
    unsigned char readed_values[NB_BYTES_PER_A429_WORD];
    read_FIFO_channel_RX(channel, readed_values);
    frame_char_to_uint32_t(readed_values, &RX_struct->words);
  }
}

void receive_multi_RX_channels(unsigned char nb_channel, TRX_struct RX_struct[]) // TO MODIFY : pas sur de garder cette fonction
{
  for (int i = 0; i < nb_channel; i++)
  {
    receive_A429_word(i, &RX_struct[i]);
  }
}

// void read_channels_RX(unsigned char nb_channel, TRX_struct RX_struct[]) //TO MODIFY : fonction créée et peut etre celle a utiliser
// {
//   for(int i = 0; i < nb_channel; i++)
//   {
//     memset(RX_struct, 0, sizeof(TRX_struct));
//     uint32_t nb_words = 0;
//     read_FIFO_count_RX(i, &nb_words); //TO MODIFY : A VERIFIER
//     while(nb_words <= 0)
//     {
//       get_timestamp(&RX_struct[i].timestamp, offset);  //TO MODIFY : A VERIFIER
//       RX_struct[i].channel_number = i;
//       unsigned char readed_values[NB_BYTES_PER_A429_WORD];
//       read_FIFO_RX(i, readed_values);
//       frame_char_to_uint32_t(readed_values,&RX_struct->words);
//       nb_words--;
//     }
//   }
// }