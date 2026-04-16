#include "heating_config.h"
#include "esp_spiffs.h"
#include "esp_log.h"   // Include ESP32 logging functions
#include "lvgl_port.h" 
#include "lvgl.h"
#include "ui.h"
#include "screens.h"

#define HEATING_CONFIG_ARRAY_SIZE 7

static const char *TAG = "HEATING_CONFIG";
static FILE       *ptrToFILE;
static HEATING_CONFIG heatingConfigArray[HEATING_CONFIG_ARRAY_SIZE];
static HEATING_CONFIG heatingConfigArrayTmp[HEATING_CONFIG_ARRAY_SIZE];

typedef struct _HEATING_LABEL_TEMP_
{
  lv_obj_t *lblTempMin;
  uint16_t  u16XposLblTempMin;
  uint16_t  u16YposLblTempMin;  
  lv_obj_t *lblTempMax;
  uint16_t  u16XposLblTempMax;
  uint16_t  u16YposLblTempMax;  
} HEATING_LABEL_TEMP;
HEATING_LABEL_TEMP lblTempArray[HEATING_CONFIG_ARRAY_SIZE];

typedef struct _HEATING_LABEL_TIME_
{
  lv_obj_t *lblTimeMin;
  uint16_t  u16XposLblTimeMin;
  uint16_t  u16YposLblTimeMin;  
  lv_obj_t *lblTimeMax;
  uint16_t  u16XposLblTimeMax;
  uint16_t  u16YposLblTimeMax;
} HEATING_LABEL_TIME;
HEATING_LABEL_TIME lblTimeArray[HEATING_CONFIG_ARRAY_SIZE];

static void CloseFile(void)
{
    fclose(ptrToFILE);
}

static int8_t OpenFile(char* chMode)
{    
    int8_t iRet = 0;
    ptrToFILE = fopen("/spiffs/config.cfg", chMode);
    if (ptrToFILE == NULL) 
    {
        iRet = -1;          
    }
    return iRet;
}

static int8_t WriteConfig(const HEATING_CONFIG *in_ptrHeatingConfig)
{
    int8_t iRet = 0;
    size_t scritti = fwrite(in_ptrHeatingConfig, sizeof(HEATING_CONFIG), HEATING_CONFIG_ARRAY_SIZE, ptrToFILE);
    return iRet;
}

static int8_t ReadConfig(HEATING_CONFIG *in_ptrHeatingConfig)
{
    int8_t iRet = 0;
    fread(in_ptrHeatingConfig, sizeof(HEATING_CONFIG),HEATING_CONFIG_ARRAY_SIZE, ptrToFILE);
    return iRet;
}

