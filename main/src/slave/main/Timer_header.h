#pragma once

#include <esp_timer.h>


int64_t get_time_us_partial()
{
	int64_t time_us = esp_timer_get_time();
	return time_us-time_cero;
}

int64_t get_time_us()
{
	int64_t time_us = esp_timer_get_time();
	return time_us;
}

