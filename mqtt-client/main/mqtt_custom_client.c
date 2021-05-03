#include "mqtt_custom_client.h"

static const char *MQTT_TAG = "MQTT_CLIENT";
static const char *ACCELEROMETER_X_TOPIC_NAME = "acc/x";
static const char *ACCELEROMETER_Y_TOPIC_NAME = "acc/y";
static const char *ACCELEROMETER_Z_TOPIC_NAME = "acc/z";
static const char *GYROSCOPE_X_TOPIC_NAME = "gyro/x";
static const char *GYROSCOPE_Y_TOPIC_NAME = "gyro/y";
static const char *GYROSCOPE_Z_TOPIC_NAME = "gyro/z";
static const char *TEMP_TOPIC_NAME = "temp/";

esp_mqtt_client_handle_t mqtt_client = NULL;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;

    SemaphoreHandle_t xSemaphoreMQTTConnection = (SemaphoreHandle_t)event->user_context;

    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        if (xSemaphoreMQTTConnection != NULL)
        {
            xSemaphoreGive(xSemaphoreMQTTConnection);
            ESP_LOGI(MQTT_TAG, "MQTT connection semaphore has been given.");
        }

        break;

    default:
        ESP_LOGI(MQTT_TAG, "Mqtt event '%d' has been occurred.", event->event_id);
        break;
    }
}

void mqtt_send_float(void *parameter)
{
    MQTTChannelConfig_t *config = (MQTTChannelConfig_t *)parameter;
    float value = 0;
    TickType_t ticks = (TickType_t)1000;
    const int BUF_SIZE = 10;
    char str_data[BUF_SIZE];

    memset(str_data, 0, BUF_SIZE);

    for (;;)
    {
        if (config->queue != NULL)
        {
            if (xQueueReceive(config->queue, &value, ticks) == pdPASS)
            {
                sprintf(str_data, "%f", value);
                esp_mqtt_client_publish(mqtt_client, mqtt_get_str_topic(config->topic), str_data, BUF_SIZE, 0, 0);
            }
        }
    }

    vTaskDelete(NULL);
}

void mqtt_send_16bit(void *parameter)
{
    MQTTChannelConfig_t *config = (MQTTChannelConfig_t *)parameter;
    int16_t value = 0;
    TickType_t ticks = (TickType_t)10;
    const int BUF_SIZE = 10;
    char str_data[BUF_SIZE];

    memset(str_data, 0, BUF_SIZE);

    for (;;)
    {
        if (config->queue != NULL)
        {
            if (xQueueReceive(config->queue, &value, ticks) == pdPASS)
            {
                sprintf(str_data, "%d", value);
                esp_mqtt_client_publish(mqtt_client, mqtt_get_str_topic(config->topic), str_data, BUF_SIZE, 0, 0);
            }
        }
    }

    vTaskDelete(NULL);
}

const char *mqtt_get_str_topic(MQTTTopic_t topic)
{
    switch (topic)
    {
    case TOPIC_TEMP:
        return TEMP_TOPIC_NAME;
        break;

    case TOPIC_ACCELX:
        return ACCELEROMETER_X_TOPIC_NAME;
        break;

    case TOPIC_ACCELY:
        return ACCELEROMETER_Y_TOPIC_NAME;
        break;

    case TOPIC_ACCELZ:
        return ACCELEROMETER_Z_TOPIC_NAME;
        break;

    case TOPIC_GYROX:
        return GYROSCOPE_X_TOPIC_NAME;
        break;

    case TOPIC_GYROY:
        return GYROSCOPE_Y_TOPIC_NAME;
        break;

    case TOPIC_GYROZ:
        return GYROSCOPE_Z_TOPIC_NAME;
        break;

    default:
        return NULL;
        break;
    }
}

void vMQTTInitialize(SemaphoreHandle_t xWifiSemaphore, SemaphoreHandle_t xSemaphoreMQTTConnection)
{

    if (xWifiSemaphore != NULL)
    {
        if (xSemaphoreTake(xWifiSemaphore, (TickType_t)10) == pdTRUE)
        {
            esp_mqtt_client_config_t config = {
                .uri = BROKER_IP,
                .client_cert_pem = (const char *)client_crt_start,
                .client_key_pem = (const char *)client_key_start,
                .user_context = xSemaphoreMQTTConnection};

            mqtt_client = esp_mqtt_client_init(&config);

            esp_mqtt_client_register_event(mqtt_client,
                                           ESP_EVENT_ANY_ID,
                                           mqtt_event_handler, mqtt_client);
            esp_mqtt_client_start(mqtt_client);
            xSemaphoreGive(xWifiSemaphore);
        }
    }
}

void mqtt_register_float_channel(MQTTChannelConfig_t *channel_config, char *channel_name)
{
    xTaskCreate(&mqtt_send_float, channel_name, 2048, (void *)channel_config, 1, NULL);
}

void mqtt_register_16bits_channel(MQTTChannelConfig_t *channel_config, char *channel_name)
{
    xTaskCreate(&mqtt_send_16bit, channel_name, 2048, (void *)channel_config, 1, NULL);
}