void action_heating_screen_config(lv_event_t *e) 
{
    objects_t objs = objects;    

    // Download Configuration from spifs
    memset(heatingConfigArray,0,sizeof(heatingConfigArray));

    for(int i = 0; i < HEATING_CONFIG_ARRAY_SIZE;i++)
    {
        lv_obj_set_pos(lblTempArray[i].lblTempMin, lblTempArray[i].u16XposLblTempMin , lblTempArray[i].u16YposLblTempMin);
        lv_obj_set_pos(lblTempArray[i].lblTempMax, lblTempArray[i].u16XposLblTempMax , lblTempArray[i].u16YposLblTempMax);

        lv_obj_set_pos(lblTimeArray[i].lblTimeMin, lblTimeArray[i].u16XposLblTimeMin , lblTimeArray[i].u16YposLblTimeMin);
        lv_obj_set_pos(lblTimeArray[i].lblTimeMax, lblTimeArray[i].u16XposLblTimeMax , lblTimeArray[i].u16YposLblTimeMax);
    }


    if(OpenFile("r") != -1)
    {
        if(ReadConfig(heatingConfigArray) != -1)
        {
            CloseFile();
            
            ESP_LOGI(TAG, "###############################################################");
            for(int i = 0; i < HEATING_CONFIG_ARRAY_SIZE;i++)
            {
                switch(heatingConfigArray[0].enConfigType)
                {
                    case OFF:
                    ESP_LOGI(TAG, "Heating Setting: OFF");
                    break;

                    case TEMPERATURE:
                    ESP_LOGI(TAG, "Heating Setting: TEMPERATURE");
                    ESP_LOGI(TAG, "Day: %d Temp Min: %d Temp Max: %d", (i+1),heatingConfigArray[i].stTempRange.u8TempMin,heatingConfigArray[i].stTempRange.u8TempMax);
                    break;

                    case TIMER:
                    ESP_LOGI(TAG, "Heating Setting: TIMER");
                    ESP_LOGI(TAG, "Day: %d Temp Min: %d Temp Max: %d", (i+1),heatingConfigArray[i].stTimerRange.u8ClockMin,heatingConfigArray[i].stTimerRange.u8ClockMax);
                    break;

                    default:
                    break;
                }
            }    
            ESP_LOGI(TAG, "###############################################################");
            
            lv_dropdown_set_selected(objs.drop_day_monday, (uint16_t)heatingConfigArray[0].enConfigType);
            // monday
            if(heatingConfigArray[0].enConfigType == OFF)
            {
                lv_obj_add_state(objs.slider_temp_monday,     LV_STATE_DISABLED);
                lv_obj_add_state(objs.slider_timer_monday,    LV_STATE_DISABLED);
            }
            else  
            if(heatingConfigArray[0].enConfigType == TEMPERATURE)
            {
                lv_obj_add_state(objs.slider_timer_monday,LV_STATE_DISABLED);
                lv_slider_set_range(objs.slider_temp_monday, heatingConfigArray[0].stTempRange.u8TempMin, heatingConfigArray[0].stTempRange.u8TempMax);
            }
            else
            {
                lv_obj_add_state(objs.slider_temp_monday,LV_STATE_DISABLED);
                lv_slider_set_range(objs.slider_temp_monday, heatingConfigArray[0].stTimerRange.u8ClockMin, heatingConfigArray[0].stTimerRange.u8ClockMax);
            }
            // tuesday
            lv_dropdown_set_selected(objs.drop_day_monday, (uint16_t)heatingConfigArray[1].enConfigType);
            if(heatingConfigArray[1].enConfigType == OFF)
            {
                lv_obj_add_state(objs.slider_temp_tuesday,    LV_STATE_DISABLED);
                lv_obj_add_state(objs.slider_timer_tuesday,   LV_STATE_DISABLED);
            }
            else  
            if(heatingConfigArray[1].enConfigType == TEMPERATURE)
            {
                lv_obj_add_state(objs.slider_timer_tuesday,LV_STATE_DISABLED);
                lv_slider_set_range(objs.slider_temp_tuesday, heatingConfigArray[1].stTempRange.u8TempMin, heatingConfigArray[1].stTempRange.u8TempMax);
            }
            else
            {
                lv_obj_add_state(objs.slider_temp_tuesday,LV_STATE_DISABLED);   
                lv_slider_set_range(objs.slider_temp_tuesday, heatingConfigArray[1].stTimerRange.u8ClockMin, heatingConfigArray[1].stTimerRange.u8ClockMax);     
            }

            // wednesday
            lv_dropdown_set_selected(objs.drop_day_wednesday, (uint16_t)heatingConfigArray[2].enConfigType);
            if(heatingConfigArray[2].enConfigType == OFF)
            {
                lv_obj_add_state(objs.slider_temp_wednesday,  LV_STATE_DISABLED);
                lv_obj_add_state(objs.slider_timer_wednesday, LV_STATE_DISABLED);
            }
            else  
            if(heatingConfigArray[2].enConfigType == TEMPERATURE)
            {
                lv_obj_add_state(objs.slider_timer_wednesday,LV_STATE_DISABLED);
                lv_slider_set_range(objs.slider_temp_wednesday, heatingConfigArray[2].stTempRange.u8TempMin, heatingConfigArray[2].stTempRange.u8TempMax);
            }
            else
            {
                lv_obj_add_state(objs.slider_temp_wednesday,LV_STATE_DISABLED);   
                lv_slider_set_range(objs.slider_temp_wednesday, heatingConfigArray[2].stTimerRange.u8ClockMin, heatingConfigArray[2].stTimerRange.u8ClockMax);           
            }

            // thursday   
            lv_dropdown_set_selected(objs.drop_day_thursday, (uint16_t)heatingConfigArray[3].enConfigType);
            if(heatingConfigArray[2].enConfigType == OFF)
            {
                lv_obj_add_state(objs.slider_temp_thursday,   LV_STATE_DISABLED);
                lv_obj_add_state(objs.slider_timer_thursday,  LV_STATE_DISABLED);
            }
            else  
            if(heatingConfigArray[2].enConfigType == TEMPERATURE)
            {
                lv_obj_add_state(objs.slider_timer_thursday,LV_STATE_DISABLED);
                lv_slider_set_range(objs.slider_temp_thursday, heatingConfigArray[3].stTempRange.u8TempMin, heatingConfigArray[3].stTempRange.u8TempMax);
            }
            else
            {
                lv_obj_add_state(objs.slider_temp_thursday,LV_STATE_DISABLED);  
                lv_slider_set_range(objs.slider_temp_thursday, heatingConfigArray[3].stTimerRange.u8ClockMin, heatingConfigArray[3].stTimerRange.u8ClockMax);           
            }
            // friday
            lv_dropdown_set_selected(objs.drop_day_friday, (uint16_t)heatingConfigArray[4].enConfigType);
            if(heatingConfigArray[2].enConfigType == OFF)
            {
                lv_obj_add_state(objs.slider_temp_friday,     LV_STATE_DISABLED);
                lv_obj_add_state(objs.slider_timer_friday,    LV_STATE_DISABLED); 
            }
            else  
            if(heatingConfigArray[2].enConfigType == TEMPERATURE)
            {
                lv_obj_add_state(objs.slider_timer_friday,LV_STATE_DISABLED);
                lv_slider_set_range(objs.slider_temp_friday, heatingConfigArray[4].stTempRange.u8TempMin, heatingConfigArray[4].stTempRange.u8TempMax);
            }
            else
            {
                lv_obj_add_state(objs.slider_temp_friday,LV_STATE_DISABLED); 
                lv_slider_set_range(objs.slider_temp_friday, heatingConfigArray[4].stTimerRange.u8ClockMin, heatingConfigArray[4].stTimerRange.u8ClockMax);           
            }

            // saturday
            lv_dropdown_set_selected(objs.drop_day_saturday, (uint16_t)heatingConfigArray[5].enConfigType);
            if(heatingConfigArray[2].enConfigType == OFF)
            {
                lv_obj_add_state(objs.slider_temp_saturday,   LV_STATE_DISABLED);
                lv_obj_add_state(objs.slider_timer_saturday,  LV_STATE_DISABLED);
            }
            else  
            if(heatingConfigArray[2].enConfigType == TEMPERATURE)
            {
                lv_obj_add_state(objs.slider_timer_saturday,LV_STATE_DISABLED);
                lv_slider_set_range(objs.slider_temp_saturday, heatingConfigArray[5].stTempRange.u8TempMin, heatingConfigArray[5].stTempRange.u8TempMax);
            }
            else
            {
                lv_obj_add_state(objs.slider_temp_saturday,LV_STATE_DISABLED);   
                lv_slider_set_range(objs.slider_temp_saturday, heatingConfigArray[5].stTimerRange.u8ClockMin, heatingConfigArray[5].stTimerRange.u8ClockMax);
            }

            // sunday
            lv_dropdown_set_selected(objs.drop_day_sunday, (uint16_t)heatingConfigArray[6].enConfigType);
            if(heatingConfigArray[2].enConfigType == OFF)
            {
                lv_obj_add_state(objs.slider_temp_sunday,     LV_STATE_DISABLED);
                lv_obj_add_state(objs.slider_timer_sunday,    LV_STATE_DISABLED);
            }
            else  
            if(heatingConfigArray[2].enConfigType == TEMPERATURE)
            {
                lv_obj_add_state(objs.slider_timer_sunday,LV_STATE_DISABLED);
                lv_slider_set_range(objs.slider_temp_sunday, heatingConfigArray[6].stTempRange.u8TempMin, heatingConfigArray[6].stTempRange.u8TempMax);
            }
            else
            {
                lv_obj_add_state(objs.slider_temp_sunday,LV_STATE_DISABLED); 
                lv_slider_set_range(objs.slider_temp_sunday, heatingConfigArray[6].stTimerRange.u8ClockMin, heatingConfigArray[6].stTimerRange.u8ClockMax);
            }
        }
        else
        {
            ESP_LOGE(TAG, "Error To Read Config File");
        }
    }
    else
    {
        ESP_LOGE(TAG, "Error To Open File");
    }

#if 0
    switch(heatingConfigArray[0].enConfigType)
    {
        case OFF:
        // Disabilita tutte le due sliders
        lv_obj_add_state(objs.slider_temp_monday,     LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_monday,    LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_tuesday,    LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_tuesday,   LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_wednesday,  LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_wednesday, LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_thursday,   LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_thursday,  LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_friday,     LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_friday,    LV_STATE_DISABLED);        
        lv_obj_add_state(objs.slider_temp_saturday,   LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_saturday,  LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_sunday,     LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_sunday,    LV_STATE_DISABLED);

        lv_obj_t *ta = objs.drop_day_monday;
        lv_dropdown_clear_options(ta);
        lv_dropdown_add_option(ta, heatingConfigArray[0].enConfigType, 0); // 0 è l'indice (posizione)
        ESP_LOGI(TAG_CONFIG, "WifiSsid: %s", stUSerConfig.strWifiSsid);

        break;

        case TEMPERATURE:
        // Disabilita la slider Timer
        lv_obj_add_state(objs.slider_timer_monday,    LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_tuesday,   LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_wednesday, LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_thursday,  LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_friday,    LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_saturday,  LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_timer_sunday,    LV_STATE_DISABLED);
        
        break;

        case TIMER:
        // Disabilita la slider Temperature
        lv_obj_add_state(objs.slider_temp_monday,    LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_tuesday,   LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_wednesday, LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_thursday,  LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_friday,    LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_saturday,  LV_STATE_DISABLED);
        lv_obj_add_state(objs.slider_temp_sunday,    LV_STATE_DISABLED);        
        break;

        default:
        break;        
    }
#endif 

#if 0
    // pupulate combobox
    objs.drop_day_monday = heatingConfigArray[0].enConfigType
    drop_day_tuesday
    drop_day_wednesday
    drop_day_thursday
    drop_day_friday
    drop_day_saturday
    drop_day_sunday

    if (strlen(stUSerConfig.strHostname) > 0)
    {
        lv_obj_t *ta = objs.txt_hostname;
        lv_textarea_set_text(ta, stUSerConfig.strHostname);
    }
    // Get Wi-Fi SSID
    if (strlen(stUSerConfig.strWifiSsid) > 0)
    {
        lv_obj_t *ta = objs.drp_wifi_ssid;
        lv_dropdown_clear_options(ta);
        lv_dropdown_add_option(ta, stUSerConfig.strWifiSsid, 0); // 0 è l'indice (posizione)
        ESP_LOGI(TAG_CONFIG, "WifiSsid: %s", stUSerConfig.strWifiSsid);
    }
#endif    
}

