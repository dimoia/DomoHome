#include <string.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c.h"
#include "pcf8523.h"
#include "lvgl_port.h"    
#include "ui.h"
#include "screens.h"

#define I2C_PCF8523_ADDR 0x68 //!< I2C address

#define PCF8523_CONTROL_1			0x00
#define PCF8523_CONTROL_2			0x01
#define PCF8523_CONTROL_3			0x02
#define PCF8523_SECONDS				0x03
#define PCF8523_MINUTES				0x04
#define PCF8523_HOURS				0x05
#define PCF8523_DAYS				0x06
#define PCF8523_WEEKDAYS			0x07
#define PCF8523_MONTHS				0x08
#define PCF8523_YEARS				0x09
#define PCF8523_MINUTE_ALARM		0x0A
#define PCF8523_HOUR_ALARM			0x0B
#define PCF8523_DAY_ALARM			0x0C
#define PCF8523_WEEKDAY_ALARM		0x0D
#define PCF8523_OFFSET				0x0E
#define PCF8523_TMR_CLKOUT_CTRL		0x0F
#define PCF8523_TMR_A_FREQ_CTRL		0x10
#define PCF8523_TMR_A_REG			0x11
#define PCF8523_TMR_B_FREQ_CTRL		0x12
#define PCF8523_TMR_B_REG			0x13

#define PCF8523_CONTROL_1_CAP_SEL_BIT	7
#define PCF8523_CONTROL_1_T_BIT			6
#define PCF8523_CONTROL_1_STOP_BIT		5
#define PCF8523_CONTROL_1_SR_BIT		4
#define PCF8523_CONTROL_1_1224_BIT		3
#define PCF8523_CONTROL_1_SIE_BIT		2
#define PCF8523_CONTROL_1_AIE_BIT		1
#define PCF8523_CONTROL_1CIE_BIT		0

#define PCF8523_CONTROL_2_WTAF_BIT		7
#define PCF8523_CONTROL_2_CTAF_BIT		6
#define PCF8523_CONTROL_2_CTBF_BIT		5
#define PCF8523_CONTROL_2_SF_BIT		4
#define PCF8523_CONTROL_2_AF_BIT 		3
#define PCF8523_CONTROL_2_WTAIE_BIT		2
#define PCF8523_CONTROL_2_CTAIE_BIT		1
#define PCF8523_CONTROL_2_CTBIE_BIT		0

#define PCF8523_SECONDS_OS_BIT			7
#define PCF8523_SECONDS_10_BIT       	6
#define PCF8523_SECONDS_10_LENGTH   	3
#define PCF8523_SECONDS_1_BIT        	3
#define PCF8523_SECONDS_1_LENGTH     	4

#define PCF8523_MINUTES_10_BIT       	6
#define PCF8523_MINUTES_10_LENGTH    	3
#define PCF8523_MINUTES_1_BIT        	3
#define PCF8523_MINUTES_1_LENGTH     	4

#define PCF8523_HOURS_MODE_BIT  	    3 // 0 = 24-hour mode, 1 = 12-hour mode
#define PCF8523_HOURS_AMPM_BIT      	5 // 2nd HOURS_10 bit if in 24-hour mode
#define PCF8523_HOURS_10_BIT        	4
#define PCF8523_HOURS_1_BIT          	3
#define PCF8523_HOURS_1_LENGTH       	4

#define PCF8523_WEEKDAYS_BIT 	        2
#define PCF8523_WEEKDAYS_LENGTH         3

#define PCF8523_DAYS_10_BIT          5
#define PCF8523_DAYS_10_LENGTH       2
#define PCF8523_DAYS_1_BIT           3
#define PCF8523_DAYS_1_LENGTH        4

#define PCF8523_MONTH_10_BIT         4
#define PCF8523_MONTH_1_BIT          3
#define PCF8523_MONTH_1_LENGTH       4

#define PCF8523_YEAR_10H_BIT         7
#define PCF8523_YEAR_10H_LENGTH      4
#define PCF8523_YEAR_1H_BIT          3
#define PCF8523_YEAR_1H_LENGTH       4

static char *strWeekDay[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};
static i2c_master_dev_handle_t i2c_dev_obj;
TaskHandle_t pcf8523_TaskHandle;
static const char *TAG = "PCF8523";  // Tag used for ESP log output


static uint8_t dec2bcd(uint8_t val);
static int Pcf8523_Get_WeekDay(int iYear, int iMouth, int iDay);
static char *Pcf8523_Get_MountName(int iMouth);


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

