#include "hl_A429_core_task.hpp"
extern QueueHandle_t queue_TX[NB_TX_CHANNELS];
extern QueueHandle_t queue_RX[NB_RX_CHANNELS];
TRX_struct_test peek_struct;
extern uint32_t offset;
extern TRX_struct_test channel_RX_test[NB_RX_CHANNELS];
extern TRX_struct_test channel_TX_test[NB_TX_CHANNELS];
extern xSemaphoreHandle mutex_RX, mutex_TX;

void a429_communication_task(void *pvParameters)
{
    if (mutex_RX == NULL)
    {
        for (;;)
        {
            delay(500);
            Serial.println("mutexRX NULL");
        }
    }
    if (mutex_TX == NULL)
    {
        for (;;)
        {
            delay(500);
            Serial.println("mutexTX NULL");
        }
    }
    esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(xPortGetCoreID()));
    unsigned long t0 = millis();
    for (;;)
    {
        for (int i = 0; i < 1; i++)
        {
            if (xQueuePeek(queue_TX[i], &peek_struct, 0) == pdTRUE)
            {
                uint32_t temp_timestamp;
                get_timestamp(&temp_timestamp);
                if (peek_struct.timestamp <= (temp_timestamp - 10 - offset))
                {
                    xQueueReceive(queue_TX[i], &channel_TX_test[i], 0);
                    send_multi_TX_channels2(1, &channel_TX_test[i]);
                }
            }
        }

        // Réception
        read_multi_RX_channels2(1, channel_RX_test);
        if (channel_RX_test[0].timestamp != 0)
        {

            channel_RX_test[0].timestamp = channel_RX_test[0].timestamp - offset;
            xQueueSend(queue_RX[0], &channel_RX_test[0], 0);
        }

        esp_task_wdt_reset();
        // if (millis() - t0 > 1000 && xQueuePeek(queue_TX[0], &peek_struct, 0) == pdFALSE)
        // {
        //     t0 = millis();
        //     vTaskDelay(10);
        // }
    }
}
