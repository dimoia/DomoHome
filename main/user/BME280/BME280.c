#include <stdio.h>          // Standard input/output library
#include "esp_log.h"        // ESP32 logging library for debugging and error logging
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl_port.h" 
#include "lvgl.h"
#include "ui.h"
#include "bme280.h"  // Include the CAN driver for communication

TaskHandle_t bme280_TaskHandle;
static const char *TAG = "BME280";  // Tag used for ESP log output
static bme280_t bme280Obj;  // Global BME280 configuration object

static int      bme280_Burst_Read            (const bme280_t *config, int32_t* raw_pressure, int32_t* raw_temperature, int32_t* raw_humidity);
static bool     bme280_isOn                  (bme280_t *ptrConfigObj);
static int      bme280_Write_Configuration   (const bme280_t *config);
static int      bme280_Read_Calibration_Data (bme280_t *ptrConfigObj);

static int32_t  bme280_Compensate_Temperature(bme280_t *config, int32_t adc_T);
static uint32_t bme280_Compensate_Presure    (const bme280_t *config, int32_t adc_P);
static uint32_t bme280_Compensate_Humidity   (const bme280_t *config, int32_t adc_H);



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
    uint32_t u32Pressure;
    uint32_t u32Humidity;
    int32_t  i32Temperature;
    char user_data[100];
    char strTemp[15];
    char strPressure[15];
    char strHumidity[15];
    objects_t objs = objects;
    lv_obj_t *Lbl_temperature    = objs.lbl_temperature;
    lv_obj_t *Lbl_temp_value     = objs.lbl_temp_value;

    lv_obj_t *Lbl_pressure       = objs.lbl_pressure;
    lv_obj_t *Lbl_press_value    = objs.lbl_press_value;

    lv_obj_t *Lbl_humidity       = objs.lbl_humidity;
    lv_obj_t *Lbl_humidity_value = objs.lbl_humidity_value;

    lv_obj_t * chart_temp_humy =  objs.chart_temp_humy;

    lv_chart_series_t * ser_temp;
    lv_chart_series_t * ser_humi;

    lv_chart_set_type(chart_temp_humy, LV_CHART_TYPE_LINE);

    lv_obj_set_style_line_width(chart_temp_humy, 1, LV_PART_TICKS);
    lv_chart_set_point_count(chart_temp_humy, 150); // Set the number of points on the chart
    
    ser_temp = lv_chart_add_series(chart_temp_humy, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);  
    ser_humi = lv_chart_add_series(chart_temp_humy, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_SECONDARY_Y);
    lv_chart_set_axis_tick(chart_temp_humy, LV_CHART_AXIS_SECONDARY_Y, 10, 5, 5, 2, true, 40);
    lv_chart_set_range(chart_temp_humy, LV_CHART_AXIS_SECONDARY_Y, 0, 100);

    lv_chart_set_axis_tick(chart_temp_humy, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 40);
    lv_chart_set_range(chart_temp_humy, LV_CHART_AXIS_PRIMARY_Y, -10, 50);
    //  lv_obj_set_style_local_line_width(chart_temp_humy, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 1);
    while (1)
    {
         // Create a timer to update the textarea every 100ms
         lv_timer_t * t = lv_timer_create(bme280UpdateLvgObjectCb, 100, NULL);
         lv_timer_set_repeat_count(t, 1);  // Execute once
         
         bme280_Read_Pressure_Temperature_Humidity(&bme280Obj, &u32Pressure, &u32Humidity, &i32Temperature);

         float fPressure    = (u32Pressure   / 256.0f)/100.0f; // Convert to hPa
         float fHumidity    = u32Humidity    / 1024.0f;        // Convert to %RH
         float fTemperature = i32Temperature / 100.0f;         // Convert to °C
         sprintf((char*)user_data, "Pressure: %03f Pa, Temperature: %03f degrees C, Humidity %03f%%\r\n",fPressure,fTemperature,fHumidity);
         //ESP_LOGI(TAG, "%s", user_data);


         memset(strTemp,0,sizeof(strTemp));
         memset(strPressure,0,sizeof(strPressure));
         memset(strHumidity,0,sizeof(strHumidity));
         sprintf((char*)strTemp, "%.01f",fTemperature);
         sprintf((char*)strPressure, "%.00f",fPressure);
         sprintf((char*)strHumidity, "%.01f",fHumidity);

         lv_color_t iColor;
         if(fTemperature <0)  
            iColor = lv_color_hex(0x0f6ac3);
        else if( (fTemperature < 15) && (fTemperature >=0))
            iColor = lv_color_hex(0x03bce4);
        else if( (fTemperature >= 15) && (fTemperature < 25))
            iColor = lv_color_hex(0x0fc372);
        else if( (fTemperature >= 25) && (fTemperature < 30)) 
            iColor = lv_color_hex(0xf4f10f);
        else
            iColor = lv_color_hex(0xaa0000);


        lvgl_port_lock(-1);
       
       // lv_label_set_text(Lbl_temperature, "Temp [°C]: ");
        lv_obj_set_style_text_color(Lbl_temp_value, iColor, 0);
        lv_label_set_text(Lbl_temp_value, strTemp);        

       // lv_label_set_text(Lbl_pressure, "Pressure [hPa]: ");
        lv_label_set_text(Lbl_press_value, strPressure);

       // lv_label_set_text(Lbl_Humidity, "Humidity [%]: ");
        lv_label_set_text(Lbl_humidity_value, strHumidity);

        lv_chart_set_next_value(chart_temp_humy, ser_temp, fTemperature);
        lv_chart_set_next_value(chart_temp_humy, ser_humi, fHumidity);
       
        lv_chart_refresh(chart_temp_humy); /*Required after direct set*/

        lvgl_port_unlock();

         vTaskDelay(pdMS_TO_TICKS(500)); // Delay for 2 seconds
    }
}

