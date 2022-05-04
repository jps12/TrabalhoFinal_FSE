#include "wifi.h"
#include "flash.h"

void app_main() {
    Flash_init();
    wifi_config();
}