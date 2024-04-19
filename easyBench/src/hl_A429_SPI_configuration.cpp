#include "hl_A429_SPI_configuration.hpp"

int config_MCR_register(unsigned char MCR_value)
{
  int size_array = 1;
  unsigned char MCR_frame[] = {MCR_value};
  unsigned char readed_MCR_reg[size_array];
  write_register2(ADD_MCR_W, MCR_frame, size_array, FAST_ACCESS);
  read_register2(ADD_MCR_R, readed_MCR_reg, size_array, FAST_ACCESS);
  if(MCR_value == readed_MCR_reg[0])
  {
    return CONFIG_MCR_REG;
  }
  else
  {
    return NOT_CONFIG_MCR_REG;
  }
}

int config_TX_CTR_register(unsigned char channel, unsigned char TX_CTR_value)
{
  if(channel <=7 && channel >= 0)
  {
    int channel_address = 0x8030 + channel;
    int size_array = 1;
    unsigned char TX_CTR_frame[] = {TX_CTR_value};
    unsigned char readed_TX_CTR_reg[size_array];
    write_register2(channel_address, TX_CTR_frame, size_array, REGISTER_DIRECT_ADDRESSING);
    read_register2(channel_address, readed_TX_CTR_reg, size_array, REGISTER_DIRECT_ADDRESSING);
    if(TX_CTR_value == readed_TX_CTR_reg[0])
    {
      return CONFIG_TX_CTR_REG;
    }
    else
    {
      return NOT_CONFIG_TX_CTR_REG;
    }
//    return WRITE_RX_CHANNEL; // Changer la valeur car autre sortie
  }
  else
  {
    return NOT_NB_TX_CHANNEL;
  }  
}

int config_RX_CTR_register(unsigned char channel, unsigned char RX_CTR_value)
{
  if(channel <=15 && channel >= 0)
  {
    int channel_address = 0x8020 + channel;
    int size_array = 1;
    unsigned char RX_CTR_frame[] = {RX_CTR_value};
    unsigned char readed_RX_CTR_reg[size_array];
    write_register2(channel_address, RX_CTR_frame, size_array, REGISTER_DIRECT_ADDRESSING);
    read_register2(channel_address, readed_RX_CTR_reg, size_array, REGISTER_DIRECT_ADDRESSING);
    if(RX_CTR_value == readed_RX_CTR_reg[0])
    {
      return CONFIG_RX_CTR_REG;
    }
    else
    {
      return NOT_CONFIG_RX_CTR_REG;
    }
  }
  else
  {
    return NOT_NB_RX_CHANNEL;
  }
}

int config_look_up_table_RX_channel(unsigned char channel)
{
  if(channel <=15 && channel >= 0)
  {
    int channel_address = 0x6800 + channel*NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL;
    int size_array = 1;
    unsigned char look_up_frame[NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL];
    for(int i=0;i<NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL;i++)
    {
      look_up_frame[i] = 0xFF;
    }
    
    unsigned char readed_look_up_reg[NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL];
    write_register2(channel_address, look_up_frame, NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL, MAP);
    read_register2(channel_address, readed_look_up_reg, NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL, MAP);
    int return_value = 0;
    for(int i=0;i<NB_ADD_LOOK_UP_TABLE_PER_CHANNNEL;i++)
    {
      if(look_up_frame[i] != readed_look_up_reg[i])
      {
        return_value++;
      }
    }    
    if(return_value == 0)
    {
      return CONFIG_LOOK_UP_REG;
    }
    else
    {
      return NOT_CONFIG_LOOK_UP_REG;
    }
  }
  else
  {
    return NOT_NB_RX_CHANNEL2;
  }  
}

int config_SPI2(double clock_frequency, unsigned char data_order, unsigned char data_mode)
{
  init_SPI2_pins();
  init_SPI2_idle_state();
  init_SPI2_settings(clock_frequency, data_order, data_mode);
  init_SPI2_active_state();
  return CONFIG_SPI2;
}

int config_TRX(int TRX_mode, unsigned int nb_RX_channels, unsigned int nb_TX_channels)
{
  if(TRX_mode == TRX || TRX_mode == RX || TRX_mode == TX)
  {
    if(TRX_mode == TRX|| TRX_mode == RX)
    {
      for(int i = 0; i <=15; i++)
      {
        config_RX_CTR_register(i, INIT_RX_CTR_REG);
        config_look_up_table_RX_channel(i);
      }
    }
    else if(TRX_mode == TRX|| TRX_mode == TX)
    {
      for(int i = 0; i <=7; i++)
      {
        config_TX_CTR_register(i, INIT_TX_CTR_REG);
      }
    }
    return CONFIG_TRX;
  }
  else
  {
    return NOT_TRX_MODE;
  }
}

// TRX_struct a[32]; potentiellement utile pour l'allocation statique
int config_TRX_structs(unsigned int nb_channel, TRX_struct *TRX_structs)
{
  TRX_structs = (TRX_struct*)malloc(nb_channel*sizeof(TRX_struct));
  if(TRX_structs == NULL)
  {
    return NOT_CONFIG_STRUCT;
  }

  memset(TRX_structs, 0, nb_channel*sizeof(TRX_struct));
  return 0;
}