static lv_obj_t * meter;
/* Global Function */
void pcf8523_task(void *arg)
{  
	char user_data[50];
	struct tm currentTime;
	ESP_LOGI(TAG, "PCF8523 Task Started");
    objects_t objs = objects;
    lv_obj_t *LblDate  = objs.lbl_date;
	lv_obj_t *LblYear  = objs.lbl_year;
	lv_obj_t *LblTime  = objs.lbl_time;
    lv_obj_t *LblPaneltemp  = objs.temp_humi_panel;	
	lv_obj_t *LblPanelTime  = objs.panel_data_time;	
	lv_obj_set_scrollbar_mode(LblPaneltemp, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scrollbar_mode(LblPanelTime, LV_SCROLLBAR_MODE_OFF);
    //lv_obj_t *meter = objs.lbl_time;

	//lv_meter_indicator_t * indic_hour = lv_meter_add_needle_img(meter, scale_min, &img_hand, 5, 5);
	//lv_meter_set_indicator_value(meter, meter, 7);

	meter = lv_meter_create(lv_scr_act());
    lv_obj_set_size(meter, 220, 220);
    lv_obj_set_pos(meter, 226, 300);

    /*Create a scale for the minutes*/
    /*61 ticks in a 360 degrees range (the last and the first line overlaps)*/
    lv_meter_scale_t * scale_min = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale_min, 61, 1, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(meter, scale_min, 0, 60, 360, 270);

    /*Create another scale for the hours. It's only visual and contains only major ticks*/
    lv_meter_scale_t * scale_hour = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale_hour, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY));               /*12 ticks*/
    lv_meter_set_scale_major_ticks(meter, scale_hour, 1, 2, 20, lv_color_black(), 10);    /*Every tick is major*/
    lv_meter_set_scale_range(meter, scale_hour, 1, 12, 330, 300);       /*[1..12] values in an almost full circle*/

    LV_IMG_DECLARE(img_hand)

    /*Add a the hands from images*/
    lv_meter_indicator_t * indic_min = lv_meter_add_needle_img(meter, scale_min, &img_hand, 5, 5);
    lv_meter_indicator_t * indic_hour = lv_meter_add_needle_img(meter, scale_min, &img_hand, 5, 5);
    
    while (1)
    {        
		if(Pcf8523_Get_Time(&currentTime) < 0) 
		{
			ESP_LOGE(TAG, "Failed to get time from PCF8563");
		} else 
		{
			/*
			ESP_LOGI(TAG, "Current Date/Time: %04d-%02d-%02d %02d:%02d:%02d",
					 currentTime.tm_mday, 
					 currentTime.tm_mon, 
					 currentTime.tm_year,
					 currentTime.tm_hour, 
					 currentTime.tm_min, 
					 currentTime.tm_sec);*/
            int iWeekDay = Pcf8523_Get_WeekDay(currentTime.tm_year, currentTime.tm_mon, currentTime.tm_mday);
			
			ESP_LOGI(TAG, "%s %02d %s %04d %02d:%02d:%02d", 
				strWeekDay[iWeekDay],
				currentTime.tm_mday,
				Pcf8523_Get_MountName(currentTime.tm_mon),
				currentTime.tm_year,
				currentTime.tm_hour,
				currentTime.tm_min,
				currentTime.tm_sec);	
            
			memset(user_data,0,sizeof(user_data));					
			sprintf((char*)user_data, "%s %02d %s",			
			  Pcf8523_Get_MountName(currentTime.tm_mon),
			  currentTime.tm_mday,
			  strWeekDay[iWeekDay]);
			
			lvgl_port_lock(-1);
			lv_label_set_text(LblDate, user_data);
			memset(user_data,0,sizeof(user_data));
            sprintf((char*)user_data, "%04d",currentTime.tm_year);
			lv_label_set_text(LblYear, user_data);
			memset(user_data,0,sizeof(user_data));
			sprintf((char*)user_data, "%02d:%02d:%02d",currentTime.tm_hour,currentTime.tm_min,currentTime.tm_sec);
			lv_label_set_text(LblTime, user_data);
			if((currentTime.tm_hour % 12) == 0)
            	lv_meter_set_indicator_value(meter, indic_hour, currentTime.tm_hour*5);
            else
			    lv_meter_set_indicator_value(meter, indic_hour, (currentTime.tm_hour%12)*5);

			lv_meter_set_indicator_value(meter, indic_min, currentTime.tm_min);
			lvgl_port_unlock();
			
      
		}
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 seconds
    }
}

int8_t Pcf8523_Init()
{
  
    int8_t iRetVal = 0;
   
    ESP_LOGI(TAG, "Init PCF8523 On I2C "); // Log the PCF8523 initialization

    DEV_I2C_Set_Slave_Addr(&i2c_dev_obj,I2C_PCF8523_ADDR); // Set the I2C slave address for PCF8523

	// Start the temperature monitoring task
    xTaskCreate(pcf8523_task, "pcf8523_task", 3 * 1024, NULL, 3, &pcf8523_TaskHandle);
    if (pcf8523_TaskHandle == NULL) 
	{
		iRetVal = -1;
        ESP_LOGE(TAG, "Failed to create pcf8523_task");
    }
    return iRetVal;    
}

