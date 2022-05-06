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
#include "esp32/rom/uart.h"

#include "wifi.h"
#include "flash.h"
#include "http_client.h"
#include "mqtt.h"
#include "cJSON.h"
#include "gpio.h"

#define MAC_ADDRESS "00:00:00:00:00"

// - framework-espidf 3.40302.0 (4.3.2) 

xSemaphoreHandle conexaoWifiSemaphore;
xSemaphoreHandle conexaoMQTTSemaphore;

void config_low_power_mode()
{
    esp_sleep_enable_gpio_wakeup();
    esp_sleep_enable_timer_wakeup(1 * 1000000); // Tempo dado em microsegundos
}


void IniciaMQTTConfig(void *params){
    while (true){
        if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)){
            mqtt_start();
        }
    }
}

void IniciaSleepLowPower(void *params){
    while (true){
        if (xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY)){
            esp_light_sleep_start();
        }
    }
}


void config_app(){
    Flash_init();

    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();

    wifi_config();

    xTaskCreate(&IniciaMQTTConfig, "Configura MQTT inicialmente", 4096, NULL, 1, NULL);

    mqtt_envia_mac(MAC_ADDRESS);
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

#if CONFIG_LOW_POWER_ENABLE

        uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

        vTaskDelay(10000 / portTICK_PERIOD_MS);

        esp_light_sleep_start();


#else
        float temperatura = 0, umidade = 0;

        for (int i=0; i<5; i++)
        {
            temperatura += get_temperatura_gpio();
            umidade += get_umidade_gpio();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        mqtt_envia_dht11_info("umidade", umidade/5.0);
        mqtt_envia_dht11_info("temperatura", temperatura/5.0);

#endif
    }
}