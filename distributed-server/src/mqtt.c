/* MQTT (over TCP) Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "mqtt.h"
#include "cJSON.h"

#define TAG "MQTT"
#define URL_BROKER CONFIG_URL_BROKER_MQTT

extern xSemaphoreHandle conexaoMQTTSemaphore;

typedef struct mqtt_message
{
    char *topic;
    char *message;
}mqtt_message;

char *comodo = "/sala";
char *base_mqtt_topic = "/fse2021/180033743";
esp_mqtt_client_handle_t client_mqtt;

// mqtt://broker.emqx.io 


static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event){
    // esp_mqtt_client_handle_t client = event->client;

    switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        xSemaphoreGive(conexaoMQTTSemaphore);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_start(){
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = URL_BROKER,
    };

    client_mqtt = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client_mqtt, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client_mqtt);

}

void mqtt_envia_mensagem(char *message, char *topic){
    int msg_id = esp_mqtt_client_publish(client_mqtt, topic, message, 0, 1, 0);
    ESP_LOGI(TAG, "Mensagem enviada com id: %d", msg_id);
}


void mqtt_envia_estado_botao(int estado_led)
{
    cJSON *buffer_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(buffer_json, "estado", estado_led);

    char mqtt_topic_publish[256] = "";

    strcat(mqtt_topic_publish, base_mqtt_topic);
    strcat(mqtt_topic_publish, comodo);
    strcat(mqtt_topic_publish, "/estado");

    mqtt_envia_mensagem(cJSON_Print(buffer_json), mqtt_topic_publish);
}

void mqtt_envia_dht11_info(char *topic, float value)
{
    cJSON *buffer_json = cJSON_CreateObject();
    cJSON_AddNumberToObject(buffer_json, topic, value);

    char mqtt_topic_publish[256] = "";

    strcat(mqtt_topic_publish, base_mqtt_topic);
    strcat(mqtt_topic_publish, comodo);
    strcat(mqtt_topic_publish, "/");
    strcat(mqtt_topic_publish, topic);

    mqtt_envia_mensagem(cJSON_Print(buffer_json), mqtt_topic_publish);
}

void mqtt_envia_mac(char * mac_address)
{
    cJSON *buffer_json = cJSON_CreateObject();
    cJSON_AddStringToObject(buffer_json, "mac", mac_address);

    char mqtt_topic_publish[256] = "";

    strcat(mqtt_topic_publish, base_mqtt_topic);
    strcat(mqtt_topic_publish, "/dispositivos");

    ESP_LOGI(TAG, "Send to Topic: %s", mqtt_topic_publish);

    mqtt_envia_mensagem(cJSON_Print(buffer_json), mqtt_topic_publish);
}