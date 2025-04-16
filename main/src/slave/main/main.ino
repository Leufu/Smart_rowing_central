// includes 
#include "utils.h"
//#include "BLE_config.h"

//#include <arduino>
//#include 
//#include <esp_sntp.h>
#include <esp_timer.h>
//#include <rtc.h>
// firts try. the idea is to let the clock works but after 10 secs. restart it. so we know we could reset it properly
// esta funcion parece prometedora esp_time_impl_set_boot_time(now - since_boot)
// la funcion anterior no fue la mejor opcion debido a que es una funcion privada de la api
// por lo que mejor usaré un tiempo que se definirá como "0" cuando se reciva una señal. Esto signifiva que el sistema tendrá su punto de boot inicial intacto y otro tiempo tomado como cero parcial.
// la implementación de el tiempo parcial funcionó!
//
// Ahora la idea es implmentarla en el envio de datos BLUEthoot y que este se active cuando se envie una señal. 
//
//struct timeval tv_now;

static uint64_t time_cero= 0;
static uint64_t time_actual=0;
/*
int64_t get_time_us()
{
    gettimeofday(&tv_now, NULL);
    int64_t time_us = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;
    return time_us;
}
*/
void setup()
{
  time_cero= esp_timer_get_time();
  Serial.begin(115200);
  while(!Serial);
  Serial.print("Restart timer test");


}

void loop()
{
  uint16_t counter= 0;
  time_actual=esp_timer_get_time();
  Serial.printf("Time uS= %llu \n ", time_actual-time_cero);
  //delay(100);

  if (time_actual - time_cero >= 10000000)
  {
    time_cero=esp_timer_get_time(); 
    Serial.println("Clock reset");
  }
  //else
  //{
  //  counter=counter+1;
  //}
}
