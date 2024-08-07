#include "core_task_unified.hpp"
#include <A429_timestamp.hpp>
#include <A429_RX.hpp>
#include <A429_TX.hpp>

typedef uint32_t ring_pos_t;

volatile ring_pos_t rx_ring_head[NB_RX_CHANNELS_MAX];
volatile ring_pos_t tx_ring_head[NB_TX_CHANNELS_MAX];

volatile ring_pos_t rx_ring_tail[NB_RX_CHANNELS_MAX];
volatile ring_pos_t tx_ring_tail[NB_TX_CHANNELS_MAX];

volatile TRX_struct rx_ring_data[NB_RX_CHANNELS_MAX][RX_RING_SIZE];
volatile TRX_struct tx_ring_data[NB_TX_CHANNELS_MAX][TX_RING_SIZE];

extern uint32_t offset_loopback;

union packed_uint32_t
{
    uint32_t ui;
    uint8_t b[4];
};

int add_tx_ring(TRX_struct _to_add, unsigned int index_channel_TX)
{
    ring_pos_t next_head = (tx_ring_head[index_channel_TX] + 1) % TX_RING_SIZE;
    if (next_head != tx_ring_tail[index_channel_TX])
    {
        memcpy((void *)&tx_ring_data[index_channel_TX][tx_ring_head[index_channel_TX]], &_to_add, sizeof(TRX_struct));
        tx_ring_head[index_channel_TX] = next_head;
        return 0;
    }
    else
        return -1;
}

int remove_tx_ring(TRX_struct *_to_remove, unsigned int index_channel_TX)
{
    if (tx_ring_head[index_channel_TX] != tx_ring_tail[index_channel_TX])
    {
        memcpy(_to_remove, (void *)&tx_ring_data[index_channel_TX][tx_ring_tail[index_channel_TX]], sizeof(TRX_struct));
        tx_ring_tail[index_channel_TX] = (tx_ring_tail[index_channel_TX] + 1) % TX_RING_SIZE;
        return 0;
    }
    else
    {
        return -1;
    }
}

int peek_tx_ring(TRX_struct *_to_peek, unsigned int index_channel_TX)
{
    if (tx_ring_head[index_channel_TX] == tx_ring_tail[index_channel_TX])
    {
        return -1;
    }
    memcpy(_to_peek, (void *)&tx_ring_data[index_channel_TX][tx_ring_tail[index_channel_TX]], sizeof(TRX_struct));
    return 0;
}

int add_rx_ring(TRX_struct _to_add, unsigned int index_channel_RX)
{
    ring_pos_t next_head = (rx_ring_head[index_channel_RX] + 1) % RX_RING_SIZE;
    if (next_head != rx_ring_tail[index_channel_RX])
    {
        memcpy((void *)&rx_ring_data[index_channel_RX][rx_ring_head[index_channel_RX]], &_to_add, sizeof(TRX_struct));
        rx_ring_head[index_channel_RX] = next_head;
        return 0;
    }
    else
        return -1;
}

int remove_rx_ring(TRX_struct *_to_remove, unsigned int index_channel_RX)
{
    if (rx_ring_head[index_channel_RX] != rx_ring_tail[index_channel_RX])
    {
        memcpy(_to_remove, (void *)&rx_ring_data[index_channel_RX][rx_ring_tail[index_channel_RX]], sizeof(TRX_struct));
        rx_ring_tail[index_channel_RX] = (rx_ring_tail[index_channel_RX] + 1) % RX_RING_SIZE;
        return 0;
    }
    else
        return -1;
}

TRX_struct peek_struct_A429;

extern uint32_t offset_send_delay;
extern uint32_t offset_init;

extern TRX_struct Serial_TX_global;
extern TRX_struct Serial_RX_global;

extern TRX_struct channel_TX[NB_TX_CHANNELS_MAX];
extern TRX_struct channel_RX[NB_RX_CHANNELS_MAX];

extern unsigned int nb_RX_channels;
extern unsigned int nb_TX_channels;

TRX_struct peek_struct_Serial;

extern uint32_t offset_init;

int compteur_mot_queue = 0;
unsigned int nb_mots_received = 0;