void action_heating_sw_init(lv_event_t *e) 
{
    uint8_t userData     = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj        = lv_event_get_target(e);    
    char strBuffer[16];

    if(code == LV_EVENT_SCREEN_LOADED) 
    {
        switch(userData)
        {
            case 1:
                ESP_LOGI(TAG, "Type Selected");                             
                break;
        }
    }
}

void action_heating_slider_temperature(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * slider    = lv_event_get_target(e);
    uint8_t userData     = (uint32_t)(uintptr_t)lv_event_get_user_data(e);

    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        heatingConfigArray[userData-1].stTempRange.u8TempMin = lv_slider_get_value(slider);
        lv_label_set_text_fmt(lblTempArray[userData-1].lblTempMin, "%d", heatingConfigArray[userData-1].stTempRange.u8TempMin);

        heatingConfigArray[userData-1].stTempRange.u8TempMax = lv_slider_get_value(slider);
        lv_label_set_text_fmt(lblTempArray[userData-1].lblTempMax, "%d", heatingConfigArray[userData-1].stTempRange.u8TempMax);

        #if 0
        switch(userData)
        {
            case 1:
             heatingConfigArray[0].stTempRange.u8TempMin = lv_slider_get_value(slider);
             heatingConfigArray[0].stTempRange.u8TempMax = lv_slider_get_value(slider);
            break;

            case 2:
             heatingConfigArray[1].stTempRange.u8TempMin = lv_slider_get_value(slider);
             heatingConfigArray[1].stTempRange.u8TempMax = lv_slider_get_value(slider);
            break;

            default:
            break;            
        }    
            #endif  
    }
}


