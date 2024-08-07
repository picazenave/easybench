#ifndef CORE_TASK_UNIFIED_HPP

#define CORE_TASK_UNIFIED_HPP

#include <ll_HI3220_SPI_com.hpp>

#define DEBUG_PORT Serial
#define DATA_PORT Serial0

void init_queue_mutex();

int add_tx_ring(TRX_struct _to_add, unsigned int index_channel_TX);

int remove_tx_ring(TRX_struct *_to_remove, unsigned int index_channel_TX);

int peek_tx_ring(TRX_struct *_to_peek, unsigned int index_channel_TX);

int add_rx_ring(TRX_struct _to_add, unsigned int index_channel_RX);

int remove_rx_ring(TRX_struct *_to_remove, unsigned int index_channel_RX);

void unified_task(void *pvParameters);

#endif