int usb_error = 0;
int test = 0;
int remove_tx_count = 0;
unsigned long t_send_TX_channel = 0;
unsigned long t_receive_channel = 0;
unsigned long t0_test = 0;
unsigned long t0_perfo = 0;
unsigned long t_perfo = 0, t_perfo_max = 0;
void unified_task(void *pvParameters)
{
    Serial0.println("start A429 task");
    unsigned long t0 = millis();
    for (;;)
    {
        t0_perfo = micros();
        t0_test = t0_perfo;

        for (int i = 0; i < nb_TX_channels; i++)
        {
            if (peek_tx_ring(&peek_struct_A429, i) == 0)
            {
                uint32_t temp_timestamp;
                get_timestamp(&temp_timestamp);
                if ((temp_timestamp) >= (offset_init - offset_send_delay + peek_struct_A429.timestamp))
                {
                    // Serial0.printf("sent t0:%d at %d|| channel head=%d | channel tail=%d\r\n", peek_struct_A429.timestamp, temp_timestamp - offset_init,tx_ring_head[0], tx_ring_tail[0]);
                    remove_tx_ring(&channel_TX[i], i);
                    send_TX_channel(i, &channel_TX[i]);
                    t_send_TX_channel = 0.4 * (micros() - t0_test) + 0.6 * t_send_TX_channel;
                    remove_tx_count++;
                }
            }
        }

        // Réception
        t0_test = micros();
        uint8_t fifo_count[NB_RX_CHANNELS_MAX] = {0};
        receive_multi_RX_channels(nb_RX_channels, channel_RX, fifo_count);
        for (int i = 0; i < nb_RX_channels; i++)
        {
            if (fifo_count[i] > 0)
            {
                nb_mots_received++;
                static uint32_t last_timestamp = 0;
                channel_RX[i].timestamp = channel_RX[i].timestamp - offset_init - offset_loopback; // compensate for RX chain latency
                last_timestamp = channel_RX[i].timestamp;
                // Serial0.printf("A429 RX n°%d=0x%X ,t=%d\n", nb_mots_received, channel_RX[i].words, channel_RX[i].timestamp); // 04-07-2024
                if (add_rx_ring(channel_RX[i], i) == -1)
                    Serial0.println("xQueueSend channel_RX overflow");
                t_receive_channel = 0.4 * (micros() - t0_test) + 0.6 * t_receive_channel;
            }
        }

        //==========================
        // Partie serie
        //==========================
        if (Serial.available() >= sizeof(TRX_struct))
        {
            test++;
            Serial.readBytes((char *)&Serial_TX_global, sizeof(TRX_struct));
            // Serial0.printf("Serial TX n°%d: 0x%X, t0: %d, ch: %d\n", test, Serial_TX_global.words, Serial_TX_global.timestamp, Serial_TX_global.channel_number); // 04-07-2024

            if (Serial_TX_global.channel_number < 8)
            {
                if (add_tx_ring(Serial_TX_global, Serial_TX_global.channel_number) == -1)
                {
                    Serial0.println("add_tx_ring Serial_TX_global overflow");
                    Serial0.printf("test=%d t_send_TX_channel=%d|t_receive_channel=%d|t_perfo=%d|t_perfo_max=%d\r\n", test, t_send_TX_channel, t_receive_channel, t_perfo, t_perfo_max);
                    Serial0.printf("channel head=%d | channel tail=%d | remove_tx_count=%d| micros()-offset=%d\r\n", tx_ring_head[0], tx_ring_tail[0], remove_tx_count, micros() - offset_init);
                    while (1)
                        delay(10);
                }
            }
            // TODO interpretation config si channel == 99
            else if (Serial_TX_global.channel_number == 99)
            {
                // TODO interpreter la config
                Serial0.println("Config Packet");
                offset_init = micros();
            }
        }

        // Réception
        static uint32_t serial_rx_counter = 0;
        for (int i = 0; i < nb_RX_channels; i++)
        {
            if (remove_rx_ring(&Serial_RX_global, i) == 0)
            {
                if (Serial.availableForWrite() < sizeof(TRX_struct))
                {
                    if (usb_error < 10)
                    {
                        Serial0.println("no space for availableForWrite");
                    }
                    else if (usb_error == 10)
                    {
                        Serial0.println("no space for availableForWrite ===========> TX LAST ERROR PRINTED");
                        while (1)
                            ;
                    }
                    usb_error++;
                }
                else
                {
                    Serial.write((uint8_t *)&Serial_RX_global, sizeof(TRX_struct));
                    serial_rx_counter++;
                }
            }
        }

        t_perfo = 0.5 * (micros() - t0_perfo) + 0.5 * t_perfo;
        if (t_perfo > t_perfo_max)
        {
            t_perfo_max = t_perfo;
        }
        static int flag_perfo = 0;
        if (millis() - t0 > 4000)
        {
            t0 = millis();
            vTaskDelay(1);
            flag_perfo++;
            if (flag_perfo == 2)
            {
                flag_perfo = 0;
                Serial0.printf("test=%d serial_rx=%d|t_send_TX_channel=%d|t_receive_channel=%d|t_perfo=%d|t_perfo_max=%d\r\n", test, serial_rx_counter, t_send_TX_channel, t_receive_channel, t_perfo, t_perfo_max);
            }
        }
    }
}