#include <stdio.h>          // Standard input/output library
#include "esp_log.h"        // ESP32 logging library for debugging and error logging
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ui.h"
#include "bme280.h"  // Include the CAN driver for communication

TaskHandle_t bme280_TaskHandle;
static const char *TAG = "BME280";  // Tag used for ESP log output

static void     burst_read            (const bme280_t *config, int32_t* raw_pressure, int32_t* raw_temperature, int32_t* raw_humidity);
static void     read_calibration_data (bme280_t *config);
static void     write_configuration   (const bme280_t *config);
static int32_t  compensate_temperature(bme280_t *config, int32_t adc_T);
static uint32_t compensate_presure    (const bme280_t *config, int32_t adc_P);
static uint32_t compensate_humidity   (const bme280_t *config, int32_t adc_H);

static bme280_t bme280Obj;

void bme280UpdateLvgObjectCb(lv_timer_t * timer) 
{
    ESP_LOGI(TAG, "BME280 bme280UpdateLvgObjectCb");
    #if 0
    if (CAN_Clear) {
        memset(can_data, 0, sizeof(can_data));  // Clear the buffer when flag is set
        CAN_Clear = false;
    }
    lv_textarea_set_text(ui_CAN_Read_Area, can_data);  // Update the UI with the new CAN data
    #endif
}

void bmp280_task(void *arg)
{
#if 0
    // TWAI configuration settings for the CAN bus
    static const twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();  // Set CAN bus speed to 500 kbps
    static const twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();  // Accept all incoming CAN messages
    static const twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, TWAI_MODE_NORMAL);  // General configuration, set TX/RX GPIOs and mode
    lv_roller_set_selected(ui_CAN_Roller, 5, LV_ANIM_OFF);
    // Initialize the CAN communication interface
    IO_EXTENSION_Output(IO_EXTENSION_IO_5, 1);  // Select CAN communication interface (0 for USB, 1 for CAN)
    
    // Initialize the CAN communication system
    can_init(t_config, f_config, g_config);  // Initialize CAN with specified configurations

    uint32_t alerts_triggered;  // Variable to store triggered CAN bus alerts
    static twai_message_t message;  // Variable to store received CAN message
    char message_str[256];  // Buffer to store formatted message string

    while (1)
    {
        alerts_triggered = can_read_alerts();  // Check for any triggered CAN bus alerts

        // If new CAN data is received, process and display it
        if (alerts_triggered & TWAI_ALERT_RX_DATA) {
            message = can_read_Byte();  // Read the received CAN message

            // Format the CAN message into a string
            snprintf(message_str, sizeof(message_str),
                     "ID: 0x%03lX\nLength: %d\nData: ",
                     message.identifier, message.data_length_code);

            // Append data bytes to the string
            for (int i = 0; i < message.data_length_code; i++) {
                snprintf(message_str + strlen(message_str), sizeof(message_str) - strlen(message_str),
                         "%02X ", message.data[i]);
            }
            // Add newline at the end of the string
            snprintf(message_str + strlen(message_str), sizeof(message_str) - strlen(message_str), "\n");

            // Append the formatted message to the global CAN data buffer
            strncat(can_data, message_str, sizeof(can_data) - strlen(can_data) - 1);   

            // Create a timer to update the textarea every 100ms
            lv_timer_t * t = lv_timer_create(can_update_textarea_cb, 100, NULL);
            lv_timer_set_repeat_count(t, 1);  // Execute once
        }
    
        // If the CAN Clear flag is set, update the display again
        if (CAN_Clear) {
            lv_timer_t * t = lv_timer_create(can_update_textarea_cb, 100, NULL);  // Update every 100ms
            lv_timer_set_repeat_count(t, 1);  // Execute once
        }
    }
