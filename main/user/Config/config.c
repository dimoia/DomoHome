#include "config.h"
#include "pcf8523.h"
#include "LVGLCustom.h"

const char *TAG_CONFIG = "config";    // Tag for Station mode (Wi-Fi client mode)
static USER_CONFIG stUSerConfig;
static wifi_ap_record_t wifi_scann_list[DEFAULT_SCAN_LIST_SIZE];  // Array to store the AP records
static CONFIG_STATUS enActualState = NOT_CONFIG;

static const char *strKeyArray[] = 
{
    "Hostname"
    "WifiSsid",
    "WifiPass",
    "Ipaddress",
    "Gateway",
    "Netmask",
    "StaticDinamicIP",
    "RtcManualAuto",
    "NtpServer",
    "WeatherServer",
    "WeatherApiKey",
    "MqttStatus",
    "MqttServer",
    "MqttPort",
    "MqttUsername",
    "MqttPassword",
    "MqttClientId",
    "MqttTopic",
    "MqttSubscribe"
};

void iConfigInit(void)
{    
   memset(&stUSerConfig, 0, sizeof(USER_CONFIG));
   return;
}

void vGetConfig(USER_CONFIG *pUserConfig)
{
    if(pUserConfig != NULL)
    {
        memcpy(pUserConfig, &stUSerConfig, sizeof(USER_CONFIG));
    }
    return;
}

void initDeviceConfig(DEVICE_CONFIG* out_ptrDeviceConfig)
{
    if(out_ptrDeviceConfig != NULL)
    {
        out_ptrDeviceConfig->bBME280Status = DEVICE_STATUS_ERROR; // Imposta lo stato iniziale del BME280 come errore
    }
}
void setBME280Status(DEVICE_CONFIG* in_ptrDeviceConfig, DEVICE_STATUS in_bStatus)
{
    if(in_ptrDeviceConfig != NULL)
    {
        in_ptrDeviceConfig->bBME280Status = in_bStatus;
    }
}

