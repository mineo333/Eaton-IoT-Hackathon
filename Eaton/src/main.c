/* Hello World Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_wifi.h"
#include "nvs_flash.h"



void app_main()
{
   wifi_init_config_t wifi_conf = WIFI_INIT_CONFIG_DEFAULT();
   
   wifi_scan_config_t scan = {0};
   
   uint16_t num_ap = 0;

     //portTICK_PERIOD_MS is in units ms/tick

    printf("Initializing NVS\n");

    ESP_ERROR_CHECK(nvs_flash_init());
    
    printf("Initialize Wifi\n");
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_conf));
    printf("Starting Wifi in Station Mode\n");
    ESP_ERROR_CHECK(esp_wifi_start());

    

    ESP_ERROR_CHECK(esp_wifi_stop());
    printf("Deinitializing Wifi\n");
    ESP_ERROR_CHECK(esp_wifi_deinit()); //surely this doesn't fail :)
    

    
reset:
    vTaskDelay(1000 / portTICK_PERIOD_MS);

   // esp_restart();
}