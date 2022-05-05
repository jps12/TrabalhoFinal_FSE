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

#define MAC_ADDRESS "00:00:00:00:00"
xSemaphoreHandle conexaoWifiSemaphore;


void IniciaMQTTRequest(void *params){
    while (true){
        if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)){
            ESP_LOGI("Main Task", "Inicia MQTT Request");
            mqtt_send_message("Teste", "/fse2021/180033743/dispositivos");
        }
    }
}

void app_main() {
    Flash_init();

    conexaoWifiSemaphore = xSemaphoreCreateBinary();

    wifi_config();

    xTaskCreate(&IniciaMQTTRequest, "Processa MQTT", 4096, NULL, 1,NULL);
    
}