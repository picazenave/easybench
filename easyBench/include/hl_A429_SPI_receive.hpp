#ifndef LL_A429_SPI_RECEIVE_HPP

#define LL_A429_SPI_RECEIVE_HPP

#include <ll_A429_SPI_read_register.hpp>
#include <ll_A429_SPI_write_register.hpp>
#include <ll_A429_SPI_received_frame_timestamp.hpp>

int frame_char_to_uint32_t(unsigned char char_values[], uint32_t uint32_values[], unsigned int nb_words);

int read_FIFO_channel_RX(unsigned char channel, unsigned char readed_values[], int nb_bytes_to_read);

int read_FIFO_count_RX(unsigned char channel, uint32_t *value);

int read_channel_RX(unsigned char channel, TRX_struct *RX_struct);

int read_multi_RX_channels(unsigned char nb_channel, TRX_struct RX_struct[]);

int frame_char_to_uint32_t2(unsigned char char_values[], uint32_t *uint32_value, unsigned int nb_words);

int read_channel_RX2(unsigned char channel, TRX_struct_test *RX_struct);

int read_multi_RX_channels2(unsigned char nb_channel, TRX_struct_test RX_struct[]);

#endif