#include "mqtt_client.h"
#include "esp_log.h"
#include "string.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "stdarg.h"

#define BROKER_IP CONFIG_MQTT_SERVER_IP
#define DEVICE_ID CONFIG_MQTT_DEVICE_ID

extern const uint8_t client_crt_start[] asm("_binary_client_crt_start");
extern const uint8_t client_crt_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_end[] asm("_binary_client_key_end");

typedef enum {    
    TOPIC_ACCELX, 
    TOPIC_ACCELY, 
    TOPIC_ACCELZ,
    TOPIC_GYROX,
    TOPIC_GYROY,
    TOPIC_GYROZ,
    TOPIC_TEMP
} MQTTTopic_t;

typedef struct {
    QueueHandle_t queue;
    MQTTTopic_t topic;
    float sumSample;
    int countSamples;
} MQTTChannelConfig_t;

void vMQTTInitialize (SemaphoreHandle_t xWifiSemaphore, SemaphoreHandle_t xSemaphoreMQTTConnection);
void mqtt_register_channel(MQTTChannelConfig_t *channel_config, char * channel_name) ;
const char * mqtt_get_str_topic (MQTTTopic_t topic);
void mqtt_register_16bits_channel(MQTTChannelConfig_t *channel_config, char * channel_name);
void mqtt_register_float_channel(MQTTChannelConfig_t *channel_config, char * channel_name);