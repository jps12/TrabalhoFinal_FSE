#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_sleep.h"

#include "wifi.h"
#include "flash.h"
#include "http_client.h"
#include "mqtt.h"
#include "cJSON.h"
#include "gpio.h"

#define MAC_ADDRESS "00:00:00:00:00"

// - framework-espidf 3.40302.0 (4.3.2) 

xSemaphoreHandle conexaoWifiSemaphore;

void config_low_power_mode()
{
    esp_sleep_enable_gpio_wakeup();
    esp_sleep_enable_timer_wakeup(10 * 1000000); // Tempo dado em microsegundos
}


void IniciaMQTTRequest(void *params){
    while (true){
        if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)){
            mqtt_send_mac(MAC_ADDRESS);
        }
    }
}

void config_app(){
    Flash_init();

    conexaoWifiSemaphore = xSemaphoreCreateBinary();

    wifi_config();

    xTaskCreate(&IniciaMQTTRequest, "Envia MQTT mensagem de inicio", 4096, NULL, 1, NULL);
}

void app_main() 
{
    
    config_app();
    config_gpio();

#if CONFIG_LOW_POWER_ENABLE

    config_low_power_mode();

#endif

    while (true)
    {
        vTaskDelay(20000 / portTICK_PERIOD_MS);

#if CONFIG_LOW_POWER_ENABLE

        esp_light_sleep_start();

#endif
    }
}