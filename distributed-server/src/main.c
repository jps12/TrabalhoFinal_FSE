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

#include "wifi.h"
#include "flash.h"
#include "http_client.h"
#include "mqtt.h"

xSemaphoreHandle conexaoWifiSemaphore;

void RealizaHTTPRequest(void *params){
    while (true){
        if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)){
            ESP_LOGI("Main Task", "Realiza HTTP Request");
            mqtt_start();
        }
    }
}

void app_main() {
    Flash_init();

    conexaoWifiSemaphore = xSemaphoreCreateBinary();

    wifi_config();

    xTaskCreate(&RealizaHTTPRequest, "Processa HTTP", 4096, NULL, 1,NULL);
    

}