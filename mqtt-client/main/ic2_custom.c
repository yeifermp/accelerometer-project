#include "ic2_custom.h"

esp_err_t ulI2CInitialize(uint32_t ulSpeed) {
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = GPIO_NUM_22,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = ulSpeed
    };

    i2c_param_config(I2C_MASTER_PORT, &config);
    return i2c_driver_install(I2C_MASTER_PORT, I2C_MODE_MASTER, 512, 512, 0);
}

esp_err_t ulI2CReadByte (uint8_t ucDeviceAddress, uint8_t ucRegister, uint8_t *pucData) {
    esp_err_t result = ESP_OK;
    i2c_cmd_handle_t cmd;    

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ucDeviceAddress << 1) | I2C_WRITE_BIT, I2C_ACK_VAL);
    i2c_master_write_byte(cmd, ucRegister, I2C_ACK_VAL); 
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ucDeviceAddress << 1) | I2C_READ_BIT, I2C_ACK_VAL);
    i2c_master_read_byte(cmd, pucData, I2C_NACK_VAL);
    i2c_master_stop(cmd);
    
    result = i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return result;
}

esp_err_t ulI2CWriteByte (uint8_t ucDeviceAddress, uint8_t ucRegister, uint8_t ucData) {
    esp_err_t result = ESP_OK;
    i2c_cmd_handle_t cmd;    

    cmd = i2c_cmd_link_create();    
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (ucDeviceAddress << 1) | I2C_WRITE_BIT, I2C_ACK_VAL);
    i2c_master_write_byte(cmd, ucRegister, I2C_ACK_VAL); 
    i2c_master_write_byte(cmd, ucData, I2C_NACK_VAL); 
    i2c_master_stop(cmd);
    
    result = i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return result;
}