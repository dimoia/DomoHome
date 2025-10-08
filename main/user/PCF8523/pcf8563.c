#include <string.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c.h"
#include "pcf8563.h"
#include "lvgl_port.h"    
#include "ui.h"
#include "screens.h"

#define I2C_PCF8563_ADDR 0x68 //!< I2C address

#define PCF8563_ADDR_STATUS1 0x00
#define PCF8563_ADDR_STATUS2 0x01
#define PCF8563_ADDR_TIME    0x02
#define PCF8563_ADDR_ALARM   0x09
#define PCF8563_ADDR_CONTROL 0x0d
#define PCF8563_ADDR_TIMER   0x0e

static i2c_master_dev_handle_t i2c_dev_obj;
static uint8_t bcd2dec(uint8_t val);
static uint8_t dec2bcd(uint8_t val);

TaskHandle_t pcf8523_TaskHandle;
static const char *TAG = "PCF8523";  // Tag used for ESP log output


void pcf8523UpdateLvgObjectCb(lv_timer_t * timer) 
{
    ESP_LOGI(TAG, "pcf8523 pcf8523UpdateLvgObjectCb");
    #if 0
    if (CAN_Clear) {
        memset(can_data, 0, sizeof(can_data));  // Clear the buffer when flag is set
        CAN_Clear = false;
    }
    lv_textarea_set_text(ui_CAN_Read_Area, can_data);  // Update the UI with the new CAN data
    #endif
}

void pcf8523_task(void *arg)
{  
    char strTemp[15];
    char strPressure[15];
    while (1)
    {
        struct tm currentTime;
		
		if(pcf8563_get_time(&currentTime) < 0) {
			ESP_LOGE(TAG, "Failed to get time from PCF8563");
		} else {
			ESP_LOGI(TAG, "Current Time: %04d-%02d-%02d %02d:%02d:%02d",
					 currentTime.tm_year, currentTime.tm_mon, currentTime.tm_mday,
					 currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);
		}	
      

        vTaskDelay(pdMS_TO_TICKS(2000)); // Delay for 2 seconds
    }
}

static uint8_t bcd2dec(uint8_t val)
{
	return (val >> 4) * 10 + (val & 0x0f);
}

static uint8_t dec2bcd(uint8_t val)
{
	return ((val / 10) << 4) + (val % 10);
}


void pcf8563_init()
{

    int8_t iRetVal = 0;
    uint8_t chip_id     = 0;
   
    ESP_LOGI(TAG, "Init PCF8563 On I2C "); // Log the PCF8563 initialization

    DEV_I2C_Set_Slave_Addr(&i2c_dev_obj,I2C_PCF8563_ADDR); // Set the I2C slave address for PCF8563

	// Start the temperature monitoring task
    xTaskCreate(pcf8523_task, "pcf8523_task", 3 * 1024, NULL, 3, &pcf8523_TaskHandle);
    if (pcf8523_TaskHandle == NULL) {
        ESP_LOGE(TAG, "Failed to create pcf8523_task");
    }
    
}

void pcf8563_reset()
{	
	uint8_t u8Data[3];
	u8Data[0] = PCF8563_ADDR_STATUS1;
	u8Data[1] = 0x00;
	u8Data[2] = 0x00;
	
	DEV_I2C_Write_Nbyte(i2c_dev_obj, &u8Data[0], 3);
	//return i2c_dev_write_reg(dev, PCF8563_ADDR_STATUS1, data, 2);
}

int8_t pcf8563_set_time(struct tm *time)
{
    int8_t iRetVal = 0;
	uint8_t u8Data[8];
	if(time==NULL) {
		iRetVal =  -1;
	}
	else 
	{
		// Set the time time/date data */
		u8Data[0]= PCF8563_ADDR_TIME;
		u8Data[0] = dec2bcd(time->tm_sec);
		u8Data[1] = dec2bcd(time->tm_min);
		u8Data[2] = dec2bcd(time->tm_hour);
		u8Data[3] = dec2bcd(time->tm_mday);
		u8Data[4] = dec2bcd(time->tm_wday);		// tm_wday is 0 to 6
		u8Data[5] = dec2bcd(time->tm_mon + 1);	// tm_mon is 0 to 11
		u8Data[6] = dec2bcd(time->tm_year - 2000);

		DEV_I2C_Write_Nbyte(i2c_dev_obj, &u8Data[0], 8);
		//return i2c_dev_write_reg(dev, PCF8563_ADDR_TIME, data, 8);
	}
	return iRetVal;	
}

int8_t pcf8563_get_time(struct tm *time)
{
    uint8_t data[7];
	int8_t iRetVal = 0;
	if(time == NULL) {
		iRetVal = -1;
	}
    else 
	{
		DEV_I2C_Read_Nbyte(i2c_dev_obj, PCF8563_ADDR_TIME, &data[0], 7);

		/* read time */
		//esp_err_t res = i2c_dev_read_reg(dev, PCF8563_ADDR_TIME, data, 7);
		//	if (res != ESP_OK) return res;

		/* convert to unix time structure */
		ESP_LOGD("", "data=%02x %02x %02x %02x %02x %02x %02x",
					data[0],data[1],data[2],data[3],data[4],data[5],data[6]);

		time->tm_sec  = bcd2dec(data[0] & 0x7F);
		time->tm_min  = bcd2dec(data[1] & 0x7F);
		time->tm_hour = bcd2dec(data[2] & 0x3F);
		time->tm_mday = bcd2dec(data[3] & 0x3F);
		time->tm_wday = bcd2dec(data[4] & 0x07);		// tm_wday is 0 to 6
		time->tm_mon  = bcd2dec(data[5] & 0x1F) - 1;	// tm_mon is 0 to 11
		time->tm_year = bcd2dec(data[6]) + 2000;
		time->tm_isdst = 0;
	}
	return iRetVal;
}


