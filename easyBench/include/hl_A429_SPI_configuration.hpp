#ifndef LL_A429_SPI_CONFIGURATION_HPP

#define LL_A429_SPI_CONFIGURATION_HPP

#include <ll_A429_SPI_init.hpp>
#include <ll_A429_SPI_read_register.hpp>
#include <ll_A429_SPI_write_register.hpp>

#define INIT_MCR_TRX 0xC0    // MASTER CONTROL REGISTER VALUE (without bit order flip)
#define INIT_MCR_RX 0x80     // MASTER CONTROL REGISTER VALUE (without bit order flip)
#define INIT_MCR_TX 0x40     // MASTER CONTROL REGISTER VALUE (without bit order flip)
#define INIT_RX_CTR_REG 0x80
#define INIT_TX_CTR_REG 0x80
#define NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL 32

// TRX mode
#define TRX 100
#define RX 101
#define TX 102

int config_MCR_register(unsigned char MCR_value);

int config_TX_CTR_register(unsigned char channel, unsigned char TX_CTR_value);

int config_RX_CTR_register(unsigned char channel, unsigned char RX_CTR_value);

int config_look_up_table_RX_channel(unsigned char channel);

int config_SPI2(double clock_frequency, unsigned char data_order, unsigned char data_mode);

int config_TRX(int TRX_mode, unsigned int nb_RX_channels, unsigned int nb_TX_channels);

int config_TRX_structs(unsigned int nb_channel, TRX_struct *TRX_structs);

int config_TRX_structs2(unsigned int nb_channel, TRX_struct TRX_structs[]);

int config_TRX_structs3(unsigned int nb_channel, TRX_struct_test TRX_structs[]);

int config_TRX2(int TRX_mode, unsigned int nb_RX_channels, unsigned int nb_TX_channels, TRX_struct RX_channels[], TRX_struct TX_channels[]);

int config_TRX3(int TRX_mode, unsigned int nb_RX_channels, unsigned int nb_TX_channels, TRX_struct_test RX_channels[], TRX_struct_test TX_channels[]);

// UTILISE DANS LES FCT DE CONTROLE
int read_RX_CTR_REG(unsigned char channel, unsigned char readed_CTR_reg[]);

int write_RX_CTR_REG(unsigned char channel, unsigned char data_values[]);

#endif