#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "freertos/semphr.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "wifi_custom_station.h"
#include "mqtt_custom_client.h"
#include "accelerometer.h"

float mqtt_get_accelerometer_value_16bit(MQTTTopic_t topic);
void task_get_temperature(void *parameter);
void task_get_accel_gyro(void *parameter);
void task_create_channel_tasks(void *params);

static const char *MAIN_TAG = "MAIN";
static SemaphoreHandle_t wifi_semaphore = NULL;
static SemaphoreHandle_t xSemaphoreMQTTConnection = NULL;

MQTTChannelConfig_t channel_accel_x;
MQTTChannelConfig_t channel_accel_y;
MQTTChannelConfig_t channel_accel_z;
MQTTChannelConfig_t channel_gyro_x;
MQTTChannelConfig_t channel_gyro_y;
MQTTChannelConfig_t channel_gyro_z;
MQTTChannelConfig_t channel_temp;

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

    esp_err_t error = nvs_flash_init();
    uint8_t ucAccelerometerAddress = 0x0;

    if (error == ESP_ERR_NVS_NO_FREE_PAGES || error == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        error = nvs_flash_init();
    }

    ESP_ERROR_CHECK(error);

    wifi_semaphore = xSemaphoreCreateBinary();
    xSemaphoreMQTTConnection = xSemaphoreCreateBinary();

    if (wifi_semaphore != NULL && xSemaphoreMQTTConnection != NULL)
    {
        vWIFIInitialize(wifi_semaphore);
        vMQTTInitialize(wifi_semaphore, xSemaphoreMQTTConnection);
        ulAccelerometerInitialize();
        xTaskCreate(&task_create_channel_tasks, "create_channel_tasks", 2048, NULL, 2, NULL);
        ucAccelerometerAddress = ucAccelerometerGetAddress();
        ESP_LOGI(MAIN_TAG, "Accelerometer address check: 0x%x", ucAccelerometerAddress);
    }
}

void task_create_channel_tasks(void *params)
{
    while (true)
    {
        if (xSemaphoreMQTTConnection != NULL)
        {
            if (xSemaphoreTake(xSemaphoreMQTTConnection, (TickType_t)10) == pdTRUE)
            {
                xTaskCreate(&task_get_accel_gyro, "task_get_accel_x", 2048, &channel_accel_x, 2, NULL);
                xTaskCreate(&task_get_accel_gyro, "task_get_accel_y", 2048, &channel_accel_y, 2, NULL);
                xTaskCreate(&task_get_accel_gyro, "task_get_accel_z", 2048, &channel_accel_z, 2, NULL);
                xTaskCreate(&task_get_accel_gyro, "task_get_gyro_x", 2048, &channel_gyro_x, 2, NULL);
                xTaskCreate(&task_get_accel_gyro, "task_get_gyro_y", 2048, &channel_gyro_y, 2, NULL);
                xTaskCreate(&task_get_accel_gyro, "task_get_gyro_z", 2048, &channel_gyro_z, 2, NULL);
                xTaskCreate(&task_get_temperature, "task_get_temperature", 2048, &channel_temp, 2, NULL);

                channel_accel_x.topic = TOPIC_ACCELX;
                channel_accel_x.queue = xQueueCreate(1000, sizeof(float));

                channel_accel_y.topic = TOPIC_ACCELY;
                channel_accel_y.queue = xQueueCreate(1000, sizeof(float));

                channel_accel_z.topic = TOPIC_ACCELZ;
                channel_accel_z.queue = xQueueCreate(1000, sizeof(float));

                channel_gyro_x.topic = TOPIC_GYROX;
                channel_gyro_x.queue = xQueueCreate(1000, sizeof(float));

                channel_gyro_y.topic = TOPIC_GYROY;
                channel_gyro_y.queue = xQueueCreate(1000, sizeof(float));

                channel_gyro_z.topic = TOPIC_GYROZ;
                channel_gyro_z.queue = xQueueCreate(1000, sizeof(float));

                channel_temp.topic = TOPIC_TEMP;
                channel_temp.queue = xQueueCreate(1000, sizeof(float));

                mqtt_register_float_channel(&channel_accel_x, "channel_accel_x_task");
                mqtt_register_float_channel(&channel_accel_y, "channel_accel_y_task");
                mqtt_register_float_channel(&channel_accel_z, "channel_accel_x_task");
                mqtt_register_float_channel(&channel_gyro_x, "channel_gyro_x_task");
                mqtt_register_float_channel(&channel_gyro_y, "channel_gyro_y_task");
                mqtt_register_float_channel(&channel_gyro_z, "channel_gyro_z_task");
                mqtt_register_float_channel(&channel_temp, "channel_temp_task");
                break;
            }
        }
    }

    vTaskDelete(NULL);
}

void task_get_temperature(void *parameter)
{
    MQTTChannelConfig_t *channel_config = (MQTTChannelConfig_t *)parameter;
    TickType_t ticks = (TickType_t)10;
    float value = 0;

    for (;;)
    {
        if (channel_config->queue != NULL)
        {
            value = accelerometer_get_temperature();
            xQueueSend(channel_config->queue, (void *)&value, ticks);
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void task_get_accel_gyro(void *parameter)
{
    MQTTChannelConfig_t *channel_config = (MQTTChannelConfig_t *)parameter;
    TickType_t ticks = (TickType_t)10;
    float value = 0;

    for (;;)
    {
        if (channel_config->queue != NULL)
        {
            value = mqtt_get_accelerometer_value_16bit(channel_config->topic);

            if (channel_config->countSamples < 15)
            {
                channel_config->sumSample += value;
                channel_config->countSamples++;
            }
            else
            {
                value = channel_config->sumSample / 15;
                xQueueSend(channel_config->queue, (void *)&value, ticks);
                channel_config->countSamples = 0;
                channel_config->sumSample = 0;
            }
        }
    }

    vTaskDelete(NULL);
}

float mqtt_get_accelerometer_value_16bit(MQTTTopic_t topic)
{
    int16_t raw_value = 0;

    switch (topic)
    {
    case TOPIC_ACCELX:
        raw_value = sAccelerometerGetAccelX();
        //ESP_LOGI(MAIN_TAG, "AccelX: %d", raw_value);
        break;

    case TOPIC_ACCELY:
        raw_value = sAccelerometerGetAccelY();
        //ESP_LOGI(MAIN_TAG, "AccelY: %d", raw_value);
        break;

    case TOPIC_ACCELZ:
        raw_value = sAccelerometerGetAccelZ();
        //ESP_LOGI(MAIN_TAG, "AccelZ: %d", raw_value);
        break;

    case TOPIC_GYROX:
        raw_value = sAccelerometerGetGyroX();
        //ESP_LOGI(MAIN_TAG, "GyroX: %d", raw_value);
        break;

    case TOPIC_GYROY:
        raw_value = sAccelerometerGetGyroY();
        //ESP_LOGI(MAIN_TAG, "GyroY: %d", raw_value);
        break;
    
    case TOPIC_GYROZ:
        raw_value = sAccelerometerGetGyroZ();
        //ESP_LOGI(MAIN_TAG, "GyroZ: %d", raw_value);
        break;

    default:
        raw_value = 0;
        break;
    }

    return raw_value;
}