int config_TRX_structs2(unsigned int nb_channel, TRX_struct TRX_structs[])
{
  for(int i=0; i<nb_channel; i++)
  {
    memset(TRX_structs, 0, nb_channel*sizeof(TRX_struct));
  }
  return 0;
}

int config_TRX_structs3(unsigned int nb_channel, TRX_struct_test TRX_structs[])
{
  for(int i=0; i<nb_channel; i++)
  {
    memset(TRX_structs, 0, nb_channel*sizeof(TRX_struct_test));
  }
  return 0;
}

int config_TRX2(int TRX_mode, unsigned int nb_RX_channels, unsigned int nb_TX_channels, TRX_struct RX_channels[], TRX_struct TX_channels[])
{
  // NE PAS OUBLIER LES CONDITIONS DE NB DE VOIES
  int TX_channel_number_max;
  int RX_channel_number_max;
  switch (TRX_mode)
  {
  case TX:
    config_MCR_register(INIT_MCR_TX);
    config_TRX_structs2(nb_TX_channels, TX_channels);
    TX_channel_number_max = nb_TX_channels -1;
    for(int i = 0; i <=TX_channel_number_max; i++)
    {
      config_TX_CTR_register(i, INIT_TX_CTR_REG);
    }
    return 0;
    break;
  case RX:
    config_MCR_register(INIT_MCR_RX);
    config_TRX_structs2(nb_RX_channels, RX_channels);
    RX_channel_number_max = nb_RX_channels -1;
    for(int i = 0; i <=RX_channel_number_max; i++)
    {
      config_RX_CTR_register(i, INIT_RX_CTR_REG);
      config_look_up_table_RX_channel(i);
    }
    return 0;
    break;
  case TRX:
    config_MCR_register(INIT_MCR_TRX);
    config_TRX_structs2(nb_RX_channels, RX_channels);
    RX_channel_number_max = nb_RX_channels -1;
    config_TRX_structs2(nb_TX_channels, TX_channels);
    TX_channel_number_max = nb_TX_channels -1;
    for(int i = 0; i <=RX_channel_number_max; i++)
    {
      config_RX_CTR_register(i, INIT_RX_CTR_REG);
      config_look_up_table_RX_channel(i);
    }
    for(int i = 0; i <=TX_channel_number_max; i++)
    {
      config_TX_CTR_register(i, INIT_TX_CTR_REG);
    }
    return 0;
    break;
  }
  return 0;
}

int config_TRX3(int TRX_mode, unsigned int nb_RX_channels, unsigned int nb_TX_channels, TRX_struct_test RX_channels[], TRX_struct_test TX_channels[])
{
  // NE PAS OUBLIER LES CONDITIONS DE NB DE VOIES
  int TX_channel_number_max;
  int RX_channel_number_max;
  switch (TRX_mode)
  {
  case TX:
    config_MCR_register(INIT_MCR_TX);
    config_TRX_structs3(nb_TX_channels, TX_channels);
    TX_channel_number_max = nb_TX_channels -1;
    for(int i = 0; i <=TX_channel_number_max; i++)
    {
      config_TX_CTR_register(i, INIT_TX_CTR_REG);
    }
    return 0;
    break;
  case RX:
    config_MCR_register(INIT_MCR_RX);
    config_TRX_structs3(nb_RX_channels, RX_channels);
    RX_channel_number_max = nb_RX_channels -1;
    for(int i = 0; i <=RX_channel_number_max; i++)
    {
      config_RX_CTR_register(i, INIT_RX_CTR_REG);
      config_look_up_table_RX_channel(i);
    }
    return 0;
    break;
  case TRX:
    config_MCR_register(INIT_MCR_TRX);
    config_TRX_structs3(nb_RX_channels, RX_channels);
    RX_channel_number_max = nb_RX_channels -1;
    config_TRX_structs3(nb_TX_channels, TX_channels);
    TX_channel_number_max = nb_TX_channels -1;
    for(int i = 0; i <=RX_channel_number_max; i++)
    {
      config_RX_CTR_register(i, INIT_RX_CTR_REG);
      config_look_up_table_RX_channel(i);
    }
    for(int i = 0; i <=TX_channel_number_max; i++)
    {
      config_TX_CTR_register(i, INIT_TX_CTR_REG);
    }
    return 0;
    break;
  }
  return 0;
}

int read_RX_CTR_REG(unsigned char channel, unsigned char readed_CTR_reg[])
{
  if(channel <=15 && channel >= 0)
  {
    int channel_address = 0x8020 + channel;
    read_register2(channel_address, readed_CTR_reg, 1, REGISTER_DIRECT_ADDRESSING);
    return READ_RX_CHANNEL;
  }
  else
  {
    return NOT_READ_RX_CHANNEL;
  }
}

int write_RX_CTR_REG(unsigned char channel, unsigned char data_values[])
{
  if(channel <=15 && channel >= 0)
  {
    int channel_address = 0x8020 + channel;
    write_register2(channel_address, data_values, 1, REGISTER_DIRECT_ADDRESSING);
    return WRITE_RX_CHANNEL;
  }
  else
  {
    return NOT_WRITE_RX_CHANNEL;
  }
}