DEVICE_STATUS getBME280Status(const DEVICE_CONFIG* in_ptrDeviceConfig)
{
    if(in_ptrDeviceConfig != NULL)
    {
        return in_ptrDeviceConfig->bBME280Status;
    }
    return DEVICE_STATUS_ERROR;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////// Configuration Status Section
///////////////////////////////////////////////////////////////////////////////////////////////////////
CONFIG_STATUS getConfigStatus(void)
{
    return enActualState;
}
void setConfigStatus(CONFIG_STATUS in_eConfigStatus)
{
    enActualState = in_eConfigStatus;
}


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
/// Action Callback When Hostname Textbox is edited
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_txt_hostname_cb(lv_event_t *e) 
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
        lv_obj_move_foreground(kb); 
        lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_TEXT_LOWER ); // Set keyboard to number mode for IP address input          
        ESP_LOGI(TAG_CONFIG, "Click On Hostname Textbox ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        memcpy(stUSerConfig.strHostname, lv_textarea_get_text(ta), sizeof(stUSerConfig.strHostname));
        ESP_LOGI(TAG_CONFIG, "Defocus On Hostname Textbox  ");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Select SSID from Dropdown
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_ssid_select(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj       = lv_event_get_target(e);    
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        ESP_LOGI(TAG_CONFIG, "%s ", wifi_scann_list[lv_dropdown_get_selected(obj)].ssid);  // Log RSSI (signal strength)
        memcpy(stUSerConfig.strWifiSsid, wifi_scann_list[lv_dropdown_get_selected(obj)].ssid, sizeof(stUSerConfig.strWifiSsid));
    }    
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///  Action Callback When WiFi Password Textbox is edited
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
        lv_obj_move_foreground(kb); 
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
/// Action Callback When WiFi Connect Button is Clicked
/// e Pointer to LVGL event structure
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
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
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// File System Configuration Section
///////////////////////////////////////////////////////////////////////////////////////////////////////

#include "nvs_flash.h"
#include "esp_log.h"
#include "nvs.h"

//CONFIG_KEY_VALUE_PAIR createConfigKeyValuePair[MAX_KEY_VALUE_PAIRS];
static nvs_handle_t my_handle;
static size_t iKeyValueLength = 0;

static int8_t get_value_size_by_key(const char* in_ptrFilename, const char* in_ptrKey, size_t* out_ptrValueSize)
{
    int8_t iRet = 0;
    
    if ((out_ptrValueSize == NULL) || (in_ptrFilename == NULL) || (in_ptrKey == NULL))
    {
        ESP_LOGE(TAG_CONFIG, "One or more input pointers are NULL");
        iRet = -1;
    }
    else
    {
        // 1. Apri il namespace "storage" in modalità sola lettura
        if (nvs_open(in_ptrFilename, NVS_READONLY, &my_handle) != ESP_OK)
        {
            iRet = -1;
            ESP_LOGE(TAG_CONFIG, "Error opening %s ", in_ptrFilename);
        }
        else 
        {
            size_t required_size;
            if (nvs_get_str(my_handle, in_ptrKey, NULL, &required_size) == ESP_OK)
            {
                *out_ptrValueSize = required_size;
            } 
            else 
            {
                ESP_LOGE(TAG_CONFIG, "Key %s not present in %s", in_ptrKey, in_ptrFilename);
                iRet = -1;
            }  
            nvs_close(my_handle); // 3. Chiudi sempre l'handle         
        }        
    }    
    return iRet;
}

static int8_t get_value_by_key(const char* in_ptrFilename, const char* in_ptrKey,char* out_ptrValue, size_t in_ptrValueSize)
{
    esp_err_t err;
    int8_t iRet = 0;

    // 1. Apri il namespace "storage" in modalità sola lettura
    if (nvs_open(in_ptrFilename, NVS_READONLY, &my_handle) != ESP_OK)
    {
        iRet = -1;
        ESP_LOGE(TAG_CONFIG, "Error opening %s ", in_ptrFilename);
    }
    else 
    {
        if( nvs_get_str(my_handle, in_ptrKey, out_ptrValue, in_ptrValueSize) == ESP_OK)
        {
            ESP_LOGE(TAG_CONFIG, "Key %s present with value: %s", in_ptrKey, out_ptrValue);
            nvs_close(my_handle);
        }
        else 
        {
            ESP_LOGE(TAG_CONFIG, "Key %s not present in %s", in_ptrKey, in_ptrFilename);
            iRet = -1;
        }
    }    
    return iRet;
}

static int8_t check_for_config_file(const char* in_ptrFilename)
{
    int8_t iRet = 0;
    // Try to open config file
    if(nvs_open(in_ptrFilename, NVS_READONLY, &my_handle) != ESP_OK)
    {
        iRet = -1;
        ESP_LOGE(TAG_CONFIG, "Error opening %s ", in_ptrFilename);
    }
    else
    {
        ESP_LOGI(TAG_CONFIG, "Configuration file '%s' exists.", in_ptrFilename);
        nvs_close(my_handle); // Close the handle after checking
    }
    return iRet;
}

static int8_t delete_file(const char* filename)
{
    int8_t iRet = 0;
    // Try to close config file
    if(nvs_open(filename, NVS_READONLY, &my_handle) == ESP_OK)
    {
        nvs_erase_all(my_handle); // Erase all key-value pairs in the namespace
        nvs_commit(my_handle);    // Commit the changes to ensure they are saved
        nvs_close(my_handle);     // Close the handle after erasing
        ESP_LOGI(TAG_CONFIG, "File '%s' deleted successfully.", filename);
    }
    else
    {
        iRet = -1;
        ESP_LOGE(TAG_CONFIG, "Error opening NVS handle for deletion");
    }
    return iRet;
}
#if 0
static int8_t read_config(const char* in_ptrFilename,const char *in_ptrKey, char* out_ptrValue, size_t *out_ptrMaxLen)

    int8_t iRet = 0;

    // 1. Apri il namespace "storage" in modalità sola lettura
    err = nvs_open(in_ptrFilename, NVS_READONLY, &my_handle);
    if (err != ESP_OK) 
    {
        iRet = -1;
        ESP_LOGE(TAG_CONFIG, "Error opening NVS handle: %s", esp_err_to_name(err));
    }
    else 
    {
        size_t required_size;
        if(nvs_get_str(my_handle, in_ptrKey, NULL, &required_size) == ESP_OK)
        {
            nvs_get_str(my_handle, in_ptrKey, out_ptrValue, out_ptrMaxLen);
            ESP_LOGE(TAG_CONFIG, "Configurazione trovata: %s", out_ptrValue);
        }        
        else 
        {
            iRet = -1;
            ESP_LOGE(TAG_CONFIG, "Error reading from NVS");
        }
    }
    // 3. Chiudi sempre l'handle
    nvs_close(my_handle);
    return iRet;
}


