#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c.h"
#include "aht10.h"
#include "lvgl_port.h"    
#include "ui.h"
#include "screens.h"
#include "aht.h"


#define AHT10_I2CADDR_DEFAULT 0x38//0x38   ///< AHT default i2c address
#define AHT10_I2CADDR_ALTERNATE 0x39 ///< AHT alternate i2c address

#define AHT10_CMD_CALIBRATE 0xE1     ///< Calibration command
#define AHT10_CMD_TRIGGER 0xAC       ///< Trigger reading command
#define AHT10_CMD_SOFTRESET 0xBA     ///< Soft reset command
#define AHT10_STATUS_BUSY 0x80       ///< Status bit for busy
#define AHT10_STATUS_CALIBRATED 0x08 ///< Status bit for calibrated

static i2c_master_dev_handle_t i2c_dev_obj; 
TaskHandle_t aht10_TaskHandle;
static const char *TAG = "AHT10";  // Tag used for ESP log output

void aht10UpdateLvgObjectCb(lv_timer_t * timer) 
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

void aht10_task(void *arg)
{     
     aht_t dev = { 0 };
    dev.mode = AHT_MODE_NORMAL;
    dev.type = AHT_TYPE_AHT1x;

    ESP_LOGI(TAG, "aht10_task Start "); // Log the PCF8563 initialization

    

    ESP_ERROR_CHECK(aht_init_desc(&dev, AHT10_I2CADDR_DEFAULT, 0, I2C_MASTER_SDA, I2C_MASTER_SCL));
    ESP_LOGI(TAG, "aht_init_desc Start "); // Log the PCF8563 initialization

    ESP_ERROR_CHECK(aht_init(&dev));
    ESP_LOGI(TAG, "aht_init Start "); // Log the PCF8563 initialization

     bool calibrated;
    ESP_ERROR_CHECK(aht_get_status(&dev, NULL, &calibrated));
      if (calibrated)
        ESP_LOGI(TAG, "Sensor calibrated");
    else
        ESP_LOGW(TAG, "Sensor not calibrated!");


    float fTemperature;
    float fHumidity;
    while (1)
    {
        esp_err_t res = aht_get_data(&dev, &fTemperature, &fHumidity);
        if (res == ESP_OK)
            ESP_LOGI(TAG, "Temperature: %.1f°C, Humidity: %.2f%%", fTemperature, fHumidity);
        else
            ESP_LOGE(TAG, "Error reading data: %d (%s)", res, esp_err_to_name(res));

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}




static void aht10_reset(void) 
{
  /* Two byte reset. First byte register, second byte data */
  uint8_t u8Data[] = { 0x00, 0x58 };
  DEV_I2C_Write_Nbyte(i2c_dev_obj, &u8Data[0], 2);
}

static void aht10_softReset(void) 
{
  DEV_I2C_Write_Byte(i2c_dev_obj, AHT10_CMD_SOFTRESET, 1);
}


static uint8_t aht10_getStatus(void) 
{
  uint8_t u8Ret;
  u8Ret = DEV_I2C_Read_Byte(i2c_dev_obj);
  return u8Ret;
}

static int8_t aht10_writeData(const uint8_t* in_ptr_u8ToDataToBeWrite, uint8_t in_uiNumberOfByteToWrite)
{
  int8_t iRetVal = 0;   
  uint8_t u8Cmd[3];
  
  u8Cmd[0] = AHT10_CMD_SOFTRESET;
  DEV_I2C_Write_Nbyte(i2c_dev_obj, &u8Cmd[0], 1);

  vTaskDelay(pdMS_TO_TICKS(20)); // Wait for 20 ms after reset
  while (aht10_getStatus() & AHT10_STATUS_BUSY)
  {
    vTaskDelay(pdMS_TO_TICKS(20)); // Wait for 20 ms after reset
  }
  
  u8Cmd[0] = AHT10_CMD_CALIBRATE;
  u8Cmd[1] = 0x08;
  u8Cmd[2] = 0x00;

  DEV_I2C_Write_Nbyte(i2c_dev_obj, &u8Cmd[0], 3);
  vTaskDelay(pdMS_TO_TICKS(20)); // Wait for 20 ms after reset
  while (aht10_getStatus() & AHT10_STATUS_BUSY)
  {
    vTaskDelay(pdMS_TO_TICKS(20)); // Wait for 20 ms after reset
  }
  if (!(aht10_getStatus() & AHT10_STATUS_CALIBRATED)) 
  {
    iRetVal = -1;        
  }              
  return iRetVal;
}


static int8_t aht10_readData(uint8_t* out_ptr_u8ToDataToBeWrite, uint8_t in_uiNumberOfByteToRead)
{
  int8_t iRetVal = 0;  
  uint8_t u8Cmd[3] = { AHT10_CMD_TRIGGER, 0x33, 0 };
  //uint8_t u8Data[6];

  if (out_ptr_u8ToDataToBeWrite == NULL)
    iRetVal = -1;
  else
  {     
    DEV_I2C_Write_Nbyte(i2c_dev_obj, &u8Cmd[0], 3);   
    vTaskDelay(pdMS_TO_TICKS(20)); // Wait for 20 ms after reset    
    while (aht10_getStatus() & AHT10_STATUS_BUSY) 
    {
        vTaskDelay(pdMS_TO_TICKS(20)); // Wait for 20 ms after reset
    }
    DEV_I2C_Read_Bytes(i2c_dev_obj,out_ptr_u8ToDataToBeWrite,in_uiNumberOfByteToRead);
  //  memcpy(out_ptr_u8ToDataToBeWrite, data, in_uiNumberOfByteToRead);    
  }
  return iRetVal;
   
}

int8_t aht10_init()
{
  ESP_LOGI(TAG, "Init AHT10 On I2C "); // Log the PCF8563 initialization

  #if 0
  DEV_I2C_Set_Slave_Addr(&i2c_dev_obj,AHT10_I2CADDR_DEFAULT); // Set the I2C slave address for PCF8563
 
  
  //aht10_reset();
  vTaskDelay(pdMS_TO_TICKS(20)); // Wait for 20 ms after reset
  if(aht10_writeData(NULL,0) < 0) 
  {
      ESP_LOGE(TAG, "AHT10 Calibration Failed");
      return -1;
  }
  ESP_LOGI(TAG, "AHT10 Calibration Done "); // Log the PCF8563 initialization
	// Start the temperature monitoring task
#endif
  ESP_ERROR_CHECK(i2cdev_init());
  xTaskCreate(aht10_task, "ath10_task", 3 * 1024, NULL, 3, &aht10_TaskHandle);
  if (aht10_TaskHandle == NULL) {
      ESP_LOGE(TAG, "Failed to create ath10_task");
  }
    
  return 0; 
}

int8_t aht10_getCurrentTempAndHumidity(float *out_ptrTemperature, float *out_ptrHumidity)
{
  int iRetVal = 0;
  float fTemperatureC;
  float fHumidity;
  //aht10_softReset();
  aht10_writeData(NULL,0);
  ESP_LOGI(TAG, "aht10_softReset executed "); // Log the aht10 initialization
  uint8_t data[6];
  aht10_readData(data, 6);
  ESP_LOGI(TAG, "aht10_readData executed "); // Log the aht10 initialization
  uint32_t h = data[1];
  h <<= 8;
  h |= data[2];
  h <<= 4;
  h |= data[3] >> 4;
  fHumidity = ((float)h * 100) / 0x100000;  
      
  uint32_t tdata = data[3] & 0x0F;
  tdata <<= 8;
  tdata |= data[4];
  tdata <<= 8;
  tdata |= data[5];
  fTemperatureC = ((float)tdata * 200 / 0x100000) - 50;  
  
  *out_ptrTemperature = fTemperatureC;
  *out_ptrHumidity = fHumidity;
  
  return iRetVal;
}