#endif
 uint32_t pressure;
 uint32_t humidity;
 int32_t temperature;
 char user_data[100];
  while (1)
    {
         // Create a timer to update the textarea every 100ms
         lv_timer_t * t = lv_timer_create(bme280UpdateLvgObjectCb, 100, NULL);
         lv_timer_set_repeat_count(t, 1);  // Execute once
         
         bme280_read_all(&bme280Obj, &pressure, &humidity, &temperature);

         int iPressure;
         int iPressureDec;

         int  iTemperature;
         int iTemperatureDec;

         int iHumidity;
         int iHumidityDec;

         float fPressure = (pressure / 256.0f)/100.0f; // Convert to hPa
         float fHumidity = humidity / 1024.0f;         // Convert to %RH
         float fTemperature = temperature / 100.0f;      // Convert to °C

         iPressure = pressure >> 8;
         iPressureDec= (pressure & 0xFF) * 100 >> 8;

         iTemperature = temperature / 100;
         iTemperatureDec = temperature % 100;

         iHumidity = humidity >> 10;
         iHumidityDec = (humidity &0x3FF) * 1000 >> 10;

         sprintf((char*)user_data, "Pressure: %03f Pa, Temperature: %03f degrees C, Humidity %03f%%\r\n",fPressure,fTemperature,fHumidity);
       
         ESP_LOGI(TAG, "%s", user_data);
         //ESP_LOGI(TAG, "Pressure %lu ",pressure);
         //ESP_LOGI(TAG, "Humidity %lu ",humidity);
      //   ESP_LOGI(TAG, "Temperature %d ",temperature);

         vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}


int8_t bme280_init()
{
    int8_t iRetVal = 0;
    uint8_t chip_id     = 0;
    uint8_t reg_address = BME280_ID_REG;

    ESP_LOGI(TAG, "Init BME280 On I2C "); // Log the BMP280 initialization

    DEV_I2C_Set_Slave_Addr(&bme280Obj.addr,BME280_I2C_ADDRESS); // Set the I2C slave address for BMP280

   
	bme280Obj.temperature_oversampling = OVERSAMPLING_x1;
	bme280Obj.pressure_oversampling = OVERSAMPLING_x1;
	bme280Obj.humidity_oversampling = OVERSAMPLING_x1;
	bme280Obj.standby = SB_1000MS;
	bme280Obj.filter = FILTER_OFF;
	bme280Obj.mode = NORMAL;

    // Read the chip ID from the BMP280
    DEV_I2C_Write_Nbyte(bme280Obj.addr, &reg_address, 1);
    chip_id = DEV_I2C_Read_Byte(bme280Obj.addr);
    if (chip_id != BME280_CHIP_ID)
    {
        iRetVal = -1;
    }
    else
    {
        ESP_LOGI(TAG, "BME280 Device ID %d ",chip_id);
        vTaskDelay(pdMS_TO_TICKS(10)); // Give the sensor 10 ms delay to reset.

        
        // get compensation values
        read_calibration_data(&bme280Obj);

        ESP_LOGI(TAG,"BME280 Dumping calibration...");
        write_configuration(&bme280Obj);
       
    }

    // Start the temperature monitoring task
    xTaskCreate(bmp280_task, "bmp280_task", 3 * 1024, NULL, 3, &bme280_TaskHandle);
    if (bme280_TaskHandle == NULL) {
        ESP_LOGE(TAG, "Failed to create bmp280_task");
    }
    
    return iRetVal;
}

int8_t bme280_read_all(bme280_t *ptrConfigObj, uint32_t* pressure, uint32_t* humidity, int32_t* temperature)
{
    int8_t iRetVal = 0;
    int32_t raw_pressure = 0;
    int32_t raw_temperature = 0;
    int32_t raw_humidity = 0;

    if (!ptrConfigObj)
    {  
        iRetVal = -1;
    }
    else
    {
        burst_read(ptrConfigObj, &raw_pressure, &raw_temperature, &raw_humidity);
        *temperature = compensate_temperature(ptrConfigObj, raw_temperature);
        *pressure    = compensate_presure   (ptrConfigObj, raw_pressure);
        *humidity    = compensate_humidity  (ptrConfigObj, raw_humidity);
    }

    return iRetVal;
}

