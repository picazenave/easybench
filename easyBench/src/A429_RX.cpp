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

}

IRAM_ATTR int read_FIFO_count_RX(unsigned char channel, uint32_t *value)
{
  if (channel <= 15 && channel >= 0)
  {
    uint8_t SPI_frame_tx[3] = {0};
    uint8_t SPI_frame_rx[3] = {0};

    SPI_frame_tx[0] = (uint8_t)0xE6 + (channel & 0xF0);
    SPI_frame_tx[1] = ((0x8 + (channel & 0x0F)) << 4) & 0xF0; // & operation to ensure 0b0000

    digitalWrite(PIN_SPI_HCS, LOW);
    SPI.transferBytes(SPI_frame_tx, SPI_frame_rx, 3);
    digitalWrite(PIN_SPI_HCS, HIGH);
    *value = SPI_frame_rx[2];
    return 0; // SUCCESS
  }
  else
  {
    return -1; // FAIL
  }
}

IRAM_ATTR int read_ALL_FIFO_count_RX(unsigned char channel, uint8_t value[], uint32_t channel_count)
{
  if (channel <= 15 && channel >= 0)
  {
    uint8_t SPI_frame_tx[3 + NB_RX_CHANNELS_MAX] = {0};
    uint8_t SPI_frame_rx[3 + NB_RX_CHANNELS_MAX] = {0};

    SPI_frame_tx[0] = (uint8_t)0xE6 + (channel & 0xF0);
    SPI_frame_tx[1] = ((0x8 + (channel & 0x0F)) << 4) & 0xF0; // & operation to ensure 0b0000

    digitalWrite(PIN_SPI_HCS, LOW);
    SPI.transferBytes(SPI_frame_tx, SPI_frame_rx, 3 + channel_count);
    digitalWrite(PIN_SPI_HCS, HIGH);
    for (uint32_t i = 0; i < channel_count; i++)
    {
      value[i] = SPI_frame_rx[2 + i];
    }
    // if (value[1] > 0)
    //   for (uint32_t i = 0; i < 3 + NB_RX_CHANNELS_MAX; i++)
    //     Serial0.printf("SPI_rx[%d]=0x%02X|", i, SPI_frame_rx[i]);
    // Serial0.println();
    return 0; // SUCCESS
  }
  else
  {
    return -1; // FAIL
  }
}

void receive_A429_word(unsigned char channel, TRX_struct *RX_struct, uint32_t word_count)
{
  memset(RX_struct, 0, sizeof(TRX_struct));
  if (word_count != 0)
  {
    get_timestamp(&RX_struct->timestamp);
    RX_struct->channel_number = channel;
    unsigned char readed_values[NB_BYTES_PER_A429_WORD];
    read_FIFO_channel_RX(channel, readed_values);
    frame_char_to_uint32_t(readed_values, &RX_struct->words);
  }
}

void receive_multi_RX_channels(uint32_t nb_RX_channels, TRX_struct RX_struct[], uint8_t fifo_count[])
{
  read_ALL_FIFO_count_RX(0, fifo_count, nb_RX_channels);
  for (int i = 0; i < nb_RX_channels; i++)
  {
    if (fifo_count[i] > 0)
    {
      receive_A429_word(i, &RX_struct[i], fifo_count[i]);
    }
  }
}