#pragma once

#include <esp_timer.h>

static uint64_t time_cero= 0;
static uint64_t time_actual=0;


int64_t get_time_us()
{
	int64_t time_us = esp_timer_get_time();
	return time_us;
}