void action_heating_slider_timer(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * slider    = lv_event_get_target(e);
    uint8_t userData     = (uint32_t)(uintptr_t)lv_event_get_user_data(e);

    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        heatingConfigArray[userData-1].stTimerRange.u8ClockMin = lv_slider_get_value(slider);
        lv_label_set_text_fmt(lblTimeArray[userData-1].lblTimeMin, "%d", heatingConfigArray[userData-1].stTimerRange.u8ClockMin);

        heatingConfigArray[userData-1].stTimerRange.u8ClockMax = lv_slider_get_value(slider);
        lv_label_set_text_fmt(lblTimeArray[userData-1].lblTimeMax, "%d", heatingConfigArray[userData-1].stTimerRange.u8ClockMax);

        #if 0
        switch(userData)
        {
            case 1:
             heatingConfigArray[0].stTempRange.u8TempMin = lv_slider_get_value(slider);
             heatingConfigArray[0].stTempRange.u8TempMax = lv_slider_get_value(slider);
            break;

            case 2:
             heatingConfigArray[1].stTempRange.u8TempMin = lv_slider_get_value(slider);
             heatingConfigArray[1].stTempRange.u8TempMax = lv_slider_get_value(slider);
            break;

            default:
            break;            
        }    
            #endif  
    }
    
}

