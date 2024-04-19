#ifndef LL_A429_SPI_TRANSMIT_HPP

#define LL_A429_SPI_TRANSMIT_HPP

#include <ll_A429_SPI_read_register.hpp>
#include <ll_A429_SPI_write_register.hpp>
#include <ll_A429_SPI_received_frame_timestamp.hpp>

int frame_uint32_t_to_char(unsigned char char_values[], uint32_t uint32_values[], unsigned int nb_words);

int send_TX_channel(unsigned char channel, TRX_struct *TX_struct);

int send_multi_TX_channels(unsigned char nb_channel, TRX_struct TX_struct[]);

int frame_uint32_t_to_char2(unsigned char char_values[], uint32_t uint32_values, unsigned int nb_words);

int send_TX_channel2(unsigned char channel, TRX_struct_test *TX_struct);

int send_multi_TX_channels2(unsigned char nb_channel, TRX_struct_test TX_struct[]);

#endif