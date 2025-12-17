#include "config.h"
#include "pcf8523.h"

const char *TAG_CONFIG = "config";    // Tag for Station mode (Wi-Fi client mode)
static USER_CONFIG stUSerConfig;
static wifi_ap_record_t wifi_scann_list[DEFAULT_SCAN_LIST_SIZE];  // Array to store the AP records

int8_t iConfigInit(void)
{    
    memset(&stUSerConfig, 0, sizeof(USER_CONFIG));
    return wifi_init();
}
/*
void vGetDefaultConfig(USER_CONFIG *pUserConfig)
{
    memcpy(pUserConfig, &stUSerConfig, sizeof(USER_CONFIG));
    return;
}
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Action Callback When RealTme Settings Screen is loaded
/// @param e   Pointer to LVGL event structure
/// @return none
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_settings_screen_cb(lv_event_t *e) 
{
    struct tm currentTime;
    objects_t objs               = objects;
    lv_obj_t *dropdown           = objs.drop_day;
    uint8_t id                   = 0;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);

    if(Pcf8523_Get_Time(&currentTime) < 0) 
    {
        ESP_LOGE(TAG_CONFIG, "Failed to get time from PCF8563");
    } else 
    {
        stUSerConfig.stRtcClock.tm_year = currentTime.tm_year;
        stUSerConfig.stRtcClock.tm_mon  = currentTime.tm_mon;   
        stUSerConfig.stRtcClock.tm_mday = currentTime.tm_mday;
        stUSerConfig.stRtcClock.tm_hour = currentTime.tm_hour;
        stUSerConfig.stRtcClock.tm_min  = currentTime.tm_min;
        stUSerConfig.stRtcClock.tm_sec  = currentTime.tm_sec;

        
        ESP_LOGI(TAG_CONFIG, "Current Date/Time: %04d-%02d-%02d %02d:%02d:%02d",
                    currentTime.tm_mday, 
                    currentTime.tm_mon, 
                    currentTime.tm_year,
                    currentTime.tm_hour, 
                    currentTime.tm_min, 
                    currentTime.tm_sec);
                   
        int iWeekDay = dayofweek(currentTime.tm_year, currentTime.tm_mon, currentTime.tm_mday);        
        stUSerConfig.stRtcClock.tm_wday = iWeekDay;
        lv_dropdown_set_selected(dropdown, stUSerConfig.stRtcClock.tm_mday - 1); // Days are 1-31, dropdown index starts from 0

        // Months
        dropdown = objs.drop_month;
        lv_dropdown_set_selected(dropdown, stUSerConfig.stRtcClock.tm_mon - 1); // Months are 1-12, dropdown index starts from 0

        // Years        
        dropdown = objs.drop_year;
        id = stUSerConfig.stRtcClock.tm_year - 2025; // Assuming dropdown starts from 2020      
        lv_dropdown_set_selected(dropdown, id);

        // Hours
        dropdown = objs.drop_hour;
        lv_dropdown_set_selected(dropdown, stUSerConfig.stRtcClock.tm_hour); // Hours are 0-23

        // Minutes  
        dropdown = objs.drop_minute;
        lv_dropdown_set_selected(dropdown, stUSerConfig.stRtcClock.tm_min); // Minutes are 0-59
    
    }        
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RealTime Clock Settings Section
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Action Callback When RealTme Set Clock Button is clicked
/// @param e  Pointer to LVGL event structure
/// @return none
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_btn_real_time_set_clock_cb(lv_event_t *e) 
{
    objects_t objs               = objects;
    lv_obj_t *SwManRTC_NtpServer = objs.sw_manual_rtc_ntp_server;
    lv_event_code_t code = lv_event_get_code(e);
    int iDayOfweek;
    if(code == LV_EVENT_CLICKED) 
    {
        if(stUSerConfig.eRtcManualAuto == RTC_MANUAL)
        {
            ESP_LOGI(TAG_CONFIG, "Set RTC Manual ");

            iDayOfweek = dayofweek(stUSerConfig.stRtcClock.tm_mday,
                                   stUSerConfig.stRtcClock.tm_mon,
                                   stUSerConfig.stRtcClock.tm_year);
            if(iDayOfweek < 0)
            {
                iDayOfweek = 0;
            }
            stUSerConfig.stRtcClock.tm_wday = iDayOfweek;
            Pcf8523_Set_Time(&stUSerConfig.stRtcClock);            
	    }
        else
        {
            ESP_LOGI(TAG_CONFIG, "Set RTC From NTP Server ");
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Action Callback When RealTme Switch Manual/NTP Server is clicked
/// @param e  Pointer to LVGL event structure
/// @return none
//////////////////////////////////////////////////////////////////////////////////////////////////////
void action_sw_manual_rtc_ntp_server(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        ESP_LOGI(TAG_CONFIG, "State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        stUSerConfig.eRtcManualAuto = lv_obj_has_state(obj, LV_STATE_CHECKED) ? RTC_MANUAL : RTC_FROM_NTP_SERVER;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Action Callback When RealTme Date/Time Dropdown is changed
/// @param e  Pointer to LVGL event structure
/// @return none
//////////////////////////////////////////////////////////////////////////////////////////////////////
void action_drop_date_time(lv_event_t *e) 
{
    char strBuffer[16];
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj        = lv_event_get_target(e);    
    uint8_t userData     = (uint32_t)(uintptr_t)lv_event_get_user_data(e);

    if(code == LV_EVENT_VALUE_CHANGED) 
    { 
        switch(userData)
        {
            case 1:
                ESP_LOGI(TAG_CONFIG, "Day Selected ");                
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_CONFIG, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_mday = atoi(strBuffer);                
                break;
            case 2:
                ESP_LOGI(TAG_CONFIG, "Month Selected ");                
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_CONFIG, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_mon = lv_dropdown_get_selected(obj) + 1; // Months are 0-11 in struct tm               
                break;
            case 3:
                ESP_LOGI(TAG_CONFIG, "Year Selected ");                
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_CONFIG, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_year = atoi(strBuffer);                
                break;
            case 4:
                ESP_LOGI(TAG_CONFIG, "Hour Selected ");
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_CONFIG, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_hour = atoi(strBuffer);
                break;
            case 5:
                ESP_LOGI(TAG_CONFIG, "Minute Selected ");
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_CONFIG, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_min = atoi(strBuffer);
                break;    
            default:
                break;
        }        
    } 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// WiFi Section
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Action Callback When WiFi Scan Button is Clicked
/// @param e   Pointer to LVGL event structure
/// @return none
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_wifi_scann(lv_event_t *e) 
{
    lv_event_code_t code     = lv_event_get_code(e);
    lv_obj_t *btn_WifiScan   = lv_event_get_target(e);   
    objects_t objs           = objects;  
    lv_obj_t *list_wifi_ssid = objs.drp_wifi_ssid;
    char wifiAppInfo[128]; // Array to hold SSID strings of found APs
    
    if(code == LV_EVENT_CLICKED) 
    {
        ESP_LOGI(TAG_CONFIG, "WiFi Scan Clicked ");        
        lv_obj_add_state(btn_WifiScan, LV_STATE_DISABLED); // Disable the scan button during scanning 
        lv_dropdown_clear_options(list_wifi_ssid);  // Clear existing options in the dropdown       
     
        int iNumberOfAP = iWifiScan(wifi_scann_list, DEFAULT_SCAN_LIST_SIZE);
        for (int i = 0; i < iNumberOfAP; i++) 
        {
            memset(wifiAppInfo, 0, sizeof(wifiAppInfo));
            sprintf(wifiAppInfo, "%s (RSSI: %d CH: %d)", wifi_scann_list[i].ssid,wifi_scann_list[i].rssi,wifi_scann_list[i].primary);    
            
            lv_dropdown_add_option(list_wifi_ssid, wifiAppInfo, i);
            ESP_LOGI(TAG_CONFIG, "SSID \t\t%s", wifi_scann_list[i].ssid);  // Log SSID (network name)
            ESP_LOGI(TAG_CONFIG, "RSSI \t\t%d", wifi_scann_list[i].rssi);  // Log RSSI (signal strength)        
            ESP_LOGI(TAG_CONFIG, "Channel \t\t%d", wifi_scann_list[i].primary);  // Log channel number
        }
        lv_obj_clear_state(btn_WifiScan, LV_STATE_DISABLED); // Re-enable the scan button after scanning is complete
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Select SSID from Dropdown
/// @param  e Pointer to LVGL event structure
/// @return none
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_ssid_select(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj       = lv_event_get_target(e);    
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
       // char buf[64];
        //lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        
       // LV_LOG_USER("Option: %s", buf);
        ESP_LOGI(TAG_CONFIG, "%s ", wifi_scann_list[lv_dropdown_get_selected(obj)].ssid);  // Log RSSI (signal strength)
        memcpy(stUSerConfig.strWifiSsid, wifi_scann_list[lv_dropdown_get_selected(obj)].ssid, sizeof(stUSerConfig.strWifiSsid));
    }    
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Action Callback When WiFi Password Textbox is edited
/// @param  e Pointer to LVGL event structure
/// @return none
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_wifi_txt_psw(lv_event_t *e) 
{      
    objects_t objs    = objects;
    lv_keyboard_t *kb = (lv_keyboard_t *)objs.kek_keyboard;
   
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_BOTTOM_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_TEXT_LOWER ); // Set keyboard to number mode for IP address input  
        ESP_LOGI(TAG_CONFIG, "Click On Wifi Psw Textbox ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        memcpy(stUSerConfig.strWifiPassword, lv_textarea_get_text(ta), sizeof(stUSerConfig.strWifiPassword));        
        ESP_LOGI(TAG_CONFIG, "Defocus On Wifi Psw Textbox ");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Action Callback When WiFi Connect Button is Clicked
/// @param  e Pointer to LVGL event structure
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_wifi_connect_cb(lv_event_t *e) 
{
    lv_event_code_t code     = lv_event_get_code(e);
    lv_obj_t *btn_WifiScan   = lv_event_get_target(e);   
    objects_t objs           = objects;
    
    //wifi_ap_record_t wifi_scann_list[DEFAULT_SCAN_LIST_SIZE];  // Array to store the AP records

    if(code == LV_EVENT_CLICKED) 
    {
        if(iWifiConnectInStationMode((uint8_t *)stUSerConfig.strWifiSsid, (uint8_t *)stUSerConfig.strWifiPassword, WIFI_AUTH_WPA2_PSK) < 0) 
        {      
            ESP_LOGE(TAG_CONFIG, "iWifiConnectInStationMode Failed ");
            static const char * btns[] ={"OK", ""};
            lv_obj_t * mboxError = lv_msgbox_create(NULL, "Error", "WiFi Connection Error", btns, true);
            lv_obj_center(mboxError);
        }

        lv_textarea_add_text(objs.txt_ipaddress, stUSerConfig.stNetworkConfig.strIpAddr);;
        lv_textarea_add_text(objs.txt_netmask,   stUSerConfig.stNetworkConfig.strNetMAsk);;
        lv_textarea_add_text(objs.txt_gateway,   stUSerConfig.stNetworkConfig.strGateway);;

       // stop_wifi();
        #if 0
        wifi_config_t wifi_configuration;
        const char *your_ssid = "HUAWEI-B535-13F7";//stUSerConfig.strWifiSsid;
        const char *your_pass = "Intrepido123.";//stUSerConfig.strWifiPassword;
        strcpy((char*)wifi_configuration.sta.ssid,your_ssid); // copy chars from hardcoded configs to struct
        strcpy((char*)wifi_configuration.sta.password,your_pass);
        wifi_configuration.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;//setting authmode
        esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);//setting up configs when event ESP_IF_WIFI_STA
        esp_wifi_start();//start connection with configurations provided in funtion
        esp_wifi_set_mode(WIFI_MODE_STA);//station mode selected
        if(esp_wifi_connect() != ESP_OK)//connecting to wifi with provided ssid and pass
        {
            ESP_LOGE(TAG_CONFIG, "Failed to Connect to WiFi SSID: %s", stUSerConfig.strWifiSsid);
        }
        else
        {
            ESP_LOGI(TAG_CONFIG, "Successfully Connected to WiFi SSID: %s with password %s", stUSerConfig.strWifiSsid,stUSerConfig.strWifiPassword);
            esp_wifi_disconnect(); //disconnecting after successful connection
        }
        #endif
        //printf( "wifi_init_softap finished. SSID:%s  password:%s",your_ssid,your_pass);

        #if 0
        ESP_LOGI(TAG_CONFIG, "Connect Clicked ");
        if(-1 == iWifiConnectInStationMode((uint8_t *)stUSerConfig.strWifiSsid, (uint8_t *)stUSerConfig.strWifiPassword, WIFI_AUTH_WPA2_PSK))
        {
            ESP_LOGE(TAG_CONFIG, "Failed to Connect to WiFi SSID: %s", stUSerConfig.strWifiSsid);
        }
        else
        {
            ESP_LOGI(TAG_CONFIG, "Successfully Connected to WiFi SSID: %s", stUSerConfig.strWifiSsid);
        }    
            #endif   
    }     
}
