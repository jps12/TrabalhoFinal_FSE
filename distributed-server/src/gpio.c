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

#include "cJSON.h"
#include "gpio.h"
#include "mqtt.h"

#define GPIO_LED_PIN 2
#define GPIO_BOTAO_PIN 0
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
        if (xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY))
        {
            swich_gpio_led_level();
            ESP_LOGI(TAG, "Botao acionado");
        }
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

#if CONFIG_LOW_POWER_ENABLE

    gpio_wakeup_enable(GPIO_BOTAO_PIN, GPIO_INTR_LOW_LEVEL);

#endif

    filaDeInterrupcao = xQueueCreate(10, sizeof(int));
    xTaskCreate(trataInterrupcaoBotao, "TrataBotao", 2048, NULL, 1, NULL);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_BOTAO_PIN, gpio_isr_handler_botao, (void *) GPIO_BOTAO_PIN);

    gpio_set_level(GPIO_LED_PIN, estado_led);

   
}

void swich_gpio_led_level()
{
    estado_led = !estado_led;
    gpio_set_level(GPIO_LED_PIN, estado_led);

    send_estado_botao_mqtt(estado_led);
}

int get_gpio_led_level()
{
    return estado_led;
}