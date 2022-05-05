/* Blink Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define GPIO_LED_PIN 2

void config_gpio()
{
    gpio_pad_select_gpio(GPIO_LED_PIN);
    gpio_set_direction(GPIO_LED_PIN, GPIO_MODE_OUTPUT);

    int estado = 1;
    while (true)
    {
        gpio_set_level(GPIO_LED_PIN, estado);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        estado = !estado;
    }
   
}