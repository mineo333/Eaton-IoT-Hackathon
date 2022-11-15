/*
* This is a simple example of code that the 
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

void attack_success(){
    printf("Reached Buffer Overflow Target!");
}

void recv_cb(const uint8_t *mac_addr, const uint8_t *data, int data_len) {

  char buf[8];
  printf("MAC: %x:%x:%x:%x:%x:%x\n", mac_addr[0], mac_addr[1], mac_addr[2],
         mac_addr[3], mac_addr[4], mac_addr[5]);

  printf("len: %d\n", data_len);

  // memcpy(buf, data, data_len); //induce a buffer overflow

  for (int i = 0; i < 0x20; i++) {
    //*((char *)buf + i) = *(data + i);
    printf("%x\n", *((char *)buf + i));
  }
}

void app_main() {
  wifi_init_config_t wifi_conf = WIFI_INIT_CONFIG_DEFAULT();

    esp_now_peer_info_t peer = {0};
    memcpy(&peer.peer_addr, &peer_addr, sizeof(peer_addr));
    
    peer.ifidx = WIFI_IF_STA;

    uint8_t mac[6];

  // portTICK_PERIOD_MS is in units ms/tick

  printf("Initializing NVS\n");

  printf("attack_success addr: %lx", (unsigned long)attack_success);

  ESP_ERROR_CHECK(nvs_flash_init());

  printf("Initialize Wifi\n");
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_conf));
  printf("Starting Wifi in Station Mode\n");
  ESP_ERROR_CHECK(esp_wifi_start()); //

  // ESP-NOW stuff
    attack_success();
  
  printf("Initializing ESP-NOW");
  ESP_ERROR_CHECK(esp_now_init());
  ESP_ERROR_CHECK(esp_now_add_peer(&peer)); //add peer as a peer. This ensures that the callback is called when we recv an ESPNOW packet
  ESP_ERROR_CHECK(esp_wifi_get_mac(WIFI_IF_STA, mac));
  printf("MAC: %x:%x:%x:%x:%x:%x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  ESP_ERROR_CHECK(esp_now_register_recv_cb(recv_cb)); //this is probably an interrupt. 
    for(;;){

    }



  ESP_ERROR_CHECK(esp_now_unregister_recv_cb());
  
  printf("Deinitializing ESP-NOW");
  ESP_ERROR_CHECK(esp_now_deinit());
  printf("Stopping WiFi\n");
  ESP_ERROR_CHECK(esp_wifi_stop());
  printf("Deinitializing Wifi\n");
  ESP_ERROR_CHECK(esp_wifi_deinit()); 

  //attack_success(); //prevents the compiler from complaining. Never reached due to infinite loop

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  // esp_restart();
}