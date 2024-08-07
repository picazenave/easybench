#include "A429_config.hpp"

void config_HI3220(double clock_frequency, unsigned char data_order, unsigned char data_mode)
{
  init_SPI_pins();
  int retries = 0;
  while (retries<10)
  {
    if (HI3220_set_state(HI3220_IDLE_STATE) == 0)
    {
      break;
    }
  }
  Serial0.printf("HI ready after %d retries\n\r",retries);
  init_SPI_settings(clock_frequency, data_order, data_mode);
  HI3220_set_state(HI3220_ACTIVE_STATE);
}

int config_MCR_register(unsigned char MCR_value)
{
  unsigned char MCR_frame[] = {MCR_value};
  unsigned char readed_MCR_reg[SIZE_OPCODE_READ_FA];
  write_register_FA(ADD_MCR_W, MCR_frame);
  read_register_FA(ADD_MCR_R, readed_MCR_reg);
  if (MCR_value == readed_MCR_reg[0])
  {
    return 0; // SUCCESS
  }
  else
  {
    return -1; // FAIL
  }
}

int config_TX_CTR_register(unsigned char channel, unsigned char TX_CTR_value)
{
  if (channel <= 7 && channel >= 0)
  {
    int channel_address = 0x8030 + channel;
    int size_array = 1;
    unsigned char TX_CTR_frame[] = {TX_CTR_value};
    unsigned char readed_TX_CTR_reg[size_array];
    write_register_RDA(channel_address, TX_CTR_frame);
    read_register_RDA(channel_address, readed_TX_CTR_reg);
    if (TX_CTR_value == readed_TX_CTR_reg[0])
    {
      return 0; // SUCCESS
    }
    else
    {
      return -1; // FAIL
    }
  }
  else
  {
    return -1; // FAIL
  }
}

int config_RX_CTR_register(unsigned char channel, unsigned char RX_CTR_value)
{
  if (channel <= 15 && channel >= 0)
  {
    int channel_address = 0x8020 + channel;
    int size_array = 1; // TO MODIFY : plus besoin normalement
    unsigned char RX_CTR_frame[] = {RX_CTR_value};
    unsigned char readed_RX_CTR_reg[size_array];
    write_register_RDA(channel_address, RX_CTR_frame);
    read_register_RDA(channel_address, readed_RX_CTR_reg);
    if (RX_CTR_value == readed_RX_CTR_reg[0])
    {
      return 0; // SUCCESS
    }
    else
    {
      return -1; // FAIL
    }
  }
  else
  {
    return -1; // FAIL
  }
}

int config_look_up_table_RX_channel(unsigned char channel)
{
  if (channel <= 15 && channel >= 0)
  {
    int channel_address = 0x6800 + channel * NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL;
    int size_array = 1; // TO MODIFY : plus besoin normalement
    unsigned char look_up_frame[NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL];
    for (int i = 0; i < NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL; i++)
    {
      look_up_frame[i] = 0xFF;
    }

    unsigned char readed_look_up_reg[NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL];
    write_register_MAP(channel_address, look_up_frame, NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL);
    read_register_MAP(channel_address, readed_look_up_reg, NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL);
    int return_value = 0;
    for (int i = 0; i < NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL; i++)
    {
      if (look_up_frame[i] != readed_look_up_reg[i])
      {
        return_value++;
      }
    }
    if (return_value == 0)
    {
      return 0; // SUCCESS
    }
    else
    {
      return -1; // FAIL
    }
  }
  else
  {
    return -1; // FAIL
  }
}

int config_TRX(int TRX_mode, unsigned int nb_RX_channels, unsigned int nb_TX_channels, TRX_struct RX_channels[], TRX_struct TX_channels[], unsigned int TX_rate_mode, unsigned int RX_rate_mode)
{
  // NE PAS OUBLIER LES CONDITIONS DE NB DE VOIES
  int TX_channel_number_max;
  int RX_channel_number_max;
  if ((RX_rate_mode == HS || RX_rate_mode == LS) && (TX_rate_mode == HS || TX_rate_mode == LS))
  {
    switch (TRX_mode)
    {
    case TX:
      config_MCR_register(INIT_MCR_TX);
      memset(TX_channels, 0, nb_TX_channels * sizeof(TRX_struct));
      TX_channel_number_max = nb_TX_channels - 1;
      if (TX_rate_mode == HS)
      {
        for (int i = 0; i <= TX_channel_number_max; i++)
        {
          config_TX_CTR_register(i, INIT_TX_HS_CTR_REG);
        }
      }
      else if (TX_rate_mode == LS)
      {
        for (int i = 0; i <= TX_channel_number_max; i++)
        {
          config_TX_CTR_register(i, INIT_TX_LS_CTR_REG);
        }
      }
      return 0;
      break;
    case RX:
      config_MCR_register(INIT_MCR_RX);
      memset(RX_channels, 0, nb_RX_channels * sizeof(TRX_struct));
      RX_channel_number_max = nb_RX_channels - 1;
      if (RX_rate_mode == HS)
      {
        for (int i = 0; i <= RX_channel_number_max; i++)
        {
          config_RX_CTR_register(i, INIT_RX_LS_CTR_REG);
          config_look_up_table_RX_channel(i);
        }
      }
      else if (RX_rate_mode == LS)
      {
        for (int i = 0; i <= RX_channel_number_max; i++)
        {
          config_RX_CTR_register(i, INIT_RX_LS_CTR_REG);
          config_look_up_table_RX_channel(i);
        }
      }
      return 0;
      break;
    case TRX:
      config_MCR_register(INIT_MCR_TRX);
      memset(RX_channels, 0, nb_RX_channels * sizeof(TRX_struct));
      RX_channel_number_max = nb_RX_channels - 1;
      memset(TX_channels, 0, nb_TX_channels * sizeof(TRX_struct));
      TX_channel_number_max = nb_TX_channels - 1;
      if (RX_rate_mode == HS)
      {
        for (int i = 0; i <= RX_channel_number_max; i++)
        {
          config_RX_CTR_register(i, INIT_RX_HS_CTR_REG);
          config_look_up_table_RX_channel(i);
        }
      }
      else if (RX_rate_mode == LS)
      {
        for (int i = 0; i <= RX_channel_number_max; i++)
        {
          config_RX_CTR_register(i, INIT_RX_LS_CTR_REG);
          config_look_up_table_RX_channel(i);
        }
      }
      if (TX_rate_mode == HS)
      {
        for (int i = 0; i <= TX_channel_number_max; i++)
        {
          config_TX_CTR_register(i, INIT_TX_HS_CTR_REG);
        }
      }
      else if (TX_rate_mode == LS)
      {
        for (int i = 0; i <= TX_channel_number_max; i++)
        {
          config_TX_CTR_register(i, INIT_TX_LS_CTR_REG);
        }
      }
      return 0;
      break;
    }
  }
  else
  {
    return -1; // FAIL
  }

  return 0;
}