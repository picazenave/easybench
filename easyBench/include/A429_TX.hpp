#ifndef A429_TX_HPP

#define A429_TX_HPP

#include <ll_HI3220_SPI_com.hpp>

void frame_uint32_t_to_char(unsigned char char_values[], uint32_t uint32_values);

void send_TX_channel(unsigned char channel, TRX_struct *TX_struct);

void send_multi_TX_channels(unsigned char nb_channel, TRX_struct TX_struct[]);

#endif