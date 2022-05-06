/* Blink Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/rtc_io.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "dht11.h"

#include "cJSON.h"
#include "gpio.h"
#include "mqtt.h"

#define GPIO_LED_PIN 2
#define GPIO_BOTAO_PIN 0
#define GPIO_TEMPERATURA_PIN 4
#define TAG "GPIO"

xQueueHandle filaDeInterrupcao;

int estado_led = 0;

static void IRAM_ATTR gpio_isr_handler_botao(void *args)
{
    int pino = (int)args;
    xQueueSendFromISR(filaDeInterrupcao, &pino, NULL);
}

void trataInterrupcaoBotao(void *params)
{
    int pino;

    while (true)
    {
#if CONFIG_LOW_POWER_ENABLE

        if (rtc_gpio_get_level(GPIO_BOTAO_PIN) == 0)
        {
            if (xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY))
            {
                ESP_LOGI(TAG, "Botao acionado");
                troca_gpio_led_estado();
            }
            ESP_LOGI(TAG, "Esperando botao ser solto...");
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }
        
#else

        if (xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY))
        {
            ESP_LOGI(TAG, "Botao acionado");
            troca_gpio_led_estado();
        }

#endif

    }
}

void config_gpio()
{
    gpio_pad_select_gpio(GPIO_LED_PIN);
    gpio_set_direction(GPIO_LED_PIN, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(GPIO_BOTAO_PIN);
    gpio_set_direction(GPIO_BOTAO_PIN, GPIO_MODE_INPUT);

    gpio_pulldown_en(GPIO_BOTAO_PIN);
    gpio_pullup_dis(GPIO_BOTAO_PIN);

    gpio_set_intr_type(GPIO_BOTAO_PIN, GPIO_INTR_POSEDGE);

    DHT11_init(GPIO_TEMPERATURA_PIN);

#if CONFIG_LOW_POWER_ENABLE

    gpio_wakeup_enable(GPIO_BOTAO_PIN, GPIO_INTR_LOW_LEVEL);

#endif

    filaDeInterrupcao = xQueueCreate(10, sizeof(int));
    xTaskCreate(trataInterrupcaoBotao, "TrataBotao", 4096, NULL, 1, NULL);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_BOTAO_PIN, gpio_isr_handler_botao, (void *) GPIO_BOTAO_PIN);

    gpio_set_level(GPIO_LED_PIN, estado_led);

   
}

void troca_gpio_led_estado()
{
    estado_led = !estado_led;
    gpio_set_level(GPIO_LED_PIN, estado_led);

    mqtt_envia_estado_botao(estado_led);
}

int get_temperatura_gpio()
{
    struct dht11_reading tmp = DHT11_read();
    return tmp.temperature;
}

int get_umidade_gpio()
{
    struct dht11_reading tmp = DHT11_read();
    return tmp.humidity;
}