#include "esp_log.h"
#include "ic2_custom.h"

#define MPU6050_ADDRESS 0x69
#define MPU6050_SMPLRT_DIV 0x19
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_CONFIG 0x1A
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_INT_ENABLE 0x38
#define MPU6050_ACCEL_XOUT 0x3B
#define MPU6050_ACCEL_YOUT 0x3D
#define MPU6050_ACCEL_ZOUT 0x3F
#define MPU6050_TEMP_OUT 0x41
#define MPU6050_GYRO_XOUT 0x43
#define MPU6050_GYRO_YOUT 0x45
#define MPU6050_GYRO_ZOUT 0x47
#define MPU6050_WHO_I_AM 0x75
#define I2C_CLK_SPEED 100000

esp_err_t ulAccelerometerGetAddress (uint8_t *pucAddress);
esp_err_t ulAccelerometerInitialize (void);
int16_t sAccelerometerGetAccelX (void);
int16_t sAccelerometerGetAccelY (void);
int16_t sAccelerometerGetAccelZ (void);
int16_t sAccelerometerGetGyroX (void);
int16_t sAccelerometerGetGyroY (void);
int16_t sAccelerometerGetGyroZ (void);
float accelerometer_get_temperature (void);
uint8_t ucAccelerometerGetAddress (void);
int16_t sGetNumber (uint8_t ucMsb, uint8_t ucLsb);