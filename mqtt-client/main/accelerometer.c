#include "accelerometer.h"

static const char *ACC_TAG = "ACCELEROMETER";
static SemaphoreHandle_t semaphore_accelerometer = NULL;

//  Initialize MPU6050
esp_err_t ulAccelerometerInitialize(void) {
    esp_err_t result = ulI2CInitialize(I2C_CLK_SPEED);

    semaphore_accelerometer = xSemaphoreCreateMutex();

    if (result != ESP_OK)  {
        ESP_LOGE(ACC_TAG, "Error in I2C module. Error number: %d", result);
    }

    result = ulI2CWriteByte(MPU6050_ADDRESS, MPU6050_SMPLRT_DIV, 0x07);
    if (result != ESP_OK) {
        ESP_LOGE(ACC_TAG, "Error when setting SMPLRT_DIV register. Error number: %d", result);
    }

    result = ulI2CWriteByte(MPU6050_ADDRESS, MPU6050_PWR_MGMT_1, 0x01);
    if (result != ESP_OK) {
        ESP_LOGE(ACC_TAG, "Error when setting PWR_MGMT_1 register. Error number: %d", result);
    }

    result = ulI2CWriteByte(MPU6050_ADDRESS, MPU6050_CONFIG, 0x00);
    if (result != ESP_OK) {
        ESP_LOGE(ACC_TAG, "Error when setting CONFIG register. Error number: %d", result);
    }

    result = ulI2CWriteByte(MPU6050_ADDRESS, MPU6050_ACCEL_CONFIG, 0x00);
    if (result != ESP_OK) {
        ESP_LOGE(ACC_TAG, "Error when setting ACCEL_CONFIG register. Error number: %d", result);
    }

    result = ulI2CWriteByte(MPU6050_ADDRESS, MPU6050_GYRO_CONFIG, 0x00);
    if (result != ESP_OK) {
        ESP_LOGE(ACC_TAG, "Error when setting GYRO_CONFIG register. Error number: %d", result);
    }

    ulI2CWriteByte(MPU6050_ADDRESS, MPU6050_INT_ENABLE, 0x01);
    if (result != ESP_OK) {
        ESP_LOGE(ACC_TAG, "Error when setting INT_ENABLE register. Error number: %d", result);
    }

    semaphore_accelerometer = xSemaphoreCreateMutex();
    xSemaphoreGive(semaphore_accelerometer);            // Inform when the initialization is complete

    return result;
}

int16_t sAccelerometerGetAccelX(void)
{
    uint8_t usAccelXH = 0;
    uint8_t usAccelXL = 0;

    for (;;)
    {
        if (xSemaphoreTake(semaphore_accelerometer, (TickType_t)10) == pdTRUE)  // Wait until the I2C module is release
        {
            esp_err_t error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT, &usAccelXH);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading ACCEL_XOUT_H register. Error number: %d", error);
                return 0;
            }

            error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_ACCEL_XOUT + 1, &usAccelXL);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading ACCEL_XOUT_L register. Error number: %d", error);
                return 0;
            }

            xSemaphoreGive(semaphore_accelerometer);    // Release the I2C module so other tasks can use it
            break;
        }
    }

    return sGetNumber(usAccelXH, usAccelXL);
}

int16_t sAccelerometerGetAccelY(void)
{
    uint8_t ucAccelYH = 0;
    uint8_t ucAccelYL = 0;

    for (;;)
    {
        if (xSemaphoreTake(semaphore_accelerometer, (TickType_t)10) == pdTRUE)  // Wait until the I2C module is release
        {
            esp_err_t error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_ACCEL_YOUT, &ucAccelYH);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading ACCEL_YOUT_H register. Error number: %d", error);
                return 0;
            }

            error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_ACCEL_YOUT + 1, &ucAccelYL);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading ACCEL_YOUT_L register. Error number: %d", error);
                return 0;
            }

            xSemaphoreGive(semaphore_accelerometer);    // Release the I2C module so other tasks can use it
            break;
        }
    }

    return sGetNumber(ucAccelYH, ucAccelYL);
}

int16_t sAccelerometerGetAccelZ(void)
{
    uint8_t usAccelZH = 0;
    uint8_t usAccelZL = 0;

    for (;;)
    {
        if (xSemaphoreTake(semaphore_accelerometer, (TickType_t)10) == pdTRUE)  // Wait until the I2C module is release
        {
            esp_err_t error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_ACCEL_ZOUT, &usAccelZH);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading ACCEL_ZOUT_H register. Error number: %d", error);
                return 0;
            }

            error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_ACCEL_ZOUT + 1, &usAccelZL);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading ACCEL_ZOUT_L register. Error number: %d", error);
                return 0;
            }

            xSemaphoreGive(semaphore_accelerometer);    // Release the I2C module so other tasks can use it
            break;
        }
    }

    return sGetNumber(usAccelZH, usAccelZL);
}

