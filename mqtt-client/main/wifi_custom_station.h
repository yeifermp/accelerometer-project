#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/semphr.h"
#include "esp_log.h"

#define WIFI_SSID CONFIG_WIFI_SSID
#define WIFI_PASS CONFIG_WIFI_PASSWORD
#define MAXIMUM_RETRY CONFIG_MAXIMUM_RETRY
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

void vWIFIInitialize (SemaphoreHandle_t xSemaphore);