void action_heating_sw(lv_event_t *e)
{
    // Called when user change the value in combo box
    char strBuffer[16];
    objects_t objs       = objects; 
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj        = lv_event_get_target(e);    
    uint8_t userData     = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    uint16_t index;

    if(code == LV_EVENT_VALUE_CHANGED) 
    { 
        switch(userData)
        {
            case 1:
                ESP_LOGI(TAG, "Monday Selected "); 
                index = lv_dropdown_get_selected(obj);               
                if(index == 1) // TEMP
                {
                    lv_obj_clear_state(objs.slider_timer_monday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_temp_monday, heatingConfigArray[0].stTempRange.u8TempMin, heatingConfigArray[0].stTempRange.u8TempMax);
                }
                else if(index == 2) //TIMER
                {
                    lv_obj_clear_state(objs.slider_temp_monday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_timer_monday, heatingConfigArray[0].stTimerRange.u8ClockMin, heatingConfigArray[0].stTimerRange.u8ClockMax);
                }
                else // NONE
                {
                    lv_obj_add_state(objs.slider_temp_monday,     LV_STATE_DISABLED);
                    lv_obj_add_state(objs.slider_timer_monday,    LV_STATE_DISABLED);
                }                
            break;

            case 2:
                ESP_LOGI(TAG, "Tuesday Selected "); 
                index = lv_dropdown_get_selected(obj);               
                if(index == 1) // TEMP
                {
                    lv_obj_clear_state(objs.slider_timer_tuesday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_temp_tuesday, heatingConfigArray[1].stTempRange.u8TempMin, heatingConfigArray[1].stTempRange.u8TempMax);
                }
                else if(index == 2) //TIMER
                {
                    lv_obj_clear_state(objs.slider_temp_tuesday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_timer_tuesday, heatingConfigArray[1].stTimerRange.u8ClockMin, heatingConfigArray[1].stTimerRange.u8ClockMax);
                }
                else // NONE
                {
                    lv_obj_add_state(objs.slider_temp_tuesday,     LV_STATE_DISABLED);
                    lv_obj_add_state(objs.slider_timer_tuesday,    LV_STATE_DISABLED);
                }                
            break;

            case 3: //wednesday
                ESP_LOGI(TAG, "Wednesday Selected "); 
                index = lv_dropdown_get_selected(obj);               
                if(index == 1) // TEMP
                {
                    lv_obj_clear_state(objs.slider_timer_wednesday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_temp_wednesday, heatingConfigArray[2].stTempRange.u8TempMin, heatingConfigArray[2].stTempRange.u8TempMax);
                }
                else if(index == 2) //TIMER
                {
                    lv_obj_clear_state(objs.slider_temp_wednesday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_timer_wednesday, heatingConfigArray[2].stTimerRange.u8ClockMin, heatingConfigArray[2].stTimerRange.u8ClockMax);
                }
                else // NONE
                {
                    lv_obj_add_state(objs.slider_temp_wednesday,     LV_STATE_DISABLED);
                    lv_obj_add_state(objs.slider_timer_wednesday,    LV_STATE_DISABLED);
                }                
            break;

            case 4: //thursday
                ESP_LOGI(TAG, "Thursday Selected "); 
                index = lv_dropdown_get_selected(obj);               
                if(index == 1) // TEMP
                {
                    lv_obj_clear_state(objs.slider_timer_thursday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_temp_thursday, heatingConfigArray[3].stTempRange.u8TempMin, heatingConfigArray[3].stTempRange.u8TempMax);
                }
                else if(index == 2) //TIMER
                {
                    lv_obj_clear_state(objs.slider_temp_thursday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_timer_thursday, heatingConfigArray[3].stTimerRange.u8ClockMin, heatingConfigArray[3].stTimerRange.u8ClockMax);
                }
                else // NONE
                {
                    lv_obj_add_state(objs.slider_temp_thursday,     LV_STATE_DISABLED);
                    lv_obj_add_state(objs.slider_timer_thursday,    LV_STATE_DISABLED);
                }                
            break;

            case 5: //thursday
                ESP_LOGI(TAG, "Friday Selected "); 
                index = lv_dropdown_get_selected(obj);               
                if(index == 1) // TEMP
                {
                    lv_obj_clear_state(objs.slider_timer_friday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_temp_friday, heatingConfigArray[4].stTempRange.u8TempMin, heatingConfigArray[4].stTempRange.u8TempMax);
                }
                else if(index == 2) //TIMER
                {
                    lv_obj_clear_state(objs.slider_temp_friday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_timer_friday, heatingConfigArray[4].stTimerRange.u8ClockMin, heatingConfigArray[4].stTimerRange.u8ClockMax);
                }
                else // NONE
                {
                    lv_obj_add_state(objs.slider_temp_friday,     LV_STATE_DISABLED);
                    lv_obj_add_state(objs.slider_timer_friday,    LV_STATE_DISABLED);
                }                
            break;

            case 6: //thursday
                ESP_LOGI(TAG, "Saturday Selected "); 
                index = lv_dropdown_get_selected(obj);               
                if(index == 1) // TEMP
                {
                    lv_obj_clear_state(objs.slider_timer_saturday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_temp_saturday, heatingConfigArray[5].stTempRange.u8TempMin, heatingConfigArray[5].stTempRange.u8TempMax);
                }
                else if(index == 2) //TIMER
                {
                    lv_obj_clear_state(objs.slider_temp_saturday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_timer_saturday, heatingConfigArray[5].stTimerRange.u8ClockMin, heatingConfigArray[5].stTimerRange.u8ClockMax);
                }
                else // NONE
                {
                    lv_obj_add_state(objs.slider_temp_saturday,     LV_STATE_DISABLED);
                    lv_obj_add_state(objs.slider_timer_saturday,    LV_STATE_DISABLED);
                }                
            break;

            case 7: //Saturday
                ESP_LOGI(TAG, "Sunday Selected "); 
                index = lv_dropdown_get_selected(obj);               
                if(index == 1) // TEMP
                {
                    lv_obj_clear_state(objs.slider_timer_sunday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_temp_sunday, heatingConfigArray[6].stTempRange.u8TempMin, heatingConfigArray[6].stTempRange.u8TempMax);
                }
                else if(index == 2) //TIMER
                {
                    lv_obj_clear_state(objs.slider_temp_sunday, LV_STATE_DISABLED);
                    lv_slider_set_range(objs.slider_timer_sunday, heatingConfigArray[6].stTimerRange.u8ClockMin, heatingConfigArray[6].stTimerRange.u8ClockMax);
                }
                else // NONE
                {
                    lv_obj_add_state(objs.slider_temp_sunday,     LV_STATE_DISABLED);
                    lv_obj_add_state(objs.slider_timer_sunday,    LV_STATE_DISABLED);
                }                
            break;            

            default:
            break;
        }
    }
}


