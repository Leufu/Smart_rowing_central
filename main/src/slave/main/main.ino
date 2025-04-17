// includes 
//#include <util.h>
//

static uint64_t time_cero;
static uint64_t time_actual;
static uint64_t time_partial;

#include <CodeCell.h>
#include <Timer_header.h>
#include "Temp_header.h"
#include"BLE_config.h"
#include <BNO085.h>
#include <imu_header.h>
#include <led.h>
//#include <capacitor.h>
#include <Rtos_Task.h>

// firts try. the idea is to let the clock works but after 10 secs. restart it. so we know we could reset it properly
// esta funcion parece prometedora esp_time_impl_set_boot_time(now - since_boot)
// la funcion anterior no fue la mejor opcion debido a que es una funcion privada de la api
// por lo que mejor usaré un tiempo que se definirá como "0" cuando se reciva una señal. Esto signifiva que el sistema tendrá su punto de boot inicial intacto y otro tiempo tomado como cero parcial.
// la implementación de el tiempo parcial funcionó!
//
// Ahora la idea es implmentarla en el envio de datos BLUEthoot y que este se active cuando se envie una señal. 
//
// es funcional. El programa actual crea un service y char que permite escribir en el. Si se escribe// reinicia el timer, lo que crea un pseudo tiempo. 
// 2 consideraciones. Es posible mejorar el proceso tomando la latencia entre los dispositivos y restarlo al valor actual
// El dispositivo no deja reconectarse despues de hacer una desconección. De seguro hay que agregar una función que permita re iniciar el paring una vez se desconecta el server

//struct timeval tv_now;


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
  time_cero= get_time_us(); 
  Serial.begin(115200);
  while(!Serial);
  led_init();
  IMU_Init(IMU_config,10,0xFFFFFFFF);
  while(err==1)
  {
    Serial.print("intento IMU");
    delay(100);
    err=0;
    IMU_Init(IMU_config,10,0xFFFFFFFF);
  } 
  Init_BLE();
  temp_init();
  init_freertos_tasks();

  Serial.println("Restart timer test");


}

void loop()
{
  //uint16_t counter= 0;
  //time_actual=esp_timer_get_time();
  //Serial.printf("Time uS= %llu \n ", time_actual-time_cero);
  //delay(100);

  /*
  if (time_actual - time_cero >= 10000000)
  {
    time_cero=esp_timer_get_time(); 
    Serial.println("Clock reset");
  }
  */
  //else
  //{
  //  counter=counter+1;
  //}
}
