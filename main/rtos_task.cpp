#include "rtos_task.h"


TaskHandle_t HandleGpsCom=NULL;
QueueHandle_t gps_data_fifo;


void init_freertos()
{

	 xTaskCreate(TaskGpsCom,"TaskGpsCom",2048,NULL,5,HandleGpsCom);
}


void TaskGpsCom(void *pvParameters)
{


}


