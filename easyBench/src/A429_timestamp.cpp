#include "A429_timestamp.hpp"

uint32_t offset_send_delay = 80;
uint32_t offset_init = 0;

void get_timestamp(uint32_t *time)
{
    *time = esp_timer_get_time();
}