int8_t Pcf8523_Reset()
{	
	uint8_t u8Data = 0x00;

	ESP_LOGI(TAG, "Reset PCF8523 ");
	if(I2C_Write_Byte(i2c_dev_obj, PCF8523_CONTROL_1,u8Data, 100)!= ESP_OK) 
	{
		ESP_LOGE(TAG, "Failed to reset PCF8523");
		return -1; // Return error if writing time fails
	}		
	return 0;
}

int8_t Pcf8523_Set_Time(struct tm *time)
{
    int8_t iRetVal = 0;
	uint8_t u8Data[7];
	if(time==NULL) {
		iRetVal =  -1;
	}
	else 
	{
		// Set the time time/date data */
		u8Data[0] = dec2bcd(time->tm_sec);
		u8Data[1] = dec2bcd(time->tm_min);
		u8Data[2] = dec2bcd(time->tm_hour);
		u8Data[3] = dec2bcd(time->tm_mday);
		u8Data[4] = dec2bcd(time->tm_wday); // tm_wday is 0 to 6
		u8Data[5] = dec2bcd(time->tm_mon);	// tm_mon is 0 to 11
		u8Data[6] = dec2bcd(time->tm_year - 2000);
		
        if(I2C_Write_Bytes(i2c_dev_obj, PCF8523_SECONDS,u8Data, 7,100) != ESP_OK) 
		{
			ESP_LOGE(TAG, "Failed to set PCF8523 time");
			iRetVal = -1; // Return error if writing time fails
		}		
	}
	return iRetVal;	
}

int8_t Pcf8523_Get_Time(struct tm *out_stTime)
{
    uint8_t data[7];
	int8_t iRetVal = 0;
	if(out_stTime == NULL) 
	{
		iRetVal = -1;
	}
    else 
	{
        if(I2C_Read_Bytes(i2c_dev_obj, PCF8523_SECONDS, &data[0], 7, 100)	!= ESP_OK) 
		{
			ESP_LOGE(TAG, "Failed to read PCF8523 time");
			iRetVal = -1; // Return error if reading time fails
		}

		// Convert raw data into time	
		out_stTime->tm_sec  = (10 * (int)((data[0] & 0x70) >> 4)) + ((int)(data[0] & 0x0F));
		out_stTime->tm_min  = (10 * (int)((data[1] & 0x70) >> 4)) + ((int)(data[1] & 0x0F));
		out_stTime->tm_hour = (10 * (int)((data[2] & 0x30) >> 4)) + ((int)(data[2] & 0x0F));
		out_stTime->tm_mday = (10 * (int)((data[3] & 0x30) >> 4)) + ((int)(data[3] & 0x0F));
		out_stTime->tm_wday = (int)(data[4] & 0x07);
		out_stTime->tm_mon  = (10 * (int)((data[5] & 0x10) >> 4)) + ((int)(data[5] & 0x0F));
		out_stTime->tm_year = (10 * (int)((data[6] & 0xF0) >> 4)) + ((int)(data[6] & 0x0F)) + 2000;	

		//int iWeekDay = Pcf8523_Get_WeekDay(out_stTime->tm_year, out_stTime->tm_mon, out_stTime->tm_mday);
/*
		sprintf(out_StrDateTime, "%02d-%02d-%04d %02d:%02d:%02d",
				out_stTime->tm_mday,
				out_stTime->tm_mon,
				out_stTime->tm_year,				
				out_stTime->tm_hour,
				out_stTime->tm_min,
				out_stTime->tm_sec);*/
	}
	return iRetVal;
}

/* Local Function */
static uint8_t dec2bcd(uint8_t val)
{
	return ((val / 10) << 4) + (val % 10);
}

static int Pcf8523_Get_WeekDay(int iYear, int iMouth, int iDay)
{
  //formula to get weekday number
  int rst = iDay
      + ((153 * (iMouth + 12 * ((14 - iMouth) / 12) - 3) + 2) / 5)
      + (365 * (iYear + 4800 - ((14 - iMouth) / 12)))
      + ((iYear + 4800 - ((14 - iMouth) / 12)) / 4)
      - ((iYear + 4800 - ((14 - iMouth) / 12)) / 100)
      + ((iYear + 4800 - ((14 - iMouth) / 12)) / 400)
      - 32045;

  return (rst + 1) % 7;
}
static char* Pcf8523_Get_MountName(int iMouth)
{
	switch(iMouth) {
		case 1:
			return "January";
		case 2:
			return "February";
		case 3:
			return "March";
		case 4:
			return "April";
		case 5:
			return "May";
		case 6:
			return "June";
		case 7:
			return "July";
		case 8:
			return "August";
		case 9:
			return "September";
		case 10:
			return "October";
		case 11:
			return "November";
		case 12:
			return "December";        
		default:
			return "Unknown";
	}
}