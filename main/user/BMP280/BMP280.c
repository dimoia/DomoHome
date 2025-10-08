#include <stdio.h>          // Standard input/output library
#include "esp_log.h"        // ESP32 logging library for debugging and error logging
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl_port.h"    
#include "ui.h"
#include "screens.h"
#include "bmp280.h"  // Include the CAN driver for communication

TaskHandle_t bmp280_TaskHandle;
static const char *TAG = "BMP280";  // Tag used for ESP log output

bmp280_calib_t calib_params;

static i2c_master_dev_handle_t i2c_dev_obj;
#define BMP280_CHIP_ID      0x60


void bmp280UpdateLvgObjectCb(lv_timer_t * timer) 
{
    ESP_LOGI(TAG, "BMP280 bme280UpdateLvgObjectCb");
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
    char strTemp[15];
    char strPressure[15];
    while (1)
    {
        bmp280_data_t bmp280_dt;
        objects_t objs = objects;
        lv_obj_t *Lbl_temperature = objs.lbl_temperature;
        lv_obj_t *Lbl_temp_value  = objs.lbl_temp_value;
        lv_obj_t *Lbl_pressure    = objs.lbl_pressure;
        lv_obj_t *Lbl_press_value = objs.lbl_press_value;

        bmp280_i2c_read_data(&bmp280_dt);
        
        float fTemp = (bmp280_dt.temperature*1.0)/100;
        float fPressure = (((bmp280_dt.pressure)*1.0/100)/256);
        
        ESP_LOGI(TAG,"Pressure %02f Pa Temperature %02f C",fPressure,fTemp);
      

        memset(strTemp,0,sizeof(strTemp));
        memset(strPressure,0,sizeof(strPressure));
        sprintf((char*)strTemp, "%.02f",fTemp);
        sprintf((char*)strPressure, "%.02f",fPressure);

        lv_color_t iColor;
        if(fTemp <0)  
            iColor = lv_color_hex(0x0f6ac3);
        else if( (fTemp < 15) && (fTemp >=0))
            iColor = lv_color_hex(0x03bce4);
        else if( (fTemp >= 15) && (fTemp < 25))
            iColor = lv_color_hex(0x0fc372);
        else if( (fTemp >= 25) && (fTemp < 30)) 
            iColor = lv_color_hex(0xf4f10f);
        else
            iColor = lv_color_hex(0xaa0000);


        lvgl_port_lock(-1);
       
        lv_label_set_text(Lbl_temperature, "Temp [°C]: ");
        lv_obj_set_style_text_color(Lbl_temp_value, iColor, 0);
        lv_label_set_text(Lbl_temp_value, strTemp);        

        lv_label_set_text(Lbl_pressure, "Pressure [hPa]: ");
        lv_label_set_text(Lbl_press_value, strPressure);

        lvgl_port_unlock();

      

        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}


int16_t bmp280_i2c_read_calib(bmp280_calib_t *clb)
{
    uint8_t reg = REG_CALIB;
    uint8_t data[24];
    DEV_I2C_Read_Nbyte(i2c_dev_obj, reg, &data[0], 24);
    clb->dig_t1 = (data[1] << 8)  | data[0];
    clb->dig_t2 = (data[3] << 8)  | data[2];
    clb->dig_t3 = (data[5] << 8)  | data[4];
    clb->dig_p1 = (data[7] << 8)  | data[6];
    clb->dig_p2 = (data[9] << 8)  | data[8];
    clb->dig_p3 = (data[11] << 8) | data[10];
    clb->dig_p4 = (data[13] << 8) | data[12];
    clb->dig_p5 = (data[15] << 8) | data[14];
    clb->dig_p6 = (data[17] << 8) | data[16];
    clb->dig_p7 = (data[19] << 8) | data[18];
    clb->dig_p8 = (data[21] << 8) | data[20];
    clb->dig_p9 = (data[23] << 8) | data[22];
    return 0;
}

int16_t bmp280_i2c_set_calib()
{
    int16_t err = bmp280_i2c_read_calib(&calib_params);
    return err;
}

int16_t bmp280_i2c_read_config(uint8_t *cfg)
{
    uint8_t reg = REG_CONFIG;
    //int16_t err = bmp280_i2c_hal_read(I2C_ADDRESS_BMP280, &reg, cfg, 1);
     DEV_I2C_Read_Nbyte(i2c_dev_obj, reg, cfg, 1);
    return 0;
}

int16_t bmp280_i2c_write_config_filter(bmp280_filter_t fltr)
{
    uint8_t reg = REG_CONFIG;
    uint8_t data[2], cfg;
    bmp280_i2c_read_config(&cfg);
    data[0] = reg;
    data[1] = (cfg & 0xE3) | (fltr << 2);
    //err += bmp280_i2c_hal_write(I2C_ADDRESS_BMP280, data, 2);
    DEV_I2C_Write_Nbyte(i2c_dev_obj, &data[0], 2);
    return 0;
    
}

int16_t bmp280_i2c_write_config_standby_time(bmp280_sb_time_t t_sb)
{
    uint8_t reg = REG_CONFIG;
    uint8_t data[2], cfg;
    bmp280_i2c_read_config(&cfg);
    data[0] = reg;
    data[1] = (cfg & 0x1F) | (t_sb << 5);
    //err += bmp280_i2c_hal_write(I2C_ADDRESS_BMP280, data, 2);
    DEV_I2C_Write_Nbyte(i2c_dev_obj, &data[0], 2);
    return 0;
}

int16_t bmp280_i2c_read_ctrl_meas(uint8_t *cfg)
{
    uint8_t reg = REG_CTRL_MEAS;
   // int16_t err = bmp280_i2c_hal_read(I2C_ADDRESS_BMP280, &reg, cfg, 1);
      DEV_I2C_Read_Nbyte(i2c_dev_obj, reg, cfg, 1);
    return 0;
}

int16_t bmp280_i2c_write_power_mode(bmp280_pwr_mode_t pmode)
{
    uint8_t reg = REG_CTRL_MEAS;
    uint8_t cfg, data[2];
    bmp280_i2c_read_ctrl_meas(&cfg);
    data[0] = reg;
    data[1] = (cfg & 0xFC) | pmode;
    //err += bmp280_i2c_hal_write(I2C_ADDRESS_BMP280, data, 2);
    DEV_I2C_Write_Nbyte(i2c_dev_obj, &data[0], 2);
    return 0;
}

int16_t bmp280_i2c_write_osrs(bmp280_ctrl_meas_t ctrl_meas)
{
    uint8_t reg = REG_CTRL_MEAS;
    uint8_t cfg, data[2];
    bmp280_i2c_read_ctrl_meas(&cfg);
    data[0] = reg;
    data[1] = (cfg & 0x03) | (ctrl_meas.osrs_press << 2) | (ctrl_meas.osrs_tmp << 5);
    //err += bmp280_i2c_hal_write(I2C_ADDRESS_BMP280, data, 2);
    DEV_I2C_Write_Nbyte(i2c_dev_obj, &data[0], 2);
  //  bmp280_i2c_hal_ms_delay(100);
    return 0;
}

int16_t bmp280_i2c_read_status(bmp280_status_t *sts)
{
    uint8_t reg = REG_STATUS;
    uint8_t data;
    //int16_t err = bmp280_i2c_hal_read(I2C_ADDRESS_BMP280, &reg, &data, 1);
      DEV_I2C_Read_Nbyte(i2c_dev_obj, reg, &data, 1);
    sts->measuring = data & (1 << 3);
    sts->im_update = data & (1 << 0);
    return 0;
}

int16_t bmp280_i2c_reset()
{
    uint8_t reg = REG_RESET;
    uint8_t data[2];
    data[0] = reg;
    data[1] = RESET_VAL;
    //int16_t err = bmp280_i2c_hal_write(I2C_ADDRESS_BMP280, data, 2);
    DEV_I2C_Write_Nbyte(i2c_dev_obj, &data[0], 2);
    return 0;
}

int16_t bmp280_i2c_read_pressure_r(int32_t *dt)
{
    uint8_t reg = REG_PRESS_READ;
    uint8_t data[3];
    //int16_t err = bmp280_i2c_hal_read(I2C_ADDRESS_BMP280, &reg, data, 3);
     DEV_I2C_Read_Nbyte(i2c_dev_obj, reg, &data[0], 3);
    *dt = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    return 0;
}

int16_t bmp280_i2c_read_temperature_r(int32_t *dt)
{
    uint8_t reg = REG_TEMP_READ;
    uint8_t data[3];
    //int16_t err = bmp280_i2c_hal_read(I2C_ADDRESS_BMP280, &reg, data, 3);
     DEV_I2C_Read_Nbyte(i2c_dev_obj, reg, &data[0], 3);
    *dt = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    return 0;
} 

int16_t bmp280_i2c_read_data(bmp280_data_t *dt)
{
    int32_t t_fine, press_raw, temp_raw;
    
    bmp280_i2c_read_pressure_r(&press_raw);
    bmp280_i2c_read_temperature_r(&temp_raw);


    int32_t var1_t, var2_t, t;
    int64_t var1_p, var2_p, p;
    var1_t = ((((temp_raw>>3) - ((int32_t)calib_params.dig_t1 <<1))) * ((int32_t)calib_params.dig_t2)) >> 11;
    var2_t = (((((temp_raw>>4) - ((int32_t)calib_params.dig_t1)) * ((temp_raw>>4) - ((int32_t)calib_params.dig_t1))) >> 12) * ((int32_t)calib_params.dig_t3)) >> 14;
    t_fine = var1_t + var2_t;
    t = (t_fine * 5 + 128) >> 8;
    dt->temperature = t;

    var1_p = ((int64_t)t_fine) - 128000;
    var2_p = var1_p * var1_p * (int64_t)calib_params.dig_p6;
    var2_p = var2_p + ((var1_p*(int64_t)calib_params.dig_p5)<<17);
    var2_p = var2_p + (((int64_t)calib_params.dig_p4)<<35);
    var1_p = ((var1_p * var1_p * (int64_t)calib_params.dig_p3)>>8) + ((var1_p * (int64_t)calib_params.dig_p2)<<12);
    var1_p = (((((int64_t)1)<<47)+var1_p))*((int64_t)calib_params.dig_p1)>>33;

    if (var1_p == 0) 
        return -1; // avoid exception caused by division by zero

    p = 1048576 - press_raw;
    p = (((p<<31)-var2_p)*3125)/var1_p;
    var1_p = (((int64_t)calib_params.dig_p9) * (p>>13) * (p>>13)) >> 25;
    var2_p = (((int64_t)calib_params.dig_p8) * p) >> 19;
    p = ((p + var1_p + var2_p) >> 8) + (((int64_t)calib_params.dig_p7)<<4);
    dt->pressure = (uint32_t)p;

    return 0;
} 

int16_t bmp280_i2c_read_part_number(uint8_t *dt)
{
    uint8_t reg = REG_ID_PARTNUMBER;
 //   int16_t err = bmp280_i2c_hal_read(I2C_ADDRESS_BMP280, &reg, dt, 1);
     DEV_I2C_Read_Nbyte(i2c_dev_obj, reg, dt, 1);
    return 0;
} 


int8_t bmp280_init()
{

    int8_t iRetVal = 0;
    uint8_t chip_id     = 0;
   
    ESP_LOGI(TAG, "Init BMP280 On I2C "); // Log the BMP280 initialization

    DEV_I2C_Set_Slave_Addr(&i2c_dev_obj,I2C_ADDRESS_BMP280); // Set the I2C slave address for BMP280
    
    bmp280_i2c_reset();

    bmp280_i2c_read_part_number(&chip_id);
    if (chip_id != BMP280_CHIP_ID)
    {
        iRetVal = -1;
    }
    else
    {
        ESP_LOGI(TAG, "BMP280 Device ID %d ",chip_id);
        vTaskDelay(pdMS_TO_TICKS(10)); // Give the sensor 10 ms delay to reset.

        bmp280_i2c_set_calib();
        bmp280_i2c_write_power_mode(POWERMODE_NORMAL);
        bmp280_i2c_write_config_filter(FILTER_4);

        bmp280_ctrl_meas_t ctrl_meas = {
        .osrs_press = OSRS_x4,
        .osrs_tmp = OSRS_x1,
        };
        bmp280_i2c_write_osrs(ctrl_meas);

         vTaskDelay(pdMS_TO_TICKS(10)); // Give the sensor 10 ms delay to reset.

        
     
    }

    // Start the temperature monitoring task
    xTaskCreate(bmp280_task, "bmp280_task", 3 * 1024, NULL, 3, &bmp280_TaskHandle);
    if (bmp280_TaskHandle == NULL) {
        ESP_LOGE(TAG, "Failed to create bmp280_task");
    }

    return iRetVal;

}