int8_t bme280_init()
{
    int8_t iRetVal = 0;
    uint8_t chip_id     = 0;
   // uint8_t reg_address = BME280_ID_REG;

    ESP_LOGI(TAG, "Init BME280 On I2C "); // Log the BMP280 initialization

    if(I2C_Add_Slave_Addr(&bme280Obj.addr,BME280_I2C_ADDRESS) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to add BME280 I2C slave address");
        return -1; // Return error if adding slave address fails
    }

    // Default configuration
	bme280Obj.temperature_oversampling = OVERSAMPLING_x1;
	bme280Obj.pressure_oversampling = OVERSAMPLING_x1;
	bme280Obj.humidity_oversampling = OVERSAMPLING_x1;
	bme280Obj.standby = SB_1000MS;
	bme280Obj.filter = FILTER_OFF;
	bme280Obj.mode = NORMAL;

    // Read the chip ID from the BMP280
    if(I2C_Read_Byte(bme280Obj.addr, BME280_ID_REG, &chip_id, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 chip ID");
        return -1; // Return error if reading chip ID fails
    }
    else
    {
        ESP_LOGI(TAG, " Device ID %d ",chip_id);
        if (chip_id != BME280_CHIP_ID)
        {
            ESP_LOGI(TAG, " Invalid Device ID");
            iRetVal = -1;
        }
        else
        {
            // reset the sens using soft-reset, this makes sure the IIR is off, etc.
            I2C_Write_Byte(bme280Obj.addr, BME280_RESET_REG, BME280_RESET_VALUE, 100);           
            vTaskDelay(300 / 10);

            // if chip is still reading calibration, delay
            uint8_t u8RepeatAgain = 0;
            while ( bme280_isOn(&bme280Obj) && (u8RepeatAgain++ < 10) )
            {
                vTaskDelay(100 / portTICK_PERIOD_MS);
            }
            if(u8RepeatAgain >= 10)
            {
                ESP_LOGE(TAG, "BME280 initialization timeout");
                iRetVal = -1;
            }
            else
            {
                ESP_LOGI(TAG,"BME280 initialization successful");

               // get compensation values
               bme280_Read_Calibration_Data(&bme280Obj);

               ESP_LOGI(TAG,"BME280 Dumping calibration...");
               bme280_Write_Configuration(&bme280Obj);
            }    
            
            // Start the temperature monitoring task
            xTaskCreate(bmp280_task, "bmp280_task", 3 * 1024, NULL, 3, &bme280_TaskHandle);
            if (bme280_TaskHandle == NULL) {
                ESP_LOGE(TAG, "Failed to create bmp280_task");
            }    
        }
    }
    
    return iRetVal;
}

int8_t bme280_Read_Pressure_Temperature_Humidity(bme280_t *ptrConfigObj, uint32_t* pressure, uint32_t* humidity, int32_t* temperature)
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
        if(bme280_Burst_Read(ptrConfigObj, &raw_pressure, &raw_temperature, &raw_humidity) == 0)
        {
            *temperature = bme280_Compensate_Temperature(ptrConfigObj, raw_temperature);
            *pressure    = bme280_Compensate_Presure    (ptrConfigObj, raw_pressure);
            *humidity    = bme280_Compensate_Humidity   (ptrConfigObj, raw_humidity);
        }
        else
        {
            iRetVal = -1;
        }   
    }

    return iRetVal;
}

