#include "wifi.h"
#include "flash.h"
#include "mqtt.h"

void app_main() {
    Flash_init();
    wifi_config();
    mqtt_start();
}