int16_t sAccelerometerGetGyroX(void)
{
    uint8_t usGyroXH = 0;
    uint8_t usGyroXL = 0;

    for (;;)
    {
        if (xSemaphoreTake(semaphore_accelerometer, (TickType_t)10) == pdTRUE)  // Wait until the I2C module is release
        {
            esp_err_t error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_GYRO_XOUT, &usGyroXH);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading GYRO_XOUT_H register. Error number: %d", error);
                return 0;
            }

            error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_GYRO_XOUT + 1, &usGyroXL);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading GYRO_XOUT_L register. Error number: %d", error);
                return 0;
            }

            xSemaphoreGive(semaphore_accelerometer);    // Release the I2C module so other tasks can use it
            break;
        }
    }

    return sGetNumber(usGyroXH, usGyroXL);
}

int16_t sAccelerometerGetGyroY(void)
{
    uint8_t usGyroYH = 0;
    uint8_t usGyroYL = 0;

    for (;;)
    {
        if (xSemaphoreTake(semaphore_accelerometer, (TickType_t)10) == pdTRUE)  // Wait until the I2C module is release
        {
            esp_err_t error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_GYRO_YOUT, &usGyroYH);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading GYRO_YOUT_H register. Error number: %d", error);
                return 0;
            }

            error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_GYRO_YOUT + 1, &usGyroYL);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading GYRO_YOUT_L register. Error number: %d", error);
                return 0;
            }

            xSemaphoreGive(semaphore_accelerometer);    // Release the I2C module so other tasks can use it
            break;
        }
    }

    return sGetNumber(usGyroYH, usGyroYL);
}

int16_t sAccelerometerGetGyroZ(void)
{
    uint8_t usGyroZH = 0;
    uint8_t usGyroZL = 0;

    for (;;)
    {
        if (xSemaphoreTake(semaphore_accelerometer, (TickType_t)10) == pdTRUE)  // Wait until the I2C module is release
        {
            esp_err_t error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_GYRO_ZOUT, &usGyroZH);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading GYRO_ZOUT_H register. Error number: %d", error);
                return 0;
            }

            error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_GYRO_ZOUT + 1, &usGyroZL);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading GYRO_ZOUT_L register. Error number: %d", error);
                return 0;
            }

            xSemaphoreGive(semaphore_accelerometer);    // Release the I2C module so other tasks can use it
            break;
        }
    }

    return sGetNumber(usGyroZH, usGyroZL);
}

float accelerometer_get_temperature(void)
{
    uint8_t usTempH = 0;
    uint8_t usTempL = 0;
    uint8_t raw_data = 0;

    for (;;)
    {
        if (xSemaphoreTake(semaphore_accelerometer, (TickType_t)10) == pdTRUE)  // Wait until the I2C module is release
        {
            esp_err_t error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_TEMP_OUT, &usTempH);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading TEMP_OUT_H register. Error number: %d", error);
                return 0;
            }

            error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_TEMP_OUT + 1, &usTempL);

            if (error != ESP_OK)
            {
                ESP_LOGE(ACC_TAG, "Error reading TEMP_OUT_L register. Error number: %d", error);
                return 0;
            }

            raw_data = (8 << ((int16_t)usTempH)) | ((int16_t)usTempL);

            xSemaphoreGive(semaphore_accelerometer);    // Release the I2C module so other tasks can use it
            break;
        }
    }

    return (raw_data / 340.00) + 36.53;
}

uint8_t ucAccelerometerGetAddress(void)
{
    uint8_t ucAddress = 0;
    esp_err_t error = ulI2CReadByte(MPU6050_ADDRESS, MPU6050_WHO_I_AM, &ucAddress);

    if (error != ESP_OK)
    {
        ESP_LOGE(ACC_TAG, "Error reading MPU6050_WHO_I_AM register. Error number: %d", error);
        return 0;
    }

    return ucAddress;
}

int16_t sGetNumber(uint8_t ucMsb, uint8_t ucLsb)
{
    int32_t lResult = (((int16_t)ucMsb) << 8) | ((int16_t)ucLsb);

    if (lResult >= 32768)
        lResult -= 65536;

    return (int16_t)lResult;
}