static void read_calibration_data(bme280_t *ptrConfigObj)
{
    uint8_t data_buffer[BME280_CALIBRATION_LENGTH] = { 0 };
    uint8_t reg_address = BME280_CALIB00_REG;
    uint8_t data_length = BME280_CALIB23_REG - BME280_CALIB00_REG + 1;
    uint8_t data_offset = data_length;
    
    DEV_I2C_Read_Nbyte(ptrConfigObj->addr, reg_address, &data_buffer[0], data_length);
    //config->send(BME280_I2C_ADDRESS, &reg_address, 1);
    //config->receive(BME280_I2C_ADDRESS, data_buffer, data_length);

    reg_address = BME280_CALIB25_REG;
    DEV_I2C_Read_Nbyte(ptrConfigObj->addr, reg_address, &data_buffer[data_offset], 1);
    //config->send(BME280_I2C_ADDRESS, &reg_address, 1);
   // config->receive(BME280_I2C_ADDRESS, data_buffer + data_offset, 1);

    data_offset++;

    reg_address = BME280_CALIB26_REG;
    data_length = BME280_CALIB32_REG - BME280_CALIB26_REG + 1;
    DEV_I2C_Read_Nbyte(ptrConfigObj->addr, reg_address, &data_buffer[data_offset], 1);

    //config->send(BME280_I2C_ADDRESS, &reg_address, 1);
    //config->receive(BME280_I2C_ADDRESS, data_buffer + data_offset, data_length);

    ptrConfigObj->dig_H1 = data_buffer[0] | (uint16_t)data_buffer[1] << 8;
    ptrConfigObj->dig_T2 = data_buffer[2] | (int16_t)data_buffer[3] << 8;
    ptrConfigObj->dig_T3 = data_buffer[4] | (int16_t)data_buffer[5] << 8;

    ptrConfigObj->dig_P1 = data_buffer[6] | (uint16_t)data_buffer[7] << 8;
    ptrConfigObj->dig_P2 = data_buffer[8] | (int16_t)data_buffer[9] << 8;
    ptrConfigObj->dig_P3 = data_buffer[10] | (int16_t)data_buffer[11] << 8;
    ptrConfigObj->dig_P4 = data_buffer[12] | (int16_t)data_buffer[13] << 8;
    ptrConfigObj->dig_P5 = data_buffer[14] | (int16_t)data_buffer[15] << 8;
    ptrConfigObj->dig_P6 = data_buffer[16] | (int16_t)data_buffer[17] << 8;
    ptrConfigObj->dig_P7 = data_buffer[18] | (int16_t)data_buffer[19] << 8;
    ptrConfigObj->dig_P8 = data_buffer[20] | (int16_t)data_buffer[21] << 8;
    ptrConfigObj->dig_P9 = data_buffer[22] | (int16_t)data_buffer[23] << 8;

    ptrConfigObj->dig_H1 = data_buffer[24];
    ptrConfigObj->dig_H2 = data_buffer[25] | (int16_t)data_buffer[26] << 8;
    ptrConfigObj->dig_H3 = data_buffer[27];
    ptrConfigObj->dig_H4 = ((int8_t)data_buffer[28] << 4) | (data_buffer[29] & 0x0F);
    ptrConfigObj->dig_H5 = (data_buffer[29] >> 4) | ((int8_t)data_buffer[30] << 4);
    ptrConfigObj->dig_H6 = data_buffer[31];
}

static void write_configuration(const bme280_t *ptrConfigObj)
{
    uint8_t data_buffer[2];
    
    // write to ctrl_hum register
    data_buffer[0] = BME280_CTRL_HUM_REG;
    data_buffer[1] = ptrConfigObj->humidity_oversampling & 0x7;
    DEV_I2C_Write_Nbyte(ptrConfigObj->addr, &data_buffer[0], 2);
    //config->send(BME280_I2C_ADDRESS, data_buffer, 2);
    
    // write to ctrl_meas register
    data_buffer[0] = BME280_CTRL_MEAS_REG;
    data_buffer[1] = ptrConfigObj->temperature_oversampling << 5 | ptrConfigObj->pressure_oversampling << 2 | ptrConfigObj->mode;
    DEV_I2C_Write_Nbyte(ptrConfigObj->addr, &data_buffer[0], 2);
    //config->send(BME280_I2C_ADDRESS, data_buffer, 2);

    // write to config register
    data_buffer[0] = BME280_CONFIG_REG;
    data_buffer[1] = ptrConfigObj->standby << 5 | ptrConfigObj->filter << 2;
    DEV_I2C_Write_Nbyte(ptrConfigObj->addr, &data_buffer[0], 2);
    //config->send(BME280_I2C_ADDRESS, data_buffer, 2);
}

