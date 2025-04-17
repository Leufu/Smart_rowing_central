#pragma once
//#include <driver/temp_sensor.h>
#include <driver/temperature_sensor.h>

temperature_sensor_handle_t temp_sensor = NULL;
temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 80);
static float temperature_global=0;

/*
void temp_init(){
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac_offset = TSENS_DAC_L2;  // TSENS_DAC_L2 is default; L4(-40°C ~ 20°C), L2(-10°C ~ 80°C), L1(20°C ~ 100°C), L0(50°C ~ 125°C)
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
}

float read_temp()
{
	float tsens_value;
	temp_sensor_read_celsius(&tsens_value);
	return tsens_value;

}
*/

//al parecer varias de estas funciones no estan implementadas en el core de Arduino todavia así que testeare u codigo de internet que dice que funciona 

void temp_init()
{
    esp_err_t err;

    err = temperature_sensor_install(&temp_sensor_config, &temp_sensor);
    if (err == ESP_OK) {
        Serial.println("Temp sensor init");
    } else {
        Serial.printf("Temp sensor ERROR: %s\n", esp_err_to_name(err));
    }

    // Aquí sí puedes usar ESP_ERROR_CHECK si quieres que aborte si falla:
    ESP_ERROR_CHECK(temperature_sensor_enable(temp_sensor));
}

float read_temp()
{
    float tsens_value;
    ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_sensor, &tsens_value));
    return tsens_value;
}

