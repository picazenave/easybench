#ifndef A429_RX_HPP

#define A429_RX_HPP

#include <ll_HI3220_SPI_com.hpp>
#include <ll_HI3220_init.hpp>

// Rate mode
#define HS 1000000
#define LS 2000000

#define INIT_MCR_TRX 0xC0    // MASTER CONTROL REGISTER VALUE (without bit order flip)
#define INIT_MCR_RX 0x80     // MASTER CONTROL REGISTER VALUE (without bit order flip)
#define INIT_MCR_TX 0x40     // MASTER CONTROL REGISTER VALUE (without bit order flip)
#define INIT_RX_HS_CTR_REG 0x80
#define INIT_TX_HS_CTR_REG 0x80
#define INIT_RX_LS_CTR_REG 0x82
#define INIT_TX_LS_CTR_REG 0x82
#define NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL 32

// TRX mode
#define TRX 100
#define RX 101
#define TX 102

void config_HI3220(double clock_frequency, unsigned char data_order, unsigned char data_mode);

int config_MCR_register(unsigned char MCR_value);

int config_TX_CTR_register(unsigned char channel, unsigned char TX_CTR_value);

int config_RX_CTR_register(unsigned char channel, unsigned char RX_CTR_value);

int config_look_up_table_RX_channel(unsigned char channel);

int config_TRX(int TRX_mode, unsigned int nb_RX_channels, unsigned int nb_TX_channels, TRX_struct RX_channels[], TRX_struct TX_channels[], unsigned int TX_rate_mode, unsigned int RX_rate_mode);

#endif