static bool bme280_isOn(bme280_t *ptrConfigObj)
{
    uint8_t u8Status = 0;
    if(I2C_Read_Byte(ptrConfigObj->addr, BME280_STATUS_REG, &u8Status, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 status");
        return false; // Return error if reading chip ID fails
    }
    return (u8Status & (1 << 0)) != 0;
}

static int bme280_Read_Calibration_Data(bme280_t *ptrConfigObj)
{
    uint8_t u8Data[2];
    
    // Read all other calibration values similarly
    // Note: Ensure to read multi-byte values in little-endian format as required by BME280

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_T1, &ptrConfigObj->dig_T1, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }      
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_T1 %d", ptrConfigObj->dig_T1);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_T2, (uint16_t*)&ptrConfigObj->dig_T2, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_T2 %d", ptrConfigObj->dig_T2);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_T3, (uint16_t*)&ptrConfigObj->dig_T3, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_T3 %d", ptrConfigObj->dig_T3);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P1, &ptrConfigObj->dig_P1, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P1 %d", ptrConfigObj->dig_P1);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P2, (uint16_t*)&ptrConfigObj->dig_P2, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P2 %d", ptrConfigObj->dig_P2);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P3, (uint16_t*)&ptrConfigObj->dig_P3, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P3 %d", ptrConfigObj->dig_P3);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P4, (uint16_t*)&ptrConfigObj->dig_P4, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P4 %d", ptrConfigObj->dig_P4);  

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P5, (uint16_t*)&ptrConfigObj->dig_P5, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P5 %d", ptrConfigObj->dig_P5);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P6, (uint16_t*)&ptrConfigObj->dig_P6, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P6 %d", ptrConfigObj->dig_P6);  

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P7, (uint16_t*)&ptrConfigObj->dig_P7, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P7 %d", ptrConfigObj->dig_P7);  
 
    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P8, (uint16_t*)&ptrConfigObj->dig_P8, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P8 %d", ptrConfigObj->dig_P8);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_P9, (uint16_t*)&ptrConfigObj->dig_P9, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_P9 %d", ptrConfigObj->dig_P9);
    
    if(I2C_Read_Byte  (ptrConfigObj->addr, BME280_REGISTER_DIG_H1, &ptrConfigObj->dig_H1, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_H1 %d", ptrConfigObj->dig_H1);

    if(I2C_Read_WordLe(ptrConfigObj->addr, BME280_REGISTER_DIG_H2, (uint16_t*)&ptrConfigObj->dig_H2, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_H2 %d", ptrConfigObj->dig_H2);
  
    if(I2C_Read_Byte  (ptrConfigObj->addr, BME280_REGISTER_DIG_H3, &ptrConfigObj->dig_H3, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_H3 %d", ptrConfigObj->dig_H3);

    if(I2C_Read_Byte  (ptrConfigObj->addr, BME280_REGISTER_DIG_H4, &u8Data[0], 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_H4 part1 %d", u8Data[0]);

    if(I2C_Read_Byte  (ptrConfigObj->addr, BME280_REGISTER_DIG_H4+1,&u8Data[1], 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ptrConfigObj->dig_H4 = (u8Data[0] << 4) | (u8Data[1] & 0xF);
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_H4 part2 %d", u8Data[1]);

    if(I2C_Read_Byte  (ptrConfigObj->addr, BME280_REGISTER_DIG_H5, &u8Data[0], 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }  
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_H5 part1 %d", u8Data[0]);

    if(I2C_Read_Byte  (ptrConfigObj->addr, BME280_REGISTER_DIG_H5+1,&u8Data[1], 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ptrConfigObj->dig_H5 = (u8Data[0] << 4) | (u8Data[1] >> 4);
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_H5 part2 %d", u8Data[1]);

    if(I2C_Read_Byte  (ptrConfigObj->addr, BME280_REGISTER_DIG_H6, (uint8_t*)&ptrConfigObj->dig_H6, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 calibration data");
        return -1; // Return error if reading chip ID fails
    }
    ESP_LOGI(TAG, "BME280 calibration data read for BME280_REGISTER_DIG_H6 %d", ptrConfigObj->dig_H6);

    return 0;
}
static int bme280_Write_Configuration(const bme280_t *ptrConfigObj)
{
    int iRetVal = 0;
  
    if(I2C_Write_Byte(ptrConfigObj->addr, BME280_CTRL_HUM_REG,(ptrConfigObj->humidity_oversampling & 0x7),100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to write BME280 CTRL_HUM_REG");
        iRetVal = -1; // Return error if reading chip ID fails
    }
    else if(I2C_Write_Byte(ptrConfigObj->addr, BME280_CTRL_MEAS_REG,(ptrConfigObj->temperature_oversampling << 5 | ptrConfigObj->pressure_oversampling << 2 | ptrConfigObj->mode),100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to write BME280 CTRL_MEAS_REG");
        iRetVal = -1; // Return error if reading chip ID fails
    }
    else if(I2C_Write_Byte(ptrConfigObj->addr, BME280_CONFIG_REG,(ptrConfigObj->standby << 5 | ptrConfigObj->filter << 2),100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to write BME280 CONFIG_REG");
        iRetVal = -1; // Return error if reading chip ID fails
    }
    return iRetVal;  
}
// Read pressure, temperature, and humidity in a single burst read
// Note: The order of reading is important as per BME280 datasheet
static int bme280_Burst_Read(const bme280_t *ptrConfigObj, int32_t *raw_pressure, int32_t *raw_temperature, int32_t *raw_humidity)
{
    int iRetVal = 0;
    uint8_t data_buffer[BME280_DATA_LENGTH];
    if(I2C_Read_Bytes(ptrConfigObj->addr, BME280_PRESS_MSB_REG, &data_buffer[0], BME280_DATA_LENGTH, 100) != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to read BME280 data");
        iRetVal = -1; // Return error if reading chip ID fails
    }
    else
    {
        *raw_pressure    = (uint32_t)data_buffer[0] << 12 | (uint16_t)data_buffer[1] << 4 | data_buffer[2] >> 4;
        *raw_temperature = (uint32_t)data_buffer[3] << 12 | (uint16_t)data_buffer[4] << 4 | data_buffer[5] >> 4;
        *raw_humidity    = (uint16_t)data_buffer[6] << 8 | data_buffer[7];
    }
    return iRetVal;
}
    

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC.
// This must be run first before compensate_pressure and compensate_humidity
static int32_t bme280_Compensate_Temperature( bme280_t *ptrConfigObj, int32_t adc_T)
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
static uint32_t bme280_Compensate_Presure(const bme280_t *ptrConfigObj, int32_t adc_P)
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
static uint32_t bme280_Compensate_Humidity(const bme280_t *ptrConfigObj, int32_t adc_H)
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
