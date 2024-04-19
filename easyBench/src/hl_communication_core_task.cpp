#include "hl_communication_core_task.hpp"

extern QueueHandle_t queue_TX[NB_TX_CHANNELS];
extern QueueHandle_t queue_RX[NB_RX_CHANNELS];
extern uint32_t offset;
TRX_struct_test peek_struct_serial;
extern TRX_struct_test channel_RX_test[NB_RX_CHANNELS];
extern TRX_struct_test Serial_TX_global;
extern xSemaphoreHandle mutex_RX, mutex_TX;

void serial_communication_task(void *pvParameters)
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
        if (Serial.available() >= sizeof(TRX_struct_test))
        {
            Serial.readBytes((char *)&Serial_TX_global, sizeof(TRX_struct_test));
            if (Serial_TX_global.channel_number == 99)
            {
                offset = micros();
            }
            else if (Serial_TX_global.channel_number < 8)
            {
                xQueueSend(queue_TX[Serial_TX_global.channel_number], &Serial_TX_global, 0);
            }
        }

        // Réception
        for (int i = 0; i < 1; i++)
        {
            if (xQueueReceive(queue_RX[i], &peek_struct_serial, 0) == pdTRUE)
            {
                Serial.write((uint8_t *)&peek_struct_serial, sizeof(TRX_struct_test));
            }
        }
        esp_task_wdt_reset();
        // if (millis() - t0 > 1000)
        // {
        //     t0 = millis();
        //     vTaskDelay(10);
        // }
    }
}