static void burst_read(const bme280_t *ptrConfigObj, int32_t *raw_pressure, int32_t *raw_temperature, int32_t *raw_humidity)
{
    uint8_t data_buffer[BME280_DATA_LENGTH];
    uint8_t reg_address = BME280_PRESS_MSB_REG;
    DEV_I2C_Read_Nbyte(ptrConfigObj->addr, reg_address, &data_buffer[0], BME280_DATA_LENGTH);
    
    //config->send(BME280_I2C_ADDRESS, &reg_address, 1);
    //config->receive(BME280_I2C_ADDRESS, data_buffer, BME280_DATA_LENGTH);

    *raw_pressure    = (uint32_t)data_buffer[0] << 12 | (uint16_t)data_buffer[1] << 4 | data_buffer[2] >> 4;
    *raw_temperature = (uint32_t)data_buffer[3] << 12 | (uint16_t)data_buffer[4] << 4 | data_buffer[5] >> 4;
    *raw_humidity    = (uint16_t)data_buffer[6] << 8 | data_buffer[7];
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC.
// This must be run first before compensate_pressure and compensate_humidity
static int32_t compensate_temperature( bme280_t *ptrConfigObj, int32_t adc_T)
{
    int32_t var1 = (((adc_T >> 3) - ((int32_t)ptrConfigObj->dig_T1 << 1)) * ((int32_t)ptrConfigObj->dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)ptrConfigObj->dig_T1)) * ((adc_T >> 4) - ((int32_t)ptrConfigObj->dig_T1))) >> 12) *
                   ((int32_t)ptrConfigObj->dig_T3)) >> 14;
    ptrConfigObj->fine_temperature = var1 + var2;
    int32_t temperature = (ptrConfigObj->fine_temperature * 5 + 128) / 256;
    return temperature;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 inteteger bits and 8 fractional bits)
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa
static uint32_t compensate_presure(const bme280_t *ptrConfigObj, int32_t adc_P)
{
    uint32_t u32Pressure;
    int64_t var1 = ((int64_t)ptrConfigObj->fine_temperature) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)ptrConfigObj->dig_P6;
    var2 += ((int64_t)ptrConfigObj->dig_P5) << 17;
    var2 += ((int64_t)ptrConfigObj->dig_P4) << 35;
    var1 = ((var1 * var1 * (int64_t)ptrConfigObj->dig_P3) >> 8) + ((var1 * (int64_t)ptrConfigObj->dig_P2) << 12);
    var1 = ((((int64_t)1) << 47) + var1) * ((int64_t)ptrConfigObj->dig_P1) >> 33;
    if (var1 == 0)
    {
        u32Pressure = 0; //avoid exception caused by division by zero
    }
    else
    {
        int64_t pressure = 1048576 - adc_P;
        pressure = (((pressure << 31) - var2) * 3125) / var1;
        var1 = (((int64_t)ptrConfigObj->dig_P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
        var2 = (((int64_t)ptrConfigObj->dig_P8) * pressure) >> 19;
        pressure = ((pressure + var1 + var2) >> 8) + (((int64_t)ptrConfigObj->dig_P7) << 4);
        u32Pressure = (uint32_t)pressure;
    }
    return u32Pressure;
}

// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits)
static uint32_t compensate_humidity(const bme280_t *ptrConfigObj, int32_t adc_H)
{
    int32_t humidity;
    humidity = ptrConfigObj->fine_temperature - ((int32_t)76800);
    humidity = ((((adc_H << 14) - (((int32_t)ptrConfigObj->dig_H4) << 20) - (((int32_t)ptrConfigObj->dig_H5) * humidity)) + ((int32_t)16384)) >> 15) *
                (((((((humidity * ((int32_t)ptrConfigObj->dig_H6)) >> 10) * (((humidity * ((int32_t)ptrConfigObj->dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
                ((int32_t)ptrConfigObj->dig_H2) + 8192) >> 14);

    humidity = (humidity - (((((humidity >> 15) * (humidity >> 15)) >> 7) * ((int32_t)ptrConfigObj->dig_H1)) >> 4));
    humidity = (humidity < 0 ? 0 : humidity);
    humidity = (humidity > 419430400 ? 419430400 : humidity);
    return (uint32_t)(humidity >> 12);
}

#if 0
static char can_data[2 * 1024] = {0};  // Buffer for receiving data

/**
 * @brief Callback function to update the textarea with CAN data.
 *
 * This function is triggered periodically by a timer to update the
 * text area with the latest received CAN data. It checks the CAN 
 * clear flag and clears the buffer if needed.
 *
 * @param timer The timer that triggers the callback.
 */
void can_update_textarea_cb(lv_timer_t * timer) {
    if (CAN_Clear) {
        memset(can_data, 0, sizeof(can_data));  // Clear the buffer when flag is set
        CAN_Clear = false;
    }
    lv_textarea_set_text(ui_CAN_Read_Area, can_data);  // Update the UI with the new CAN data
}

/**
 * @brief Task for handling CAN communication.
 *
 * This task is responsible for initializing the CAN interface, reading CAN 
 * messages, and updating the UI with the received data. It runs in an 
 * infinite loop, constantly checking for new CAN messages and formatting 
 * them for display.
 *
 * @param arg Task argument, not used in this function.
 */


/**
 * @brief Converts a hexadecimal string to an array of bytes.
 *
 * This function removes spaces from the input string and converts each pair
 * of hexadecimal characters to a byte, storing the result in the output array.
 * If the string contains invalid hexadecimal characters or cannot be fully 
 * converted, an error code is returned.
 *
 * @param input The input string to be converted.
 * @param output The output array to store the converted bytes.
 * @param max_output_size The maximum size of the output array.
 *
 * @return The number of bytes successfully converted, or a negative error code.
 */
int string_to_hex(const char *input, uint8_t *output, size_t max_output_size) {
    size_t len = 0;

    // Traverse the input string
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ') continue;  // Skip spaces

        // Check if the character is a valid hexadecimal digit
        if (!isxdigit((int)input[i])) return -2;  // Invalid hexadecimal character
        if (len / 2 >= max_output_size) return -1;  // Output buffer is full

        // Convert the character to its hexadecimal value and store it in the output array
        output[len / 2] = (output[len / 2] << 4) | (uint8_t)strtol((char[]){input[i], '\0'}, NULL, 16);
        len++;
    }

    return (len % 2 == 0) ? len / 2 : -1;  // Return the number of bytes if valid, or an error code if invalid
}
#endif

#if 0

/*!
 *   @brief  Writes an 8 bit value over I2C or SPI
 *   @param reg the register address to write to
 *   @param value the value to write to the register
 */
static void write8(uint8_t reg, uint8_t value) 
{
  uint8_t buffer[2];

  buffer[1] = value;
  buffer[0] = reg;  
  DEV_I2C_Write_Nbyte(bme280Obj.addr, buffer, 2);
}

/*!
 *   @brief  Reads an 8 bit value over I2C 
 *   @param reg the register address to read from
 *   @returns the data byte read from the device
 */
static uint8_t read8(uint8_t reg) 
{
  uint8_t buffer[1];

  buffer[0] = reg;
  DEV_I2C_Write_Nbyte(bme280Obj.addr, buffer, 1);
  buffer[0] = DEV_I2C_Read_Byte(bme280Obj.addr);
  return buffer[0];
}


/**
 * @brief Initialize the PWM module and start the battery voltage monitoring task.
 * 
 * This function initializes the PWM module with a 1 kHz frequency, sets the initial duty cycle,
 * and starts a FreeRTOS task to periodically measure and update the battery voltage.
 */
void bme280_init()
{
    ESP_LOGI(TAG, "Init BME280 On I2C "); // Log the BMP280 initialization

    DEV_I2C_Set_Slave_Addr(&bme280Obj.addr,BME280_ADDR); // Set the I2C slave address for BMP280

    int chipID = GetChipID();
    if (chipID != 0x60) {
        ESP_LOGE(TAG, "Could not find a valid BME280 sensor, check wiring! ID: 0x%02X", chipID);
        return;
    }

    // Start the temperature monitoring task
    xTaskCreate(bmp280_task, "bmp280_task", 3 * 1024, NULL, 3, &bme280_TaskHandle);
    if (bme280_TaskHandle == NULL) {
        ESP_LOGE(TAG, "Failed to create bmp280_task");
    }
}



int readTemperature(float *fTemperatureC)
{
    int iRet = 0;

    return iRet;
}
    #endif