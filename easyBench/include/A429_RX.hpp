#ifndef A429_RX_HPP

#define A429_RX_HPP

#include <ll_HI3220_SPI_com.hpp>

void frame_char_to_uint32_t(unsigned char char_values[], uint32_t *uint32_value);

void read_FIFO_channel_RX(unsigned char channel, unsigned char readed_values[]);

int read_FIFO_count_RX(unsigned char channel, uint32_t *value);

void receive_A429_word(unsigned char channel, TRX_struct *RX_struct, uint32_t word_count);

void receive_multi_RX_channels(uint32_t nb_RX_channels, TRX_struct RX_struct[],uint8_t fifo_count[]);

// void read_channels_RX(unsigned char nb_channel, TRX_struct RX_struct[]);

#endif