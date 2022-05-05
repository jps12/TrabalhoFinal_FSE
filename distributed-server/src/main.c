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
#include "cJSON.h"
#include "gpio.h"

#define MAC_ADDRESS "00:00:00:00:00"
xSemaphoreHandle conexaoWifiSemaphore;


void IniciaMQTTRequest(void *params){
    while (true){
        if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)){
            ESP_LOGI("Main Task", "Inicia MQTT Request");

            cJSON *buffer_json = cJSON_CreateObject();
            cJSON_AddStringToObject(buffer_json, "mac", MAC_ADDRESS);

            
            mqtt_send_message(cJSON_Print(buffer_json), "/fse2021/180033743/dispositivos");
        }
    }
}

void config_app(){
    Flash_init();

    conexaoWifiSemaphore = xSemaphoreCreateBinary();

    wifi_config();

    xTaskCreate(&IniciaMQTTRequest, "Envia MQTT mensagem de inicio", 4096, NULL, 1,NULL);
}

void app_main() {
    
    config_app();

    config_gpio();
}