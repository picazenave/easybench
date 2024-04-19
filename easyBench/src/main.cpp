#include "hl_A429_SPI_configuration.hpp"
#include "hl_A429_SPI_receive.hpp"
#include "hl_A429_SPI_transmit.hpp"
#include "hl_A429_SPI_debug.hpp"

#include "hl_communication_core_task.hpp"
#include "hl_A429_core_task.hpp"

// put function declarations here:
// // Niveau au-dessus (A faire après les vacances)
// // Optimiser le code (uniformiser les types de variables)

TRX_struct channel_RX[NB_RX_CHANNELS];
TRX_struct channel_TX[NB_TX_CHANNELS];

TRX_struct_test Serial_TX_global;
TRX_struct Serial_RX_global;
TRX_struct channel_TX_global;
TRX_struct channel_RX_global;

QueueHandle_t queue_RX[NB_RX_CHANNELS];
QueueHandle_t queue_TX[NB_TX_CHANNELS];
QueueHandle_t queue_TX_global;
QueueHandle_t queue_RX_global;

TRX_struct_test channel_RX_test[NB_RX_CHANNELS];
TRX_struct_test channel_TX_test[NB_TX_CHANNELS];
TRX_struct_test test;
TRX_struct_test peek_struct_main;

xSemaphoreHandle mutex_RX=NULL,mutex_TX=NULL;

int offset = 0;
void setup()
{
  // put your setup code here, to run once:
  // -------------INIT CONFIG-------------
  Serial.begin(460800);
  Serial.setRxBufferSize(4096);
  config_SPI2(40e6, MSBFIRST, SPI_MODE0);
  // config_TRX2(TRX, NB_RX_CHANNELS, NB_TX_CHANNELS, channel_RX, channel_TX);
  config_TRX3(TRX, NB_RX_CHANNELS, NB_TX_CHANNELS, channel_RX_test, channel_TX_test);
  queue_TX_global = xQueueCreate(64 * 8, sizeof(TRX_struct));
  queue_RX_global = xQueueCreate(64 * 16, sizeof(TRX_struct));
  for (int i = 0; i < NB_RX_CHANNELS; i++)
  {
    queue_RX[i] = xQueueCreate(64, sizeof(TRX_struct_test));
  }
  for (int i = 0; i < NB_TX_CHANNELS; i++)
  {
    queue_TX[i] = xQueueCreate(64, sizeof(TRX_struct_test));
  }
  Serial.setTimeout(1);
  offset = micros();

  mutex_RX=xSemaphoreCreateMutex();
  mutex_TX=xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(a429_communication_task,
                          "a429_communication",
                          8192,
                          NULL,
                          1,
                          NULL,
                          0);
  xTaskCreatePinnedToCore(serial_communication_task,
                          "serial_task",
                          8192,
                          NULL,
                          1,
                          NULL,
                          1);
}
unsigned int i = 0;

void loop()
{
  // put your main code here, to run repeatedly:

  // TASK COMMUNICATION
  // // Emission
  // if (Serial.available() >= sizeof(TRX_struct_test))
  // {
  //   Serial.readBytes((char *)&Serial_TX_global, sizeof(TRX_struct_test));
  //   if (Serial_TX_global.channel_number == 99)
  //   {
  //     offset = micros();
  //   }
  //   else if (Serial_TX_global.channel_number < 8)
  //   {
  //     xQueueSend(queue_TX[Serial_TX_global.channel_number], &Serial_TX_global, 0);
  //   }
  // }

  // // Réception
  // for (int i = 0; i < 1; i++)
  // {
  //   if (channel_RX_test[i].timestamp != 0)
  //   {
  //     Serial.write((uint8_t *)&channel_RX_test[i], sizeof(channel_RX_test[i]));
  //   }
  // }

  //  a429_communication(queue_TX, peek_struct, offset, channel_TX_test, channel_RX_test);

  // TASK A429
  // // Emission
  // for(int i = 0; i < 1; i++)
  // {
  //   if (xQueuePeek(queue_TX[i], &peek_struct_main, 0) == pdTRUE)
  //   {
  //     uint32_t temp_timestamp;
  //     get_timestamp(&temp_timestamp);
  //     if (peek_struct_main.timestamp <= (temp_timestamp - 10 - offset))
  //     {
  //       xQueueReceive(queue_TX[i], &channel_TX_test[i], 0);
  //       send_multi_TX_channels2(1, &channel_TX_test[i]);
  //     }
  //   }
  // }

  // // Réception
  // read_multi_RX_channels2(1, channel_RX_test);
  // if (channel_RX_test[0].timestamp != 0)
  // {
  //   // Serial.printf("le timestamp : %d\n", channel_RX_test[0].timestamp);
  //   // Serial.printf("le channel : %d\n", channel_RX_test[0].channel_number);
  //   // Serial.printf("le mot : %X\n", channel_RX_test[0].words);
  //   channel_RX_test[0].timestamp = channel_RX_test[0].timestamp - offset;
  //   // Serial.write((uint8_t *)&channel_RX_test[0], sizeof(channel_RX_test[0]));
  // }
  i++;
  delay(100);
}