int8_t getHeatingConfig (HEATING_CONFIG *out_ptrToHeatingConfig)
{
    int8_t iRet = 0;
    if(out_ptrToHeatingConfig == NULL)
    {
        iRet = -1;
    }
    else
    {

    }
    return iRet;
}
int8_t setHeatingConfig (const HEATING_CONFIG *const in_ptrToHeatingConfig)
{
    int8_t iRet = 0;
    if(in_ptrToHeatingConfig == NULL)
    {
        iRet = -1;
    }
    else
    {
        
    }
    return iRet;
}

int8_t InitHeatingConfig(void)
{
    static bool bInitFs = false;
    int8_t iRet = 0;

    // Monday
    lblTempArray[0].lblTempMin = objects.monday_temp_min;
    lblTempArray[0].u16XposLblTempMin = 368;
    lblTempArray[0].u16YposLblTempMin = 84;
    lblTempArray[0].lblTempMax = objects.monday_temp_max;
    lblTempArray[0].u16XposLblTempMax = 637;
    lblTempArray[0].u16YposLblTempMax = 84;
    lblTimeArray[0].lblTimeMin = objects.monday_time_min;
    lblTimeArray[0].u16XposLblTimeMin = 692;
    lblTimeArray[0].u16YposLblTimeMin = 84;
    lblTimeArray[0].lblTimeMax = objects.monday_time_max;
    lblTimeArray[0].u16XposLblTimeMax = 964;
    lblTimeArray[0].u16YposLblTimeMax = 84;

    //Tuesday
    lblTempArray[1].lblTempMin = objects.tuesday_temp_min;
    lblTempArray[1].u16XposLblTempMin = 368;
    lblTempArray[1].u16YposLblTempMin = 143;
    lblTempArray[1].lblTempMax = objects.tuesday_temp_max;
    lblTempArray[1].u16XposLblTempMax = 637;
    lblTempArray[1].u16YposLblTempMax = 143;
    lblTimeArray[1].lblTimeMin = objects.tuesday_time_min;
    lblTimeArray[1].u16XposLblTimeMin = 692;
    lblTimeArray[1].u16YposLblTimeMin = 143;
    lblTimeArray[1].lblTimeMax = objects.tuesday_time_max;
    lblTimeArray[1].u16XposLblTimeMax = 964;
    lblTimeArray[1].u16YposLblTimeMax = 143;

    // Wednesday
    lblTempArray[2].lblTempMin = objects.wednesday_temp_min;
    lblTempArray[2].u16XposLblTempMin = 368;
    lblTempArray[2].u16YposLblTempMin = 203;
    lblTempArray[2].lblTempMax = objects.wednesday_temp_max;
    lblTempArray[2].u16XposLblTempMax = 637;
    lblTempArray[2].u16YposLblTempMax = 203;
    lblTimeArray[2].lblTimeMin = objects.wednesday_time_min;
    lblTimeArray[2].u16XposLblTimeMin = 692;
    lblTimeArray[2].u16YposLblTimeMin = 203;
    lblTimeArray[2].lblTimeMax = objects.wednesday_time_max;
    lblTimeArray[2].u16XposLblTimeMax = 964;
    lblTimeArray[2].u16YposLblTimeMax = 203;

    // Thursday
    lblTempArray[3].lblTempMin = objects.thursday_temp_min;
    lblTempArray[3].u16XposLblTempMin = 368;
    lblTempArray[3].u16YposLblTempMin = 263;
    lblTempArray[3].lblTempMax = objects.thursday_temp_max;
    lblTempArray[3].u16XposLblTempMax = 637;
    lblTempArray[3].u16YposLblTempMax = 263;
    lblTimeArray[3].lblTimeMin = objects.thursday_time_min;
    lblTimeArray[3].u16XposLblTimeMin = 692;
    lblTimeArray[3].u16YposLblTimeMin = 263;
    lblTimeArray[3].lblTimeMax = objects.thursday_time_max;
    lblTimeArray[3].u16XposLblTimeMax = 964;
    lblTimeArray[3].u16YposLblTimeMax = 263;

    // Friday
    lblTempArray[4].lblTempMin = objects.friday_temp_min;
    lblTempArray[4].u16XposLblTempMin = 368;
    lblTempArray[4].u16YposLblTempMin = 320;
    lblTempArray[4].lblTempMax = objects.friday_temp_max;
    lblTempArray[4].u16XposLblTempMax = 637;
    lblTempArray[4].u16YposLblTempMax = 320;
    lblTimeArray[4].lblTimeMin = objects.friday_time_min;
    lblTimeArray[4].u16XposLblTimeMin = 692;
    lblTimeArray[4].u16YposLblTimeMin = 320;
    lblTimeArray[4].lblTimeMax = objects.friday_time_max;
    lblTimeArray[4].u16XposLblTimeMax = 964;
    lblTimeArray[4].u16YposLblTimeMax = 320;

    lblTempArray[5].lblTempMin = objects.saturday_temp_min;
    lblTempArray[5].u16XposLblTempMin = 368;
    lblTempArray[5].u16YposLblTempMin = 381;
    lblTempArray[5].lblTempMax = objects.saturday_temp_max;
    lblTempArray[5].u16XposLblTempMax = 637;
    lblTempArray[5].u16YposLblTempMax = 381;
    lblTimeArray[5].lblTimeMin = objects.saturday_time_min;
    lblTimeArray[5].u16XposLblTimeMin = 692;
    lblTimeArray[5].u16YposLblTimeMin = 381;
    lblTimeArray[5].lblTimeMax = objects.saturday_time_max;
    lblTimeArray[5].u16XposLblTimeMax = 964;
    lblTimeArray[5].u16YposLblTimeMax = 381;
    
    lblTempArray[6].lblTempMin = objects.sunday_temp_min;
    lblTempArray[6].u16XposLblTempMin = 368;
    lblTempArray[6].u16YposLblTempMin = 440;
    lblTempArray[6].lblTempMax = objects.sunday_temp_max;
    lblTempArray[6].u16XposLblTempMax = 637;
    lblTempArray[6].u16YposLblTempMax = 440;
    lblTimeArray[6].lblTimeMin = objects.sunday_time_min;
    lblTimeArray[6].u16XposLblTimeMin = 692;
    lblTimeArray[6].u16YposLblTimeMin = 440;
    lblTimeArray[6].lblTimeMax = objects.sunday_time_max;
    lblTimeArray[6].u16XposLblTimeMax = 964;
    lblTimeArray[6].u16YposLblTimeMax = 440;   
    
    //memeset(heatingConfigArray,0,sizeof(heatingConfigArray));
/*
    for(int i = 0; i < HEATING_CONFIG_ARRAY_SIZE;i++)
    {
        lv_obj_set_pos(lblTempArray[i].lblTempMin, lblTempArray[i].u16XposLblTempMin , lblTempArray[i].u16YposLblTempMin);
        lv_obj_set_pos(lblTempArray[i].lblTempMax, lblTempArray[i].u16XposLblTempMax , lblTempArray[i].u16YposLblTempMax);

        lv_obj_set_pos(lblTimeArray[i].lblTimeMin, lblTimeArray[i].u16XposLblTimeMin , lblTimeArray[i].u16YposLblTimeMin);
        lv_obj_set_pos(lblTimeArray[i].lblTimeMax, lblTimeArray[i].u16XposLblTimeMax , lblTimeArray[i].u16YposLblTimeMax);
    }
*/
    if(bInitFs == false)
    {
        esp_vfs_spiffs_conf_t conf = 
        {
            .base_path = "/spiffs",      // Punto di montaggio nel VFS
            .partition_label = NULL,     // NULL usa la prima partizione spiffs trovata
            .max_files = 1,              // Numero massimo di file aperti contemporaneamente
            .format_if_mount_failed = true // Formatta se la partizione è vuota o corrotta
        };

        esp_err_t ret = esp_vfs_spiffs_register(&conf);
        if (ret != ESP_OK)
        {
            // Gestione errore (es. ESP_FAIL o ESP_ERR_NOT_FOUND)      
            ESP_LOGE(TAG, "SPIFFS Failure %s",esp_err_to_name(ret));   
            iRet = -1;   
        }
        else
        {           
            bInitFs = true;            
        }
    }
    else
    {
        iRet = -1;
    }
    return iRet;
}


void action_heating_save_config(lv_event_t *e) 
{
    if(OpenFile("w") != -1)
    {
        if(ReadConfig(heatingConfigArrayTmp) != -1)
        {
            CloseFile();
        }
    }
    // save only if user change values
    if(memcmp(heatingConfigArrayTmp,heatingConfigArray,sizeof(heatingConfigArray)) != 0)
    {
        if(OpenFile("w") != -1)
        {
            if(WriteConfig(heatingConfigArray) != -1)
            {
                CloseFile();
            }
        }
    }
}

void action_heating_reset_config(lv_event_t *e) 
{
    action_heating_screen_config(e);
}