#include <A429_config.hpp>
#include <ll_HI3220_SPI_com.hpp>
#include <core_task_unified.hpp>
#include <A429_timestamp.hpp>
#include <A429_RX.hpp>
#include <A429_TX.hpp>

void receive_A429_word(unsigned char channel, TRX_struct *RX_struct, uint32_t word_count);
int read_FIFO_count_RX(unsigned char channel, uint32_t *value);

#define STACK_SIZE 8192
StaticTask_t xTaskBuffer;
StackType_t xStack[STACK_SIZE]; // 8bit type

TRX_struct Serial_TX_global;
TRX_struct Serial_RX_global;

TRX_struct channel_TX[NB_TX_CHANNELS_MAX];
TRX_struct channel_RX[NB_RX_CHANNELS_MAX];

uint32_t offset_loopback; // time it takes to go from rx to tx on loopback

extern uint32_t offset_init;

unsigned int nb_RX_channels = 6;
unsigned int nb_TX_channels = 6;

void calibration_loopback_offset()
{
  uint32_t offset_calib = 0;
  TRX_struct dummy_tx;
  TRX_struct dummy_rx;

  uint32_t offset_loopback_max = 0;
  uint32_t offset_loopback_min = 9999;

  dummy_tx.channel_number = 0;
  dummy_tx.words = 0xAA55BABE;
  for (int i = 0; i < 200; i++)
  {
    uint32_t temp_timestamp;
    get_timestamp(&temp_timestamp);
    send_TX_channel(0, &dummy_tx);
    offset_calib = micros();
    while (1)
    {
      uint32_t value = 0;
      read_FIFO_count_RX(0, &value);
      if (value > 0)
      {
        receive_A429_word(0, &dummy_rx, value);
        if (dummy_rx.timestamp != 0)
        {
          dummy_rx.timestamp = dummy_rx.timestamp - offset_calib;
          offset_loopback = 0.5 * dummy_rx.timestamp + 0.5 * offset_loopback;
          if (offset_loopback > offset_loopback_max)
            offset_loopback_max = offset_loopback;
          else if (offset_loopback < offset_loopback_min)
            offset_loopback_min = offset_loopback;
          break;
        }
      }
    }
  }
  Serial0.printf("Loopback offset full chain TX->RX =%dus (max=%dus/min=%dus) --> corrected for A429 word(320us)=%dus\n\r", offset_loopback, offset_loopback_max, offset_loopback_min, offset_loopback - 320);
  offset_loopback = offset_loopback - 320;
}

void setup()
{
  // -------------INIT CONFIG-------------
  Serial.setTxBufferSize(QUEUE_LEN * 4 * 3);
  Serial.setRxBufferSize(QUEUE_LEN * 4 * 3);

  Serial.begin(2200000);

  Serial0.begin(2200000);
  Serial0.setDebugOutput(1);
  Serial0.println("Serial0 started");
  config_HI3220(40e6, MSBFIRST, SPI_MODE0);
  config_TRX(TRX, nb_RX_channels, nb_TX_channels, channel_RX, channel_TX, HS, HS);
  offset_init = micros();
  Serial0.printf("TRX struct size=%d\r\n", sizeof(TRX_struct));
  calibration_loopback_offset();
  //------------SUGGESTION--------------
  while (1)
  {
    if (Serial.available() >= sizeof(TRX_struct))
    {
      Serial.readBytes((char *)&Serial_TX_global, sizeof(TRX_struct));

      if (Serial_TX_global.channel_number == 99)
      {
        offset_init = micros();
        switch (Serial_TX_global.words)
        {
        case 0xFFFFFFFF: // Activer toutes les voies en HS
          offset_init = micros();
          nb_RX_channels = NB_RX_CHANNELS_MAX;
          nb_TX_channels = NB_TX_CHANNELS_MAX;
          config_TRX(TRX, nb_RX_channels, nb_TX_channels, channel_RX, channel_TX, HS, HS);
          break;
        case 0xAAAAAAAA: // Activer 1 voie RX et 1 voie TX en HS
          offset_init = micros();
          nb_RX_channels = 1;
          nb_TX_channels = 1;
          config_TRX(TRX, nb_RX_channels, nb_TX_channels, channel_RX, channel_TX, HS, HS);
          break;
        case 0xBBBBBBBB: // Activer toutes les voies en LS
          offset_init = micros();
          nb_RX_channels = NB_RX_CHANNELS_MAX;
          nb_TX_channels = NB_TX_CHANNELS_MAX;
          config_TRX(TRX, nb_RX_channels, nb_TX_channels, channel_RX, channel_TX, LS, LS);
          break;
        case 0xCCCCCCCC: // Activer 1 voie RX et 1 voie TX en HS
          offset_init = micros();
          nb_RX_channels = 1;
          nb_TX_channels = 1;
          config_TRX(TRX, nb_RX_channels, nb_TX_channels, channel_RX, channel_TX, LS, LS);
          break;
        default: // Activer toutes les voies en HS
          offset_init = micros();
          nb_RX_channels = NB_RX_CHANNELS_MAX;
          nb_TX_channels = NB_TX_CHANNELS_MAX;
          config_TRX(TRX, nb_RX_channels, nb_TX_channels, channel_RX, channel_TX, HS, HS);
          Serial0.println("config done");
          break;
        }
      }
      break;
    }
  }
  Serial0.println("Done config");
  //------------SUGGESTION--------------

  // test.channel_number = 0;  //04-07-2024
  // test.timestamp = offset + 1000000;  //04-07-2024
  // test.words = 0xCAFEBABE;  //04-07-2024
  // xQueueSend(queue_TX[Serial_TX_global.channel_number], &test, 0);  //04-07-2024

  // xTaskCreatePinnedToCore(a429_communication_task,
  //                         "a429_communication",
  //                         8192,
  //                         NULL,
  //                         5,
  //                         NULL,
  //                         tskNO_AFFINITY);
  // xTaskCreatePinnedToCore(Serial_communication_task,
  //                         "Serial_task",
  //                         8192,
  //                         NULL,
  //                         4,
  //                         NULL,
  //                         tskNO_AFFINITY);
  xTaskCreateStatic(unified_task,
                    "unified_task",
                    STACK_SIZE,
                    NULL,
                    25,
                    xStack,
                    &xTaskBuffer);

  vTaskDelete(NULL);
}

void loop()
{
}