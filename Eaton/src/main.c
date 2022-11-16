/*
* Assume this is a compromised device with arbitrary code execution. 
* Assume peers are already initialized. So, we do not have to worry about ESP-NOW encryption or other issues
* 
*/
#include "esp_spi_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_random.h"
#include "esp_now.h"
#include <stdio.h>
#include <string.h>

const uint8_t peer_addr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#define SEND_SIZE 150
#define attack_addr 0x400d61f4
void fuzz() {

  // int buf[8] = {0};
  // buf[5] = attack_addr;
  //construct random data
  uint8_t* random = malloc(SEND_SIZE);
  esp_fill_random(random, SEND_SIZE); 
 // ESP_ERROR_CHECK(esp_now_send(peer_addr, (uint8_t*)buf, sizeof(buf)));
 ESP_ERROR_CHECK(esp_now_send(peer_addr, (uint8_t*)random, SEND_SIZE));
  free(random);
  //NOTE: in a real comprimised environment, peer would be NULL as we would send it to all paired peers
}



void app_main() {
    wifi_init_config_t wifi_conf = WIFI_INIT_CONFIG_DEFAULT();

    esp_now_peer_info_t peer = {0};
    memcpy(&peer.peer_addr, &peer_addr, sizeof(peer_addr));
    
    peer.ifidx = WIFI_IF_STA;

    uint8_t mac[6];

  // portTICK_PERIOD_MS is in units ms/tick

  printf("Initializing NVS\n");

  ESP_ERROR_CHECK(nvs_flash_init());

  printf("Initialize Wifi\n");
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_conf));
  printf("Starting Wifi in Station Mode\n");
  ESP_ERROR_CHECK(esp_wifi_start()); //

  // ESP-NOW stuff


  printf("Initializing ESP-NOW");
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_add_peer(&peer)); //add peer as a peer

  ESP_ERROR_CHECK(esp_wifi_get_mac(WIFI_IF_STA, mac));

  printf("MAC: %x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  fuzz();
  
  printf("Deinitializing ESP-NOW");
  ESP_ERROR_CHECK(esp_now_deinit());
  printf("Stopping WiFi\n");
  ESP_ERROR_CHECK(esp_wifi_stop());
  printf("Deinitializing Wifi\n");
  ESP_ERROR_CHECK(esp_wifi_deinit()); 

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  // esp_restart();
}