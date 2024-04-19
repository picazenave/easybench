#ifndef HL_COMMUNICATION_CORE_TASK_HPP

#define HL_COMMUNICATION_CORE_TASK_HPP

#include "esp_task_wdt.h"

#include "hl_A429_SPI_configuration.hpp"
#include "hl_A429_SPI_receive.hpp"
#include "hl_A429_SPI_transmit.hpp"
#include "hl_A429_SPI_debug.hpp"

void serial_communication_task(void *pvParameters);

#endif