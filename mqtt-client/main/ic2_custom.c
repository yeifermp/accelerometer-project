#include "ic2_custom.h"

//  Iintialize the I2C module
//  ulSpeed: Clock speed used to initialize the I2C in master mode
esp_err_t ulI2CInitialize(uint32_t ulSpeed) {
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER,                //  Configure I2C module in master mode
        .sda_io_num = GPIO_NUM_21,              //  Set GPIO21 as SDA pin
        .sda_pullup_en = GPIO_PULLUP_ENABLE,    //  Enable pull-down resistor in SDA pin
        .scl_io_num = GPIO_NUM_22,              //  Enable GPIO22 as SCL pin
        .scl_pullup_en = GPIO_PULLUP_ENABLE,    //  Enable pull-up resistor in SCL pin
        .master.clk_speed = ulSpeed
    };

    i2c_param_config(I2C_MASTER_PORT, &config); // Configure I2C module
    return i2c_driver_install(I2C_MASTER_PORT, I2C_MODE_MASTER, 512, 512, 0);   //  Install the I2C module, this module is not installed by default
}

//  Read a single byte from a slave device
//  ucDeviceAddress: slave address
//  ucRegister: Register to be read
//  pucData: buffer with the byte that has been read
esp_err_t ulI2CReadByte (uint8_t ucDeviceAddress, uint8_t ucRegister, uint8_t *pucData) {
    esp_err_t result = ESP_OK;
    i2c_cmd_handle_t cmd;    

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);                                                          // Send start bit
    i2c_master_write_byte(cmd, (ucDeviceAddress << 1) | I2C_WRITE_BIT, I2C_ACK_VAL);// Send the slave address with the write/read bit in write mode
    i2c_master_write_byte(cmd, ucRegister, I2C_ACK_VAL);                            // Send the register to be read
    i2c_master_start(cmd);                                                          // Send the start bit again in order to start the reading
    i2c_master_write_byte(cmd, (ucDeviceAddress << 1) | I2C_READ_BIT, I2C_ACK_VAL); // Send the slave address with the write/read bit in read mode
    i2c_master_read_byte(cmd, pucData, I2C_NACK_VAL);                               // Read the data
    i2c_master_stop(cmd);                                                           // Send the stop bit to end the communication
    
    result = i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return result;
}

//  Write a single byte to a slave device
//  ucDeviceAddress: slave address
//  ucRegister: Register to be write
//  ucData: byte of data to be write
esp_err_t ulI2CWriteByte (uint8_t ucDeviceAddress, uint8_t ucRegister, uint8_t ucData) {
    esp_err_t result = ESP_OK;
    i2c_cmd_handle_t cmd;    

    cmd = i2c_cmd_link_create();    
    i2c_master_start(cmd);                                                              // Send start bit
    i2c_master_write_byte(cmd, (ucDeviceAddress << 1) | I2C_WRITE_BIT, I2C_ACK_VAL);    // Send the slave address with the write/read bit in write mode
    i2c_master_write_byte(cmd, ucRegister, I2C_ACK_VAL);                                // Send the register to be write
    i2c_master_write_byte(cmd, ucData, I2C_NACK_VAL);                                   // Send the data to be write in the slave device
    i2c_master_stop(cmd);                                                               // Send the stop bit to end the communication
    
    result = i2c_master_cmd_begin(I2C_MASTER_PORT, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return result;
}