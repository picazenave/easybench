#include "hl_A429_SPI_debug.hpp"

void status_function_loop(int return_value, bool verbosity)                                    //[DONE]Function to put code in infinite loop if there is a mistake
{
  switch(return_value)
  {
    // VERBOSITY
    // INITIALIZATION
    case SPI2_PINS_INIT:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [init_SPI2_pins] SPI2_PINS_INIT (-1) : SPI2 pins are initialized (MRST, HCS, READY, RUN)\n");
      }
      break;
    case READY:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [init_SPI2_idle_state] READY (-2) : easyBench is in idle state, ready to receive SPI instructions... (the READY pin is asserted)\n");
      }
      break;
    case SPI2_SETTINGS_INIT:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [init_SPI2_settings] SPI2_SETTINGS_INIT (-3) : SPI2 settings are initialized\n");
      }
      break;
    case RUN:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [init_SPI2_active_state] RUN (-4) : easyBench is in active state...(SPI2 RUN is at HIGH state)\n");
      }
      break;

    // READING
    case FA_R:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [read_register2] FA_R (-101) : Values of readed registers acccessed via FAST ACCESS commands are stored in Readed_values[]\n");
      }
      break;
    case RDA_R:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [read_register2] RDA_R (-102) : Values of readed registers acccessed via RDA commands are stored in Readed_values[]\n");
      }
      break;
    case MAP_R:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [read_register2] MAP_R (-103) : Values of readed registers acccessed via MAP commands are stored in Readed_values[]\n");
      }
      break;
    case FIFO_R:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [read_register2] FIFO_R (-104) : Values of readed FIFO acccessed via FIFO commands are stored in Readed_values[]\n");
      }
      break;
    case READ_RX_CHANNEL:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [read_RX_CTR_REG] READ_RX_CHANNEL (-105) : The RX channel is between 0 and 15\n");
      }
      break;
    case FIFO_R2:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [read_FIFO_channel_RX] FIFO_R2 (-106) : Values of readed FIFO acccessed via FIFO commands are stored in Readed_values[]\n");
      }
      break;

      // WRITING
    case FA_W:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [write_register2] FA_W (-201) : Datas are sended via FAST ACCESS commands\n");
      }
      break;      
    case RDA_W:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [write_register2] RDA_W (-202) : Datas are sended via RDA commands\n");
      }
      break; 
    case MAP_W:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [write_register2] MAP_W (-203) : Datas are sended via MAP commands\n");
      }
      break; 
    case TRANSMIT_W:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [write_register2] TRANSMIT_W (-204) : Datas are sended via TRANSMIT commands\n");
      }
      break; 
    case WRITE_RX_CHANNEL:
      if(verbosity == true)
      {
        Serial.printf("[VERBOSITY] [write_RX_CTR_REG] WRITE_RX_CHANNEL (-205) : The RX channel is between 0 and 15\n");
      }
      break;

    // ERRORS
    // INITIALIZATION
    case NOT_READY:
      Serial.printf("[ERROR] [init_SPI2_idle_state] NOT_READY (-1001): the READY pin is not asserted (unavailable to receive SPI instructions)\n");
      while(1);

    // READING
    case NOT_READ_MODE:
      Serial.printf("[ERROR] [read_register2] NOT_READ_MODE (-1101): The read_mode value is not FA, RDA, MAP or FIFO\n");
      while(1);
    case NOT_FA_R:
      Serial.printf("[ERROR] [read_register2] NOT_FA_R (-1102): The reading address is not a fast access register address (Must be between 0x8000 and 0x801F)\n");
      while(1);
    case NOT_RDA_R:
      Serial.printf("[ERROR] [read_register2] NOT_RDA_R (-1103): The reading address is not a register direct addressing (Must be between 0x8000 and 0x8087)\n");
      while(1);
    case NOT_MAP_R:
      Serial.printf("[ERROR] [read_register2] NOT_MAP_R (-1104): The reading address is not a MAP address (Must be between 0xFFFF and 0x0000)\n");
      while(1);
    case NOT_FIFO_R:
      Serial.printf("[ERROR] [read_register2] NOT_FIFO_R (-1105): The reading address is not a FIFO address (Must be a multiple of 0x100 between 0x7000 and 0x7F00 : examples : 0x7000, 0x7100, 0x7200...)\n");
      while(1);
    case NOT_NB_BYTES_TO_READ:
      Serial.printf("[ERROR] [read_register2] NOT_NB_BYTES_TO_READ (-1106): The nb_bytes_to_read value must be a multiple of 4 except 0\n");
      while(1);
    case NOT_READ_RX_CHANNEL:
      Serial.printf("[ERROR] [read_RX_CTR_REG] NOT_READ_RX_CHANNEL (-1107): The RX channel is not between 0 and 15\n");
      while(1);
    case NOT_NB_BYTES_TO_READ2:
      Serial.printf("[ERROR] [read_FIFO_channel_RX] NOT_NB_BYTES_TO_READ2 (-1106): The nb_bytes_to_read value must be a multiple of 4 except 0\n");
      while(1);

    // WRITING
    case NOT_WRITE_MODE:
      Serial.printf("[ERROR] [write_register2] NOT_WRITE_MODE (-1201): The write_mode value is not FA, RDA or MAP\n");
      while(1);
    case NOT_NB_DATA_BYTE:
      Serial.printf("[ERROR] [write_register2] NOT_NB_DATA_BYTE (-1202): In fast access, nb_data_bytes must be 1 (because only MCR can be modified)\n");
      while(1);
    case NOT_FA_W:
      Serial.printf("[ERROR] [write_register2] NOT_FA_W (-1203): The writing address is not a fast access register address (Must be 0x8000)\n");
      while(1);
    case NOT_RDA_W:
      Serial.printf("[ERROR] [write_register2] NOT_RDA_W (-1204): The writing address is not a RDA address (Must be between 0x807F and 0x8000)\n");
      while(1);
    case NOT_MAP_W:
      Serial.printf("[ERROR] [write_register2] NOT_MAP_W (-1205): The writing address is not a MAP address (Must be between 0xFFFF and 0x0000 except some addresses)\n");
      while(1);
    case NOT_TRANSMIT_W:
      Serial.printf("[ERROR] [write_register2] NOT_TRANSMIT_W (-1206): The writing address (channel) is not a TRANSMIT channel (Must be between 0 and 7)\n");
      while(1);
    case NOT_NB_DATA_BYTES:
      Serial.printf("[ERROR] [write_register2] NOT_NB_DATA_BYTES (-1207): The nb_data_bytes value must be a multiple of 4 except 0\n");
      while(1);
    case NOT_WRITE_RX_CHANNEL:
      Serial.printf("[ERROR] [write_RX_CTR_REG] NOT_WRITE_RX_CHANNEL (-1208): The RX channel is not between 0 and 15\n");
      while(1);



    case NOT_FAST_ACCESS_W:
      Serial.printf("ERROR 5:[write_register] The writing address is not a fast access register address (Must be 0x8000)\n");
      while(1);
    case NOT_REGISTER_DIRECT_ADDRESSING_W:
      Serial.printf("ERROR 6:[write_register] The writing address is not a register direct addressing (Must be between 0x8000 and 0x807F)\n");
      while(1);
    case NOT_DATA_SIZE:  
      Serial.printf("ERROR 7:[write_register] The size of the data to send is longer than expected (Must be inferior to 0xFF)\n");
      while(1);
  } 
}

void print_SPI2_pinout()                             //[DONE]Function to print SPI2 pinout (MOSI, MISO, HCLK, HCS)
{
  Serial.printf("---SPI2 Pinout---\n");
  Serial.printf("MOSI : ");
  Serial.printf("%d\n", MOSI);
  Serial.printf("MISO : ");
  Serial.printf("%d\n", MISO);
  Serial.printf("SCK : ");
  Serial.printf("%d\n", SCK);
  Serial.printf("HCS : ");
  Serial.printf("%d\n", SS);
  Serial.printf("-----------------\n\n");
}