esp_err_t save_wifi_credentials(const char* ssid, const char* pass) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // 1. Apri il namespace "config" in modalità scrittura
    err = nvs_open("config", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) return err;

    // 2. Scrivi l'SSID
    err = nvs_set_str(my_handle, "wifi_ssid", ssid);
    if (err != ESP_OK) {
        nvs_close(my_handle);
        return err;
    }

    // 3. Scrivi la Password
    err = nvs_set_str(my_handle, "wifi_pass", pass);
    if (err != ESP_OK) {
        nvs_close(my_handle);
        return err;
    }

    // 4. IMPORTANTE: Esegui il commit per salvare fisicamente i dati
    err = nvs_commit(my_handle);
    
    // 5. Chiudi l'handle
    nvs_close(my_handle);
    return err;
}
#endif


#if 0
int8_t iDownloadConfigFileFromNVS(const char* in_ptrFilename)
{
    int iRet = 0;
    if(check_for_config_file(in_ptrFilename) == 0)
    {
        
        if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[0].key, &iKeyValueLength) == 0)
        {
            createConfigKeyValuePair[0].value = malloc(iKeyValueLength);        
            if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[0].key, createConfigKeyValuePair[0].value, iKeyValueLength) != 0)
            {
                free(createConfigKeyValuePair[0].value);
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[0].key);
                iRet = -1;
            }
            else
            {
                stUSerConfig.strWifiSsid[0] = '\0'; // Ensure the string is null-terminated
                strncpy(stUSerConfig.strWifiSsid, createConfigKeyValuePair[0].value, sizeof(stUSerConfig.strWifiSsid) - 1); // Copy value to user config
                ESP_LOGI(TAG_CONFIG, "Key: %s Value: %s", createConfigKeyValuePair[0].key, createConfigKeyValuePair[0].value);
            }
        }
        else
        {
            ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[0].key);
            iRet = -1;
        }

        if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[0].key, &iKeyValueLength) == 0)
        {
            createConfigKeyValuePair[0].value = malloc(iKeyValueLength);        
            if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[0].key, createConfigKeyValuePair[0].value, iKeyValueLength) != 0)
            {
                free(createConfigKeyValuePair[0].value);
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[0].key);
                iRet = -1;
            }
            else
            {
                stUSerConfig.strWifiSsid[0] = '\0'; // Ensure the string is null-terminated
                strncpy(stUSerConfig.strWifiSsid, createConfigKeyValuePair[0].value, sizeof(stUSerConfig.strWifiSsid) - 1); // Copy value to user config
                ESP_LOGI(TAG_CONFIG, "Key: %s Value: %s", createConfigKeyValuePair[0].key, createConfigKeyValuePair[0].value);
            }
        }
        else
        {
            ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[0].key);
            iRet = -1;
        }

        if(iRet == 0)
        {
            //createConfigKeyValuePair[1].key = "wifi_pass";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[1].key   , &iKeyValueLength) == 0)
            {
                createConfigKeyValuePair[1].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[1].key, createConfigKeyValuePair[1].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[1].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[1].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[1].key);
                iRet = -1;
            }
        }
        if(iRet == 0)
        {
           // createConfigKeyValuePair[2].key = "ipaddress";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[2].key   , &iKeyValueLength) == 0)
            {
                createConfigKeyValuePair[2].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[2].key, createConfigKeyValuePair[2].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[2].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[2].key);
                    iRet = -1;
                }
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[2].key);
                iRet = -1;
            }            
        }
        if(iRet == 0)
        {              
            //createConfigKeyValuePair[3].key = "gateway";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[3].key   , &iKeyValueLength)== 0)
            {
                createConfigKeyValuePair[3].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[3].key, createConfigKeyValuePair[3].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[3].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[3].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[3].key);
                iRet = -1;
            }
        }
        else
        {
            ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[3].key);
            iRet = -1;
        }
        if(iRet == 0)
        {
            //createConfigKeyValuePair[4].key = "netmask";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[4].key   , &iKeyValueLength)==0)
            {
                createConfigKeyValuePair[4].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[4].key, createConfigKeyValuePair[4].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[4].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[4].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[4].key);
                iRet = -1;
            }
        }

        if(iRet == 0)
        {
            //createConfigKeyValuePair[5].key = "hostname";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[5].key   , &iKeyValueLength) == 0)
            {
                createConfigKeyValuePair[5].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[5].key, createConfigKeyValuePair[5].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[5].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[5].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[5].key);
                iRet = -1;
            }
        }
        else
        {
            ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[5].key);
            iRet = -1;
        }

        if(iRet == 0)
        {
            //createConfigKeyValuePair[6].key = "static_dinamicIP";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[6].key   , &iKeyValueLength) == 0)
            {
                createConfigKeyValuePair[6].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[6].key, createConfigKeyValuePair[6].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[5].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[5].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[5].key);
                iRet = -1;
            }
        }
        else
        {
            ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[5].key);
            iRet = -1;
        }
  
        if(iRet == 0)
        {
            //createConfigKeyValuePair[7].key = "ntpServer";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[7].key   , &iKeyValueLength) == 0)
            {
                createConfigKeyValuePair[7].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[7].key, createConfigKeyValuePair[7].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[7].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[7].key);
                    iRet = -1;
                }   
            }
            else    
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[7].key);
                iRet = -1;
            }
        }

        if(iRet == 0)
        {   
            //createConfigKeyValuePair[8].key = "timezone";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[8].key   , &iKeyValueLength) == 0)
             {
                createConfigKeyValuePair[8].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[8].key, createConfigKeyValuePair[8].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[8].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[8].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[8].key);
                iRet = -1;
            }
        }
        
        if(iRet == 0)
        {        
            //createConfigKeyValuePair[9].key = "mqttBrokerIP";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[9].key   , &iKeyValueLength) == 0)
            {
                createConfigKeyValuePair[9].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[9].key, createConfigKeyValuePair[9].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[9].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[9].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[9].key);
                iRet = -1;
            }
        }
        else
        {
            ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[9].key);
            iRet = -1;
        }
        
        if(iRet == 0)
        {              
            //createConfigKeyValuePair[10].key = "mqttUserId";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[10].key   , &iKeyValueLength) == 0)
            {   
                createConfigKeyValuePair[10].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[10].key, createConfigKeyValuePair[10].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[10].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[10].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[10].key);
                iRet = -1;
            }
        }

        if(iRet == 0)
        {   
            //createConfigKeyValuePair[11].key = "mqttPsw";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[11].key   , &iKeyValueLength) == 0)
            {   
                createConfigKeyValuePair[11].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[11].key, createConfigKeyValuePair[11].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[11].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[11].key);
                    iRet = -1;
                }   
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[11].key);
                iRet = -1;
            }
        }  
        if(iRet == 0)
        {        
            //createConfigKeyValuePair[12].key = "mqttClientId";
            if(get_value_size_by_key(in_ptrFilename, createConfigKeyValuePair[12].key   , &iKeyValueLength) == 0)
            {
                createConfigKeyValuePair[12].value = malloc(iKeyValueLength);    
                if(get_value_by_key(in_ptrFilename, createConfigKeyValuePair[12].key, createConfigKeyValuePair[12].value, iKeyValueLength) != 0)
                {
                    free(createConfigKeyValuePair[12].value);
                    ESP_LOGE(TAG_CONFIG, "Errore nel recupero del valore per chiave: %s", createConfigKeyValuePair[12].key);
                    iRet = -1;
                }
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Errore nel recupero della dimensione del valore per chiave: %s", createConfigKeyValuePair[12].key);
                iRet = -1;
            }
        }

        ESP_LOGI(TAG_CONFIG, "Config file '%s' downloaded successfully.", in_ptrFilename);
    }
    else
    {
        iRet = -1;
    }
    return iRet;
}
#else
int8_t iDownloadConfigFileFromNVS(const char* in_ptrFilename)
{
    int8_t iRet = 0;
    int8_t i = 0;
    char ptrTmpKeyValueBuffer[64]; // Temporary buffer to hold key values, adjust size as needed

    if(in_ptrFilename != NULL)
    {
        ESP_LOGI(TAG_CONFIG, "Attempting to download config file '%s' from NVS.", in_ptrFilename);

        while( (i < (sizeof(strKeyArray) / sizeof(strKeyArray[0]))) && (iRet == 0) )        
        {
            ESP_LOGI(TAG_CONFIG, "Processing key: %s", strKeyArray[i]);
            if(get_value_size_by_key(in_ptrFilename, strKeyArray[i], &iKeyValueLength) == 0)
            {
                //ptrTmpKeyValueBuffer = malloc(iKeyValueLength);        
                if(get_value_by_key(in_ptrFilename, strKeyArray[i], ptrTmpKeyValueBuffer, iKeyValueLength) != 0)
                {                    
                    ESP_LOGE(TAG_CONFIG, "Error retrieving value for key: %s", strKeyArray[i]);
                    iRet = -1;
                }
                else
                {
                    stUSerConfig.strWifiSsid[0] = '\0'; // Ensure the string is null-terminated
                    strncpy(stUSerConfig.strWifiSsid, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.strWifiSsid) - 1); // Copy value to user config
                    ESP_LOGI(TAG_CONFIG, "Key: %s Value: %s", strKeyArray[i], ptrTmpKeyValueBuffer);
                }
            }
            else
            {
                ESP_LOGE(TAG_CONFIG, "Error retrieving size for key: %s", strKeyArray[i]);
                iRet = -1;
            }
            //free(ptrTmpKeyValueBuffer);
            i++;            
        }
        if(iRet == 0)
        {
            ESP_LOGI(TAG_CONFIG, "Config file '%s' downloaded successfully.", in_ptrFilename);
        }              
    }
    else
    {
        ESP_LOGE(TAG_CONFIG, "Input filename pointer is NULL.");
        iRet = -1;
    }
    return iRet;
}
#endif
void action_save_to_flash(lv_event_t *e) 
{
    nvs_handle_t writeHandle;
    USER_CONFIG objUserConfig;

    nvs_flash_erase_partition("nvs"); // remove NVS partition to ensure clean state before writing new config data
    nvs_flash_init();

    // Try to open config file
    if(nvs_open("config.txt", NVS_READWRITE, &writeHandle) != ESP_OK)
    {
        ESP_LOGE(TAG_CONFIG, "Error opening config.txt ");
        MsgConfigBox();
    }
    else
    {
        ESP_LOGI(TAG_CONFIG, "Configuration file config.txt.");
        vGetConfig(&objUserConfig);

        nvs_set_str(writeHandle, "Hostname", objUserConfig.strHostname);
        nvs_set_str(writeHandle, "WifiSsid", objUserConfig.strWifiSsid);
        nvs_set_str(writeHandle, "WifiPass", objUserConfig.strWifiPassword);

        nvs_set_str(writeHandle, "Ipaddress", objUserConfig.stNetworkConfig.strIpAddr);
        nvs_set_str(writeHandle, "Gateway", objUserConfig.stNetworkConfig.strGateway);
        nvs_set_str(writeHandle, "Netmask", objUserConfig.stNetworkConfig.strNetMAsk);

        // Static IP represented as 0, Dynamic IP represented as 1
        if(objUserConfig.stNetworkConfig.eStaticDynamic == STATIC_IP)
        {
            nvs_set_u32(writeHandle, "StaticDinamicIP", 0); 
        }
        else
        {
            nvs_set_u32(writeHandle, "StaticDinamicIP", 1);
        }
        // Clock Settings
        if(objUserConfig.eRtcManualAuto == RTC_FROM_NTP_SERVER)
        {
            // NTP Auto Clock Settings
            nvs_set_u32(writeHandle, "RtcManualAuto", 1);
            nvs_set_str(writeHandle, "NtpServer", objUserConfig.strNtpServer);
/*
            nvs_set_u32(writeHandle, "Day"   , 1);
            nvs_set_u32(writeHandle, "Month" , 1);
            nvs_set_u32(writeHandle, "Year"  , 2025);
            nvs_set_u32(writeHandle, "Hour"  , 1);
            nvs_set_u32(writeHandle, "Minute", 0);
*/             
        }
        else 
        {
            nvs_set_u32(writeHandle, "RtcManualAuto", 0); 
            /*
            nvs_set_u32(writeHandle, "Day"   , objUserConfig.stRtcClock.tm_wday);
            nvs_set_u32(writeHandle, "Month" , objUserConfig.stRtcClock.tm_mon);
            nvs_set_u32(writeHandle, "Year"  , objUserConfig.stRtcClock.tm_year);
            nvs_set_u32(writeHandle, "Hour"  , objUserConfig.stRtcClock.tm_hour);
            nvs_set_u32(writeHandle, "Minute", objUserConfig.stRtcClock.tm_min);
            nvs_set_u32(writeHandle, "Second", 0);        
            */
        }
        // Weather Server Settings
        nvs_set_str(writeHandle, "WeatherServer", objUserConfig.stWeatherConfig.strWeatherServer);
        nvs_set_str(writeHandle, "WeatherApiKey", objUserConfig.stWeatherConfig.strWeatherApiKey);
     
        // Mqtt Home Assistant Settings
        nvs_set_str(writeHandle, "MqttServer"   , objUserConfig.stMqttConfig.strMqttBrokerIpAddr);
        nvs_set_u32(writeHandle, "MqttPort"     , objUserConfig.stMqttConfig.u16MqttBrokerPort);
        nvs_set_str(writeHandle, "MqttUsername" , objUserConfig.stMqttConfig.strMqttUserID);
        nvs_set_str(writeHandle, "MqttPassword" , objUserConfig.stMqttConfig.strMqttPassword);
        nvs_set_str(writeHandle, "MqttClientId" , objUserConfig.stMqttConfig.strMqttUserID);
        nvs_set_str(writeHandle, "MqttTopic"    , objUserConfig.stMqttConfig.strMqttTopic);
        nvs_set_str(writeHandle, "MqttSubscribe", objUserConfig.stMqttConfig.strMqttSubscribe);

        if(objUserConfig.stMqttConfig.bMqttEnable)
        {
            // Mqtt Protocol Enabled
            nvs_set_u32(writeHandle, "MqttStatus", 1); 
        }
        else
        {
            nvs_set_u32(writeHandle, "MqttStatus", 0); 
        } 
        nvs_commit(writeHandle); // Commit the changes to ensure they are saved
        nvs_close(writeHandle);  // Close the handle after writing              
    }
}


void action_btn_erase_nvs(lv_event_t *e) 
{
    nvs_flash_erase_partition("nvs"); // remove NVS partition to ensure clean state before writing new config data
    nvs_flash_init();
}
