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

// - framework-espidf 3.40302.0 (4.3.2) 

xSemaphoreHandle conexaoWifiSemaphore;

char *comodo = "/sala";
char *base_mqtt_topic = "/fse2021/180033743";

void IniciaMQTTRequest(void *params){
    while (true){
        if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)){
            ESP_LOGI("Main Task", "Inicia MQTT Request");

            cJSON *buffer_json = cJSON_CreateObject();
            cJSON_AddStringToObject(buffer_json, "mac", MAC_ADDRESS);
            
            char mqtt_topic_publish[256] = "";

            strcat(mqtt_topic_publish, base_mqtt_topic);
            strcat(mqtt_topic_publish, "/dispositivos");

            mqtt_send_message(cJSON_Print(buffer_json), mqtt_topic_publish);
        }
    }
}

void send_estado_botao_mqtt(int estado_led)
{
    cJSON *buffer_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(buffer_json, "estado", estado_led);

    char mqtt_topic_publish[256] = "";

    strcat(mqtt_topic_publish, base_mqtt_topic);
    strcat(mqtt_topic_publish, comodo);
    strcat(mqtt_topic_publish, "/estado");

    mqtt_send_message(cJSON_Print(buffer_json), mqtt_topic_publish);
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

    while (true)
    {
        int estado;
        for (int i=0; i<5; i++){ // To take 10 seconds before send mqtt message
            estado = get_gpio_led_level();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        send_estado_botao_mqtt(estado);
    }
}