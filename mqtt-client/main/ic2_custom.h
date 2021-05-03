#include "driver/i2c.h"

#define I2C_NACK_VAL 0x01
#define I2C_ACK_VAL 0x0
#define I2C_READ_BIT 0x1
#define I2C_WRITE_BIT 0x0
#define I2C_MASTER_PORT 0

esp_err_t ulI2CInitialize (uint32_t ulSpeed);
esp_err_t ulI2CReadByte (uint8_t ucDeviceAddress, uint8_t ucRegister, uint8_t *pucData);
esp_err_t ulI2CWriteByte (uint8_t ucDeviceAddress, uint8_t ucRegister, uint8_t ucData);