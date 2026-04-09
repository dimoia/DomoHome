#include "config.h"
#include "pcf8523.h"
#include "LVGLCustom.h"
#include "esp_mac.h"

const char *TAG_CONFIG = "config";    // Tag for Station mode (Wi-Fi client mode)
static USER_CONFIG stUSerConfig;
static wifi_ap_record_t wifi_scann_list[DEFAULT_SCAN_LIST_SIZE];  // Array to store the AP records
static CONFIG_STATUS enActualState = NOT_CONFIG;
static lv_keyboard_t *kb;

#define KEY_ARRAY_SIZE 20

static const char *strKeyArray[] = 
{
    "Hostname",
    "WifiSsid",
    "WifiPass",
    "Ipaddress",
    "Gateway",
    "Netmask",
    "StaticDinamicIP",
    "RtcManualAuto",
    "NtpServer",
    "WeatherStatus",
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
static const char * kb_map[] = {"1", "2", "3", "\n",
                                "4", "5", "6", "\n",
                                "7", "8", "9", "\n",
                                ".", "0", LV_SYMBOL_BACKSPACE, ""};
static const lv_btnmatrix_ctrl_t kb_ctrl[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

static const char * kb_map_number[] = {"1", "2", "3", "\n",
                                       "4", "5", "6", "\n",
                                       "7", "8", "9", "\n",
                                       "0", LV_SYMBOL_BACKSPACE, ""};
static const lv_btnmatrix_ctrl_t kb_ctrl_number[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};                                



static int8_t get_mac_address(uint8_t *ptrMacAddr) 
{
    
    
    // Legge il MAC address per l'interfaccia Wi-Fi Station
    // ESP32-S3 ha il MAC di fabbrica pre-programmato negli eFuse
    esp_err_t ret = esp_read_mac(ptrMacAddr, ESP_MAC_WIFI_STA);

    if (ret == ESP_OK) 
    {
        ESP_LOGI(TAG_CONFIG, "Indirizzo MAC (STA) %02x:%02x:%02x:%02x:%02x:%02x",
               ptrMacAddr[0], ptrMacAddr[1], ptrMacAddr[2], ptrMacAddr[3], ptrMacAddr[4], ptrMacAddr[5]);
        return 0;
    } 
    else 
    {
        return -1;
    }
}

// Function to disable a textarea and handle associated keyboard (if needed)
static void disable_textarea_with_kb(lv_obj_t *ta, lv_keyboard_t *kb) 
{
    lv_obj_add_state(ta, LV_STATE_DISABLED  );
    // Additional logic (e.g., hiding keyboard) can be added here if required
}   

static void enable_kb(lv_obj_t *ta, lv_keyboard_t *kb) 
{
    // This function can be used to enable the textarea and show the keyboard if needed
    lv_obj_clear_state(ta, LV_STATE_DISABLED);
    // Additional logic (e.g., showing keyboard) can be added here if required
}


static void disable_and_hide_kb(lv_obj_t * ta, lv_obj_t * kb) 
{
    // 1. Disabilita la textbox (diventa grigia e non cliccabile)
    lv_obj_add_state(ta, LV_STATE_DISABLED);

    // 2. Rimuovi il focus (il bordo blu/arancione di selezione)
    lv_obj_clear_state(ta, LV_STATE_FOCUSED);

    // 3. Controlla se la tastiera è collegata a QUESTA specifica textbox
    if(lv_keyboard_get_textarea(kb) == ta) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN); // Nascondi la tastiera
        lv_keyboard_set_textarea(kb, NULL);      // Scollegala per sicurezza
    }
}

static void disable_net_config(lv_event_t *e)
{
    objects_t objs = objects;
        
    // If Dynamic IP is selected, hide the IP address, gateway, and netmask fields       
    static lv_style_t style_grigio;
    lv_style_init(&style_grigio);

    // Colore del testo grigio
    lv_style_set_text_color(&style_grigio, lv_palette_main(LV_PALETTE_GREY));
    // Sfondo grigio molto chiaro
    lv_style_set_bg_color(&style_grigio, lv_palette_lighten(LV_PALETTE_GREY, 3));
    // Opzionale: rendi l'intero widget un po' trasparente
    lv_style_set_opa(&style_grigio, LV_OPA_60);

    // APPLICA LO STILE SOLO PER LO STATO DISABLED
    lv_obj_add_style(objs.txt_ipaddress, &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_gateway, &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_netmask, &style_grigio, LV_STATE_DISABLED);
    
    /* lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;

    disable_and_hide_kb(objs.txt_ipaddress, kb);
    disable_and_hide_kb(objs.txt_gateway, kb);
    disable_and_hide_kb(objs.txt_netmask, kb);
}

static void enable_net_config(lv_event_t *e)
{
    objects_t objs = objects;
    // Rimuovi lo stato DISABLED per rendere i campi nuovamente interattivi
    lv_obj_clear_state(objs.txt_ipaddress, LV_STATE_DISABLED);
    lv_obj_clear_state(objs.txt_gateway, LV_STATE_DISABLED);
    lv_obj_clear_state(objs.txt_netmask, LV_STATE_DISABLED);
    // Rimuovi lo stile grigio (opzionale, se vuoi tornare al colore originale)
    lv_obj_remove_style(objs.txt_ipaddress, NULL, LV_STATE_DISABLED);
    lv_obj_remove_style(objs.txt_gateway, NULL, LV_STATE_DISABLED);
    lv_obj_remove_style(objs.txt_netmask, NULL, LV_STATE_DISABLED);
    
    /* lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;
    // La tastiera verrà mostrata automaticamente quando l'utente clicca su uno dei campi abilitati, quindi non è necessario forzarne la visualizzazione qui.
    enable_kb(objs.txt_ipaddress, kb);
}

static void tab_disable_net_config(lv_event_t *e)
{
    objects_t objs = objects;
        
    // If Dynamic IP is selected, hide the IP address, gateway, and netmask fields       
    static lv_style_t style_grigio;
    lv_style_init(&style_grigio);

    // Colore del testo grigio
    lv_style_set_text_color(&style_grigio, lv_palette_main(LV_PALETTE_GREY));
    // Sfondo grigio molto chiaro
    lv_style_set_bg_color(&style_grigio, lv_palette_lighten(LV_PALETTE_GREY, 3));
    // Opzionale: rendi l'intero widget un po' trasparente
    lv_style_set_opa(&style_grigio, LV_OPA_60);

    // APPLICA LO STILE SOLO PER LO STATO DISABLED
    lv_obj_add_style(objs.txt_tab_net_ipaddress, &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_tab_net_gateway, &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_tab_net_netmask, &style_grigio, LV_STATE_DISABLED);
    
    /* lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;

    disable_and_hide_kb(objs.txt_tab_net_ipaddress, kb);
    disable_and_hide_kb(objs.txt_tab_net_gateway, kb);
    disable_and_hide_kb(objs.txt_tab_net_netmask, kb);
}

static void tab_enable_net_config(lv_event_t *e)
{
    objects_t objs = objects;
    // Rimuovi lo stato DISABLED per rendere i campi nuovamente interattivi
    lv_obj_clear_state(objs.txt_tab_net_ipaddress, LV_STATE_DISABLED);
    lv_obj_clear_state(objs.txt_tab_net_gateway, LV_STATE_DISABLED);
    lv_obj_clear_state(objs.txt_tab_net_netmask, LV_STATE_DISABLED);
    // Rimuovi lo stile grigio (opzionale, se vuoi tornare al colore originale)
    lv_obj_remove_style(objs.txt_tab_net_ipaddress, NULL, LV_STATE_DISABLED);
    lv_obj_remove_style(objs.txt_tab_net_gateway, NULL, LV_STATE_DISABLED);
    lv_obj_remove_style(objs.txt_tab_net_netmask, NULL, LV_STATE_DISABLED);
    
    /* lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;
    // La tastiera verrà mostrata automaticamente quando l'utente clicca su uno dei campi abilitati, quindi non è necessario forzarne la visualizzazione qui.
    enable_kb(objs.txt_tab_net_ipaddress, kb);
}

static void enable_netweather_config(void)
{
     objects_t objs = objects;
     lv_obj_clear_state(objs.combo_weather_server, LV_STATE_DISABLED);

     lv_obj_clear_state(objs.txt_weather_key, LV_STATE_DISABLED);
     lv_obj_remove_style(objs.txt_weather_key, NULL, LV_STATE_DISABLED);
}

static void disable_netweather_config(void)
{
    objects_t objs = objects;
    lv_obj_add_state(objs.combo_weather_server, LV_STATE_DISABLED); // La rende non cliccabile e grigia
        
  
    // If Dynamic IP is selected, hide the IP address, gateway, and netmask fields       
    static lv_style_t style_grigio;
    lv_style_init(&style_grigio);

    // Colore del testo grigio
    lv_style_set_text_color(&style_grigio, lv_palette_main(LV_PALETTE_GREY));
    // Sfondo grigio molto chiaro
    lv_style_set_bg_color(&style_grigio, lv_palette_lighten(LV_PALETTE_GREY, 3));
    // Opzionale: rendi l'intero widget un po' trasparente
    lv_style_set_opa(&style_grigio, LV_OPA_60);

    // APPLICA LO STILE SOLO PER LO STATO DISABLED
    lv_obj_add_style(objs.txt_weather_key, &style_grigio, LV_STATE_DISABLED);
  
    kb = (lv_keyboard_t *)objs.kek_keyboard;
    disable_and_hide_kb(objs.txt_weather_key, kb);
    
}

static void enable_ntp_server(void)
{
    objects_t objs = objects;
    lv_obj_add_state(objs.drop_day, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.drop_month, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.drop_year, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.drop_hour, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.drop_minute, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.btn_set_clock, LV_STATE_DISABLED);

    lv_obj_clear_state(objs.txt_ntp_server, LV_STATE_DISABLED);
    lv_obj_remove_style(objs.txt_ntp_server, NULL, LV_STATE_DISABLED);
}

static void disable_ntp_server(void)
{

    objects_t objs = objects;

    lv_obj_clear_state(objs.drop_day, LV_STATE_DISABLED);        
    lv_obj_clear_state(objs.btn_set_clock, LV_STATE_DISABLED);

    // If Dynamic IP is selected, hide the IP address, gateway, and netmask fields       
    static lv_style_t style_grigio;
    lv_style_init(&style_grigio);

    // Colore del testo grigio
    lv_style_set_text_color(&style_grigio, lv_palette_main(LV_PALETTE_GREY));
    // Sfondo grigio molto chiaro
    lv_style_set_bg_color(&style_grigio, lv_palette_lighten(LV_PALETTE_GREY, 3));
    // Opzionale: rendi l'intero widget un po' trasparente
    lv_style_set_opa(&style_grigio, LV_OPA_60);

    // APPLICA LO STILE SOLO PER LO STATO DISABLED
    lv_obj_add_style(objs.txt_ntp_server, &style_grigio, LV_STATE_DISABLED);
  
    kb = (lv_keyboard_t *)objs.kek_keyboard;
    disable_and_hide_kb(objs.txt_ntp_server, kb);
}

static void tab_enable_netweather_config(void)
{
     objects_t objs = objects;
     lv_obj_clear_state(objs.tab_time_config_sw_wheater, LV_STATE_DISABLED);

     lv_obj_clear_state(objs.tab_time_txt_weather_key, LV_STATE_DISABLED);
     lv_obj_remove_style(objs.tab_time_txt_weather_key, NULL, LV_STATE_DISABLED);
}

static void tab_disable_netweather_config(void)
{
    objects_t objs = objects;
    lv_obj_add_state(objs.tab_time_config_sw_wheater, LV_STATE_DISABLED); // La rende non cliccabile e grigia
        
  
    // If Dynamic IP is selected, hide the IP address, gateway, and netmask fields       
    static lv_style_t style_grigio;
    lv_style_init(&style_grigio);

    // Colore del testo grigio
    lv_style_set_text_color(&style_grigio, lv_palette_main(LV_PALETTE_GREY));
    // Sfondo grigio molto chiaro
    lv_style_set_bg_color(&style_grigio, lv_palette_lighten(LV_PALETTE_GREY, 3));
    // Opzionale: rendi l'intero widget un po' trasparente
    lv_style_set_opa(&style_grigio, LV_OPA_60);

    // APPLICA LO STILE SOLO PER LO STATO DISABLED
    lv_obj_add_style(objs.tab_time_txt_weather_key, &style_grigio, LV_STATE_DISABLED);
  
    kb = (lv_keyboard_t *)objs.kek_keyboard;
    disable_and_hide_kb(objs.tab_time_txt_weather_key, kb);
    
}

static void tab_enable_ntp_server(void)
{
    objects_t objs = objects;
    lv_obj_add_state(objs.drop_tab_time_config_day, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.drop_tab_time_config_month, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.drop_tab_time_config_year, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.drop_tab_time_config_hour, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.drop_tab_time_config_minute, LV_STATE_DISABLED); // La rende non cliccabile e grigia
    lv_obj_add_state(objs.btn_tab_time_config_set_clock, LV_STATE_DISABLED);

    lv_obj_clear_state(objs.tab_time_confiig_txt_ntp_server, LV_STATE_DISABLED);
    lv_obj_remove_style(objs.tab_time_confiig_txt_ntp_server, NULL, LV_STATE_DISABLED);
}

static void tab_disable_ntp_server(void)
{

    objects_t objs = objects;

    lv_obj_clear_state(objs.drop_tab_time_config_day, LV_STATE_DISABLED);        
    lv_obj_clear_state(objs.btn_tab_time_config_set_clock, LV_STATE_DISABLED);

    // If Dynamic IP is selected, hide the IP address, gateway, and netmask fields       
    static lv_style_t style_grigio;
    lv_style_init(&style_grigio);

    // Colore del testo grigio
    lv_style_set_text_color(&style_grigio, lv_palette_main(LV_PALETTE_GREY));
    // Sfondo grigio molto chiaro
    lv_style_set_bg_color(&style_grigio, lv_palette_lighten(LV_PALETTE_GREY, 3));
    // Opzionale: rendi l'intero widget un po' trasparente
    lv_style_set_opa(&style_grigio, LV_OPA_60);

    // APPLICA LO STILE SOLO PER LO STATO DISABLED
    lv_obj_add_style(objs.tab_time_confiig_txt_ntp_server, &style_grigio, LV_STATE_DISABLED);
  
    kb = (lv_keyboard_t *)objs.kek_keyboard;
    disable_and_hide_kb(objs.tab_time_confiig_txt_ntp_server, kb);
}

void enable_mqtt_config(void)
{
   objects_t objs = objects;
   lv_obj_clear_state(objs.txt_broker_ip,     LV_STATE_DISABLED);
   lv_obj_clear_state(objs.txt_broker_port,   LV_STATE_DISABLED);
   lv_obj_clear_state(objs.txt_mqttclient_id, LV_STATE_DISABLED);
   lv_obj_clear_state(objs.txt_mqttpassword,  LV_STATE_DISABLED);
   lv_obj_clear_state(objs.txt_mqttuserid,    LV_STATE_DISABLED);
   lv_obj_clear_state(objs.txt_mqtttopic,     LV_STATE_DISABLED);
   lv_obj_clear_state(objs.txt_mqttsubscribe, LV_STATE_DISABLED);

}

void disable_mqtt_config(void)
{
    objects_t objs = objects;
    static lv_style_t style_grigio;
    kb = (lv_keyboard_t *)objs.kek_keyboard;

    lv_style_init(&style_grigio);

    // Colore del testo grigio
    lv_style_set_text_color(&style_grigio, lv_palette_main(LV_PALETTE_GREY));
    // Sfondo grigio molto chiaro
    lv_style_set_bg_color(&style_grigio, lv_palette_lighten(LV_PALETTE_GREY, 3));
    // Opzionale: rendi l'intero widget un po' trasparente
    lv_style_set_opa(&style_grigio, LV_OPA_60);

    // APPLICA LO STILE SOLO PER LO STATO DISABLED
    lv_obj_add_style(objs.txt_broker_ip,     &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_broker_port,   &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_mqttclient_id, &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_mqttpassword,  &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_mqttuserid,    &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_mqtttopic,     &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.txt_mqttsubscribe, &style_grigio, LV_STATE_DISABLED);
  
    
    disable_and_hide_kb(objs.txt_broker_ip,     kb);
    disable_and_hide_kb(objs.txt_broker_port,   kb);
    disable_and_hide_kb(objs.txt_mqttclient_id, kb);
    disable_and_hide_kb(objs.txt_mqttpassword,  kb);
    disable_and_hide_kb(objs.txt_mqttuserid,    kb);
    disable_and_hide_kb(objs.txt_mqtttopic,     kb);
    disable_and_hide_kb(objs.txt_mqttsubscribe, kb);
}

void tab_enable_mqtt_config(void)
{
   objects_t objs = objects;
   lv_obj_clear_state(objs.tab_ha_txt_broker_ip,     LV_STATE_DISABLED);
   lv_obj_clear_state(objs.tab_ha_txt_broker_port,   LV_STATE_DISABLED);
   lv_obj_clear_state(objs.tab_ha_txt_mqttclient_id, LV_STATE_DISABLED);
   lv_obj_clear_state(objs.tab_ha_txt_mqttpassword,  LV_STATE_DISABLED);
   lv_obj_clear_state(objs.tab_ha_txt_mqttuserid,    LV_STATE_DISABLED);
   lv_obj_clear_state(objs.tab_ha_txt_mqtttopic,     LV_STATE_DISABLED);
   lv_obj_clear_state(objs.tab_ha_txt_mqttsubscribe, LV_STATE_DISABLED);

}

void tab_disable_mqtt_config(void)
{
    objects_t objs = objects;
    static lv_style_t style_grigio;
    kb = (lv_keyboard_t *)objs.kek_keyboard;

    lv_style_init(&style_grigio);

    // Colore del testo grigio
    lv_style_set_text_color(&style_grigio, lv_palette_main(LV_PALETTE_GREY));
    // Sfondo grigio molto chiaro
    lv_style_set_bg_color(&style_grigio, lv_palette_lighten(LV_PALETTE_GREY, 3));
    // Opzionale: rendi l'intero widget un po' trasparente
    lv_style_set_opa(&style_grigio, LV_OPA_60);

    // APPLICA LO STILE SOLO PER LO STATO DISABLED
    lv_obj_add_style(objs.tab_ha_txt_broker_ip,     &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.tab_ha_txt_broker_port,   &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.tab_ha_txt_mqttclient_id, &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.tab_ha_txt_mqttpassword,  &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.tab_ha_txt_mqttuserid,    &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.tab_ha_txt_mqtttopic,     &style_grigio, LV_STATE_DISABLED);
    lv_obj_add_style(objs.tab_ha_txt_mqttsubscribe, &style_grigio, LV_STATE_DISABLED);
  
    
    disable_and_hide_kb(objs.tab_ha_txt_broker_ip,     kb);
    disable_and_hide_kb(objs.tab_ha_txt_broker_port,   kb);
    disable_and_hide_kb(objs.tab_ha_txt_mqttclient_id, kb);
    disable_and_hide_kb(objs.tab_ha_txt_mqttpassword,  kb);
    disable_and_hide_kb(objs.tab_ha_txt_mqttuserid,    kb);
    disable_and_hide_kb(objs.tab_ha_txt_mqtttopic,     kb);
    disable_and_hide_kb(objs.tab_ha_txt_mqttsubscribe, kb);
}


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
#define LV_TEXTAREA_DEF_BULLET  "\xE2\x80\xA2"
void action_settings_screen_cb(lv_event_t *e) 
{
    struct tm currentTime;
    objects_t objs               = objects;
    lv_obj_t *dropdown           = objs.drop_day;
    uint8_t id                   = 0;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);
    
    ESP_LOGI(TAG_CONFIG, "###############################################################");
    ESP_LOGI(TAG_CONFIG, "Hostname       : %s", stUSerConfig.strHostname);
    ESP_LOGI(TAG_CONFIG, "WifiSsid       : %s", stUSerConfig.strWifiSsid);
    ESP_LOGI(TAG_CONFIG, "WifiPass       : %s", stUSerConfig.strWifiPassword);

    ESP_LOGI(TAG_CONFIG, "Ipaddress      : %s", stUSerConfig.stNetworkConfig.strIpAddr);                
    ESP_LOGI(TAG_CONFIG, "Gateway        : %s", stUSerConfig.stNetworkConfig.strGateway);
    ESP_LOGI(TAG_CONFIG, "NetMask        : %s", stUSerConfig.stNetworkConfig.strNetMAsk);
    if(stUSerConfig.stNetworkConfig.eStaticDynamic == STATIC_IP)
    {
        ESP_LOGI(TAG_CONFIG, "StaticDinamicIP: Static IP");
    }
    else
    {
        ESP_LOGI(TAG_CONFIG, "StaticDinamicIP: Dynamic IP");
    }
    if(stUSerConfig.eRtcManualAuto == RTC_MANUAL)
    {
        ESP_LOGI(TAG_CONFIG, "RtcManualAuto  : RTC");
    }
    else
    {
        ESP_LOGI(TAG_CONFIG, "RtcManualAuto  : NTP Server");                    
    }
    ESP_LOGI(TAG_CONFIG, "NtpServer      : %s", stUSerConfig.strNtpServer);
    ESP_LOGI(TAG_CONFIG, "Weather Status : %s", stUSerConfig.stWeatherConfig.bWeatherStatus == true ? "Enabled" : "Disabled");
    ESP_LOGI(TAG_CONFIG, "WeatherServer  : %s", stUSerConfig.stWeatherConfig.strWeatherServer);
    ESP_LOGI(TAG_CONFIG, "WeatherApiKey  : %s", stUSerConfig.stWeatherConfig.strWeatherApiKey);                    
    ESP_LOGI(TAG_CONFIG, "MqttStatus     : %s", stUSerConfig.stMqttConfig.bMqttEnable == true ? "Enabled" : "Disabled");       
    ESP_LOGI(TAG_CONFIG, "MqttServer     : %s", stUSerConfig.stMqttConfig.strMqttBrokerIpAddr);                
    ESP_LOGI(TAG_CONFIG, "MqttPort       : %d", stUSerConfig.stMqttConfig.u16MqttBrokerPort);                
    ESP_LOGI(TAG_CONFIG, "MqttUsername   : %s", stUSerConfig.stMqttConfig.strMqttUserID);
    ESP_LOGI(TAG_CONFIG, "MqttPassword   : %s", stUSerConfig.stMqttConfig.strMqttPassword);
    ESP_LOGI(TAG_CONFIG, "MqttClientId   : %s", stUSerConfig.stMqttConfig.strMqttClientID);                                        
    ESP_LOGI(TAG_CONFIG, "MqttTopic      : %s", stUSerConfig.stMqttConfig.strMqttTopic);        
    ESP_LOGI(TAG_CONFIG, "MqttSubscribe  : %s", stUSerConfig.stMqttConfig.strMqttSubscribe);
    ESP_LOGI(TAG_CONFIG, "###############################################################");

    // Get Hostname
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
    // Get Wi-Fi Password
    if (strlen(stUSerConfig.strWifiPassword) > 0)
    {
        lv_obj_t *ta = objs.txt_wifi_psw;       
        //lv_textarea_set_password_mode(ta, true);
        lv_textarea_set_text(ta, stUSerConfig.strWifiPassword);
        ESP_LOGI(TAG_CONFIG, "WifiPassword: %s", stUSerConfig.strWifiPassword);
    }
    // Get IP Address
    if (strlen(stUSerConfig.stNetworkConfig.strIpAddr) > 0)
    {
        lv_obj_t *ta = objs.txt_ipaddress;
        lv_textarea_set_text(ta, stUSerConfig.stNetworkConfig.strIpAddr);
    }
    // Get Gateway
    if (strlen(stUSerConfig.stNetworkConfig.strGateway) > 0)
    {
        lv_obj_t *ta = objs.txt_gateway;
        lv_textarea_set_text(ta, stUSerConfig.stNetworkConfig.strGateway);
    }
    // Get Netmask
    if (strlen(stUSerConfig.stNetworkConfig.strNetMAsk) > 0)
    {
        lv_obj_t *ta = objs.txt_netmask;
        lv_textarea_set_text(ta, stUSerConfig.stNetworkConfig.strNetMAsk);
    }
    // Get Static/Dynamic IP
    lv_obj_t *sw = objs.sw_static_ip_dynamic_ip;
    if(stUSerConfig.stNetworkConfig.eStaticDynamic == STATIC_IP)
    {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
        lv_obj_clear_state(objs.txt_ipaddress, LV_STATE_DISABLED);
        lv_obj_clear_state(objs.txt_gateway, LV_STATE_DISABLED);
        lv_obj_clear_state(objs.txt_netmask, LV_STATE_DISABLED);
    }
    else
    {        
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
        disable_net_config(NULL);              
    }
    // Get RTC Manual/NTP Server
    sw = objs.sw_manual_rtc_ntp_server;
    if(stUSerConfig.eRtcManualAuto == RTC_MANUAL)
    {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
        // If NTP Server is selected, hide the date/time dropdowns and set RTC from NTP server
        lv_obj_add_flag(objs.drop_day, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objs.drop_month, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objs.drop_year, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objs.drop_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objs.drop_minute, LV_OBJ_FLAG_HIDDEN);
        // Disable the "Set Clock" button since we will get time from NTP server
        lv_obj_add_flag(objs.btn_set_clock, LV_OBJ_FLAG_HIDDEN);        
    }
    // Get NTP Server
    if (strlen(stUSerConfig.strNtpServer) > 0)
    {
        lv_obj_t *ta = objs.txt_ntp_server;
        lv_textarea_set_text(ta, stUSerConfig.strNtpServer);
    }

    if(stUSerConfig.stWeatherConfig.bWeatherStatus == true)
    {
        // enable weather Server/Key
        enable_netweather_config();
        lv_obj_t *sw = objs.sw_wheater;
        lv_obj_add_state(sw, LV_STATE_CHECKED);
         ESP_LOGE(TAG_CONFIG, "stUSerConfig.stWeatherConfig.bWeatherStatus = true");
    }
    else
    {
        lv_obj_t *sw = objs.sw_wheater;
        disable_netweather_config();
        
        // disable weather Server/Key
        ///lv_obj_clear_state(objs.combo_weather_server, LV_STATE_DISABLED);
        //lv_obj_clear_state(objs.txt_weather_key, LV_STATE_DISABLED);
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
        ESP_LOGE(TAG_CONFIG, "stUSerConfig.stWeatherConfig.bWeatherStatus = false");
    }
    // Get Weather Server
    if (strlen(stUSerConfig.stWeatherConfig.strWeatherServer) > 0)
    {
        lv_obj_t *ta = objs.combo_weather_server;
        lv_textarea_set_text(ta, stUSerConfig.stWeatherConfig.strWeatherServer);
    }
    // Get Weather API Key
    if (strlen(stUSerConfig.stWeatherConfig.strWeatherApiKey) > 0)
    {
        lv_obj_t *ta = objs.btn_energy_power;
        lv_textarea_set_text(ta, stUSerConfig.stWeatherConfig.strWeatherApiKey);
    }
   
    // Get current time from RTC and populate the dropdowns
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
    if(stUSerConfig.stMqttConfig.bMqttEnable == true)
    {
        enable_mqtt_config();
        lv_obj_t *sw = objs.sw_wheater;
        lv_obj_add_state(sw, LV_STATE_CHECKED);     
    }
    else
    {
        lv_obj_t *sw = objs.sw_wheater;
        lv_obj_clear_state(sw, LV_STATE_CHECKED);

    }
    if(stUSerConfig.stMqttConfig.bMqttEnable == true)
    {
        if (strlen(stUSerConfig.stMqttConfig.strMqttBrokerIpAddr) > 0)
        {
            lv_obj_t *ta = objs.txt_broker_ip;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttBrokerIpAddr);        
        }   
        uint8_t u8MacAdd[6]; 
        if(get_mac_address(u8MacAdd) == 0)
        {
            lv_obj_t *ta = objs.txt_mqttclient_id;
            lv_textarea_set_text(ta, (char*)&u8MacAdd[0]);     
        }
        else
        {
            if (strlen(stUSerConfig.stMqttConfig.strMqttClientID) > 0)
            {
                lv_obj_t *ta = objs.txt_mqttclient_id;
                lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttClientID);  
            }
        }
        if(strlen(stUSerConfig.stMqttConfig.strMqttPassword)>0)
        {
            lv_obj_t *ta = objs.txt_mqttpassword;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttPassword);
        }
        if(strlen(stUSerConfig.stMqttConfig.strMqttUserID)>0)
        {
            lv_obj_t *ta = objs.txt_mqttuserid;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttUserID);
        }
        if(strlen(stUSerConfig.stMqttConfig.strMqttTopic)>0)
        {
            lv_obj_t *ta = objs.txt_mqtttopic;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttTopic);
        }
        if(strlen(stUSerConfig.stMqttConfig.strMqttSubscribe)>0)
        {
            lv_obj_t *ta = objs.txt_mqttsubscribe;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttSubscribe);
        }  
        lv_obj_t *ta = objs.txt_broker_port; 
        char ptrTmpBuffer[10]; // Temporary buffer to hold string representation of port number
        sprintf(ptrTmpBuffer, "%d", stUSerConfig.stMqttConfig.u16MqttBrokerPort); // Convert port number to string for storage        
        lv_textarea_set_text(ta, ptrTmpBuffer);             
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

void action_txt_ntp_server_cb(lv_event_t *e)
 {
    objects_t objs    = objects;
    kb = (lv_keyboard_t *)objs.kek_keyboard;
   
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_obj_move_foreground(kb); 
        lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_TEXT_LOWER ); // Set keyboard to number mode for IP address input  
        ESP_LOGI(TAG_CONFIG, "Click On NtpServer ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        memcpy(stUSerConfig.strNtpServer, lv_textarea_get_text(ta), sizeof(stUSerConfig.strNtpServer));        
        ESP_LOGI(TAG_CONFIG, "Defocus On NTPServer ");
    }
}   

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Action Callback When RealTme Switch Manual/NTP Server is clicked
/// @param e  Pointer to LVGL event structure
/// @return none
//////////////////////////////////////////////////////////////////////////////////////////////////////
void action_sw_manual_rtc_ntp_server(lv_event_t *e)
{    
    if (e == NULL) {
        ESP_LOGE(TAG_CONFIG, "Event pointer is NULL");
        return;
    }
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj        = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) 
    {
        bool isChecked = lv_obj_has_state(obj, LV_STATE_CHECKED);        
        if(isChecked) 
        {
            // Enabled NTP Server           
            stUSerConfig.eRtcManualAuto = RTC_FROM_NTP_SERVER;
            enable_ntp_server();
        } 
        else 
        {            
            // Use RTC
            stUSerConfig.eRtcManualAuto = RTC_MANUAL;
            disable_ntp_server();
        }
        ESP_LOGI(TAG_CONFIG, "NtpSwitch Status: %s", isChecked ? "Enabled" : "Disabled");
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
/// MQTT Section
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_sw_mqtt_status(lv_event_t *e) 
{
    if (e == NULL) {
        ESP_LOGE(TAG_CONFIG, "Event pointer is NULL");
        return;
    }
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj        = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) 
    {
        bool isChecked = lv_obj_has_state(obj, LV_STATE_CHECKED);
        stUSerConfig.stMqttConfig.bMqttEnable = isChecked;
        if(isChecked) 
        {
            // Enabled             
            enable_mqtt_config();
        } 
        else 
        {
            // Disabled
             disable_mqtt_config();
        }
        ESP_LOGI(TAG_CONFIG, "MQTT Status: %s", isChecked ? "Enabled" : "Disabled");
    }
}

void action_txt_broker_port(lv_event_t *e) 
{
    objects_t objs    = objects;
    kb = (lv_keyboard_t *)objs.kek_keyboard;
    uint8_t userData  = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);

    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map_number, kb_ctrl_number);
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1);
    }
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        stUSerConfig.stMqttConfig.u16MqttBrokerPort = atoi(lv_textarea_get_text(ta));        
        ESP_LOGI(TAG_CONFIG, "Defocus On Mqtt Broker Port ");
    }

}
void action_mqtt_config(lv_event_t *e) 
{
    objects_t objs    = objects;
    kb = (lv_keyboard_t *)objs.kek_keyboard;
    uint8_t userData  = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);

    if(code == LV_EVENT_FOCUSED) 
    {
        if(userData != 0) 
        {
            lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN); // mostra la tastiera
            lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
            lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
            lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1);        
        }
        else // Broker IP
        {
            lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
            lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
            lv_obj_move_foreground(kb); 
            lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_TEXT_LOWER ); // Set keyboard to number mode for IP address input          
        }
        ESP_LOGI(TAG_CONFIG, "Click On Mqtt Config ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        switch(userData)
        {
            case 0:
                // BrokerIP
                memcpy(stUSerConfig.stMqttConfig.strMqttBrokerIpAddr, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttBrokerIpAddr));
                ESP_LOGI(TAG_CONFIG, "Click On Broker ");
                break;
                /*
            case 1:
                memcpy(stUSerConfig.stMqttConfig.u16MqttBrokerPort, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.u16MqttBrokerPort));
                ESP_LOGI(TAG_CONFIG, "Click On Broker Port ");
                break;*/
            case 2:
                memcpy(stUSerConfig.stMqttConfig.strMqttClientID, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttClientID));
                ESP_LOGI(TAG_CONFIG, "Click On Client ID ");
                break;
            case 3:
                memcpy(stUSerConfig.stMqttConfig.strMqttPassword, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttPassword));
                ESP_LOGI(TAG_CONFIG, "Click On Password ");
                break;
            case 4:
                memcpy(stUSerConfig.stMqttConfig.strMqttUserID, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttUserID));
                ESP_LOGI(TAG_CONFIG, "Click On User ID ");
                break;
            case 5:
                memcpy(stUSerConfig.stMqttConfig.strMqttTopic, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttTopic));
                ESP_LOGI(TAG_CONFIG, "Click On Topic ");
                break;                
            case 6:
                memcpy(stUSerConfig.stMqttConfig.strMqttSubscribe, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttSubscribe));
                ESP_LOGI(TAG_CONFIG, "Click On Topic ");
                break;                                

            default:            
                break;
        }       
        ESP_LOGI(TAG_CONFIG, "Defocus On Network Config Textbox ");
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
    /*lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;
   
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
    /*lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;
   
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
// Action Callback When Network Config Textbox is edited (IP Address, Netmask, Gateway, Dns)
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_txt_net_cb(lv_event_t *e) 
{
    objects_t objs    = objects;
    /*lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;
    uint8_t userData  = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_BOTTOM_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1);

        //lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_NUMBER ); // Set keyboard to number mode for IP address input              
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        switch(userData)
        {
            case 1:
                // Ip Address
                memcpy(stUSerConfig.stNetworkConfig.strIpAddr, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strIpAddr));
                ESP_LOGI(TAG_CONFIG, "Click On IP Addr Textbox ");
                break;
            case 2:
                memcpy(stUSerConfig.stNetworkConfig.strNetMAsk, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strNetMAsk));
                ESP_LOGI(TAG_CONFIG, "Click On NetMask Textbox ");
                break;
            case 3:
                memcpy(stUSerConfig.stNetworkConfig.strGateway, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strGateway));
                ESP_LOGI(TAG_CONFIG, "Click On Gateway Textbox ");
                break;

            default:            
                break;
        }       
        ESP_LOGI(TAG_CONFIG, "Defocus On Network Config Textbox ");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Action Callback When Static/Dynamic IP Switch is toggled
/// e Pointer to LVGL event structure
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_sw_static_dynamic_ip(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        ESP_LOGI(TAG_CONFIG, "State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        if(lv_obj_has_state(obj, LV_STATE_CHECKED) == true) 
        {
            // Static  IP is selected
            enable_net_config(NULL);
           
        }
        else
        {
            // DHCP is selected
            disable_net_config(NULL);            
        }
        stUSerConfig.stNetworkConfig.eStaticDynamic = lv_obj_has_state(obj, LV_STATE_CHECKED) ? STATIC_IP : DYNAMIC_IP;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Weather Management
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_sw_wheater(lv_event_t *e) 
{
    // TODO: Implement action sw_wheater here
    if (e == NULL) 
    {
        ESP_LOGE(TAG_CONFIG, "Event pointer is NULL");
        return;
    }
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj        = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) 
    {
        bool isChecked = lv_obj_has_state(obj, LV_STATE_CHECKED);
        stUSerConfig.stWeatherConfig.bWeatherStatus = isChecked;
        if(isChecked) 
        {
            // Enabled             
            enable_netweather_config();
        } 
        else 
        {
            // Disabled
             disable_netweather_config();
        }
        ESP_LOGI(TAG_CONFIG, "Weather Status: %s", isChecked ? "Enabled" : "Disabled");
    }
}

void action_combo_weather_server(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj       = lv_event_get_target(e);    
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        // 1. Ottieni l'indice della voce selezionata (0, 1, 2...)
        uint16_t sel = lv_dropdown_get_selected(obj);

        // 2. Ottieni il testo della voce selezionata
        char buf[sizeof(stUSerConfig.stWeatherConfig.strWeatherServer)];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));

        ESP_LOGI(TAG_CONFIG, "Selected %s ", buf);
        memcpy(stUSerConfig.stWeatherConfig.strWeatherServer,buf, sizeof(stUSerConfig.stWeatherConfig.strWeatherServer));
    } 
}

void action_txt_weather_key(lv_event_t *e) 
{       
    objects_t objs    = objects;
    kb = (lv_keyboard_t *)objs.kek_keyboard;
   
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_obj_move_foreground(kb); 
        lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_TEXT_LOWER ); // Set keyboard to number mode for IP address input  
        ESP_LOGI(TAG_CONFIG, "Click On Weather Key ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        memcpy(stUSerConfig.stWeatherConfig.strWeatherApiKey, lv_textarea_get_text(ta), sizeof(stUSerConfig.stWeatherConfig.strWeatherApiKey));        
        ESP_LOGI(TAG_CONFIG, "Defocus On Weather Key ");
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

static int8_t get_value_by_key(const char* in_ptrFilename, const char* in_ptrKey,char* out_ptrValue, size_t *in_ptrValueSize)
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
            //ESP_LOGE(TAG_CONFIG, "Key %s present with value: %s", in_ptrKey, out_ptrValue);
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
#if 0
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


int8_t iDownloadConfigFileFromNVS(const char* in_ptrFilename)
{
    int8_t iRet = 0;
    int8_t i = 0;
    char ptrTmpKeyValueBuffer[64]; // Temporary buffer to hold key values, adjust size as needed

    if(in_ptrFilename != NULL)
    {
        ESP_LOGI(TAG_CONFIG, "Attempting to download config file '%s' from NVS.", in_ptrFilename);
        /*
        for(int j= 0; j<KEY_ARRAY_SIZE;j++)
        {
           ESP_LOGI(TAG_CONFIG, "Processing key: %s", strKeyArray[j]);
        }*/
        while( (i < KEY_ARRAY_SIZE) && (iRet == 0) )        
        {
            ESP_LOGI(TAG_CONFIG, "Processing key: %s", strKeyArray[i]);
            if(get_value_size_by_key(in_ptrFilename, strKeyArray[i], &iKeyValueLength) == 0)
            {
                //ptrTmpKeyValueBuffer = malloc(iKeyValueLength);        
                if(get_value_by_key(in_ptrFilename, strKeyArray[i], ptrTmpKeyValueBuffer, &iKeyValueLength) != 0)
                {                    
                    ESP_LOGE(TAG_CONFIG, "Error retrieving value for key: %s", strKeyArray[i]);
                    iRet = -1;
                }
                else
                {
                    ESP_LOGI(TAG_CONFIG, "Key: %s Value: %s", strKeyArray[i], ptrTmpKeyValueBuffer);
                    switch(i)
                    {
                        case 0: // Hostname
                            stUSerConfig.strHostname[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.strHostname, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.strHostname) - 1); // Copy value to user config
                            break;
                        case 1: // WifiSsid
                            stUSerConfig.strWifiSsid[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.strWifiSsid, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.strWifiSsid) - 1); // Copy value to user config
                            break;
                        case 2: // WifiPass
                            stUSerConfig.strWifiPassword[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.strWifiPassword, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.strWifiPassword) - 1); // Copy value to user config
                            break;
                        case 3: // IpAddress
                            stUSerConfig.stNetworkConfig.strIpAddr[0] = '\0'; // Ensure the string is null-terminated      
                            strncpy(stUSerConfig.stNetworkConfig.strIpAddr, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stNetworkConfig.strIpAddr) - 1); // Copy value to user config
                            break;
                        case 4: // Gateway
                            stUSerConfig.stNetworkConfig.strGateway[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stNetworkConfig.strGateway, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stNetworkConfig.strGateway) - 1); // Copy value to user config
                            break;
                        case 5: // Netmask
                            stUSerConfig.stNetworkConfig.strNetMAsk[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stNetworkConfig.strNetMAsk, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stNetworkConfig.strNetMAsk) - 1); // Copy value to user config
                            break;
                        case 6: // Static/Dynamic IP
                            if(strncmp(ptrTmpKeyValueBuffer, "Static", 6) == 0)
                            {
                                stUSerConfig.stNetworkConfig.eStaticDynamic = STATIC_IP;
                            }
                            else if(strncmp(ptrTmpKeyValueBuffer, "Dynamic", 7) == 0)
                            {
                                stUSerConfig.stNetworkConfig.eStaticDynamic = DYNAMIC_IP;
                            }
                            else
                            {
                                ESP_LOGE(TAG_CONFIG, "Invalid value for Static/Dynamic IP: %s", ptrTmpKeyValueBuffer);
                                iRet = -1;
                            }       
                            break;
                        case 7: // RTC Manual/Auto
                            if(strncmp(ptrTmpKeyValueBuffer, "RTC", 6) == 0)
                            {
                                stUSerConfig.eRtcManualAuto = RTC_MANUAL;
                            }
                            else if(strncmp(ptrTmpKeyValueBuffer, "NTP", 3) == 0)
                            {
                                stUSerConfig.eRtcManualAuto = RTC_FROM_NTP_SERVER;
                            }
                            else
                            {
                                ESP_LOGE(TAG_CONFIG, "Invalid value for RTC Manual/Auto: %s", ptrTmpKeyValueBuffer);
                                iRet = -1;
                            }       
                            break;
                        case 8: // NTP Server
                            stUSerConfig.strNtpServer[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.strNtpServer, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.strNtpServer) - 1); // Copy value to user config
                            break;
                        case 9: // Weather status
                            if(strncmp(ptrTmpKeyValueBuffer, "Enabled", 7) == 0)
                            {
                                stUSerConfig.stWeatherConfig.bWeatherStatus = true;
                            }
                            else if(strncmp(ptrTmpKeyValueBuffer, "Disabled", 8) == 0)
                            {
                                stUSerConfig.stWeatherConfig.bWeatherStatus = false;
                            }
                            else
                            {
                                ESP_LOGE(TAG_CONFIG, "Invalid value for Weather Status: %s", ptrTmpKeyValueBuffer);
                                iRet = -1;
                            }
                            break;                             
                        case 10: // Weather Server
                            stUSerConfig.stWeatherConfig.strWeatherServer[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stWeatherConfig.strWeatherServer, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stWeatherConfig.strWeatherServer) - 1); // Copy value to user config
                            break;  
                        case 11: // Weather API Key
                            stUSerConfig.stWeatherConfig.strWeatherApiKey[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stWeatherConfig.strWeatherApiKey, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stWeatherConfig.strWeatherApiKey) - 1); // Copy value to user config
                            break; 
                        case 12: // MQTT Status
                            if(strncmp(ptrTmpKeyValueBuffer, "Enabled", 7) == 0)
                            {
                                stUSerConfig.stMqttConfig.bMqttEnable = true;
                            }
                            else if(strncmp(ptrTmpKeyValueBuffer, "Disabled", 8) == 0)
                            {
                                stUSerConfig.stMqttConfig.bMqttEnable = false;
                            }
                            else
                            {
                                ESP_LOGE(TAG_CONFIG, "Invalid value for MQTT Status: %s", ptrTmpKeyValueBuffer);
                                iRet = -1;
                            }
                            break;
                        case 13: // MQTT Server
                            stUSerConfig.stMqttConfig.strMqttBrokerIpAddr[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stMqttConfig.strMqttBrokerIpAddr, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stMqttConfig.strMqttBrokerIpAddr) - 1); // Copy value to user config
                            break;  
                        case 14: // MQTT Port
                            stUSerConfig.stMqttConfig.u16MqttBrokerPort = (uint16_t)strtoul(ptrTmpKeyValueBuffer, NULL, 10);
                            break;
                        case 15: // MQTT Username
                            stUSerConfig.stMqttConfig.strMqttUserID[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stMqttConfig.strMqttUserID, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stMqttConfig.strMqttUserID) - 1); // Copy value to user config
                            break;
                        case 16: // MQTT Password
                            stUSerConfig.stMqttConfig.strMqttPassword[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stMqttConfig.strMqttPassword, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stMqttConfig.strMqttPassword) - 1); // Copy value to user config
                            break;
                        case 17: // MQTT Client ID
                            stUSerConfig.stMqttConfig.strMqttClientID[0] = '\0'; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stMqttConfig.strMqttClientID, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stMqttConfig.strMqttClientID) - 1); // Copy value to user config
                            break;
                        case 18: // MQTT Topic
                            stUSerConfig.stMqttConfig.strMqttTopic[0] = '\0 '; // Ensure the string is null-terminated
                            strncpy(stUSerConfig.stMqttConfig.strMqttTopic, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stMqttConfig.strMqttTopic) - 1); // Copy value to user config
                            break;
                        case 19: // MQTT Subscribe
                            stUSerConfig.stMqttConfig.strMqttSubscribe[0] = '\0'; // Ensure the string is null-terminated       
                            strncpy(stUSerConfig.stMqttConfig.strMqttSubscribe, ptrTmpKeyValueBuffer, sizeof(stUSerConfig.stMqttConfig.strMqttSubscribe) - 1); // Copy value to user config
                            break;
                        default:
                            ESP_LOGW(TAG_CONFIG, "Unhandled key index: %d", i);
                            break;
                    }
                    ESP_LOGI(TAG_CONFIG, "Key: %s | Value: %s Length: %zu", strKeyArray[i], ptrTmpKeyValueBuffer, iKeyValueLength);
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
        MsgConfigBox("Error opening configuration file");
        //MsgConfigBox();
    }
    else
    {
        ESP_LOGI(TAG_CONFIG, "Configuration file config.txt.");
        vGetConfig(&objUserConfig);
        create_progress_bar();                       
        
        nvs_set_str(writeHandle, "Hostname", objUserConfig.strHostname);
        update_bar(1); 
        // Forza LVGL a processare il disegno (solo se non usi i task)
        lv_timer_handler(); 
        close_pupup_bar();

        nvs_set_str(writeHandle, "WifiSsid", objUserConfig.strWifiSsid);
        nvs_set_str(writeHandle, "WifiPass", objUserConfig.strWifiPassword);

        nvs_set_str(writeHandle, "Ipaddress", objUserConfig.stNetworkConfig.strIpAddr);
        nvs_set_str(writeHandle, "Gateway", objUserConfig.stNetworkConfig.strGateway);
        nvs_set_str(writeHandle, "Netmask", objUserConfig.stNetworkConfig.strNetMAsk);

        // Static IP represented as 0, Dynamic IP represented as 1
        nvs_set_str(writeHandle, "StaticDinamicIP", objUserConfig.stNetworkConfig.eStaticDynamic == STATIC_IP ? "Static" : "Dynamic");
        
        // Clock Settings
        if(objUserConfig.eRtcManualAuto == RTC_FROM_NTP_SERVER)
        {
            // NTP Auto Clock Settings
            nvs_set_str(writeHandle, "RtcManualAuto", "NTP");
            nvs_set_str(writeHandle, "NtpServer", objUserConfig.strNtpServer);
        }
        else 
        {
            nvs_set_str(writeHandle, "RtcManualAuto","RTC"); 
            nvs_set_str(writeHandle, "NtpServer", " "); // Clear NTP server value if manual clock is selected
        }        
        // Weather Server Settings
        if(objUserConfig.stWeatherConfig.bWeatherStatus)
        {
            nvs_set_str(writeHandle, "WeatherStatus", "Enabled"); 
        }
        else
        {
            // Weather Server Enabled
            nvs_set_str(writeHandle, "WeatherStatus", "Disabled"); 
        }
        nvs_set_str(writeHandle, "WeatherServer", objUserConfig.stWeatherConfig.strWeatherServer);
        nvs_set_str(writeHandle, "WeatherApiKey", objUserConfig.stWeatherConfig.strWeatherApiKey);
     
        // Mqtt Home Assistant Settings        
        if(objUserConfig.stMqttConfig.bMqttEnable)
        {
            // Mqtt Protocol Enabled
            nvs_set_str(writeHandle, "MqttStatus", "Enabled"); 
        }
        else
        {
            nvs_set_str(writeHandle, "MqttStatus", "Disabled"); 
        } 
        char ptrTmpBuffer[6]; // Temporary buffer to hold string representation of port number
        sprintf(ptrTmpBuffer, "%d", objUserConfig.stMqttConfig.u16MqttBrokerPort); // Convert port number to string for storage

        nvs_set_str(writeHandle, "MqttServer"   , objUserConfig.stMqttConfig.strMqttBrokerIpAddr);
        nvs_set_str(writeHandle, "MqttPort"     , ptrTmpBuffer);
        nvs_set_str(writeHandle, "MqttUsername" , objUserConfig.stMqttConfig.strMqttUserID);
        nvs_set_str(writeHandle, "MqttPassword" , objUserConfig.stMqttConfig.strMqttPassword);       
        nvs_set_str(writeHandle, "MqttClientId" , objUserConfig.stMqttConfig.strMqttClientID);    
        nvs_set_str(writeHandle, "MqttTopic"    , objUserConfig.stMqttConfig.strMqttTopic);
        nvs_set_str(writeHandle, "MqttSubscribe", objUserConfig.stMqttConfig.strMqttSubscribe);

        nvs_commit(writeHandle); // Commit the changes to ensure they are saved
        nvs_close(writeHandle);  // Close the handle after writing              
    }
}

static void erase_msgbox_event_cb(lv_event_t * e) 
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    const char * btn_txt = lv_msgbox_get_active_btn_text(obj);

    if(strcmp(btn_txt, "OK") == 0) 
    {
        nvs_flash_erase_partition("nvs"); // remove NVS partition to ensure clean state before writing new config data
        nvs_flash_init(); // Reinizializza dopo l'erase                
    }

    // Chiudi la MessageBox in ogni caso (sia OK che Close)
    lv_msgbox_close(obj);
}

void create_confirm_msgbox(void) {
    static const char * btns[] = {"OK", "Close", ""}; // Testi dei pulsanti

    lv_obj_t * mbox = lv_msgbox_create(NULL, "ATTENZIONE", 
                                      "Sei sicuro di voler cancellare la Flash?\nTutti i dati andranno persi.", 
                                      btns, true);
    
    lv_obj_add_event_cb(mbox, erase_msgbox_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox);
}


void action_btn_erase_flash(lv_event_t *e) 
{
    ESP_LOGE(TAG_CONFIG, "Erasing Configuration File from NVS");
    create_confirm_msgbox();

    //nvs_flash_erase_partition("nvs"); // remove NVS partition to ensure clean state before writing new config data
    //nvs_flash_init();     
    //MsgInfoBox("Configuration File Erased");

   // MsgConfigBox();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// Configuration Table ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_tab_main_config(lv_event_t *e) 
{
    ESP_LOGI(TAG_CONFIG, "########################################### TAB CONFIGURATION ###########################################");
    ESP_LOGI(TAG_CONFIG, "Hostname       : %s", stUSerConfig.strHostname);
    ESP_LOGI(TAG_CONFIG, "WifiSsid       : %s", stUSerConfig.strWifiSsid);
    ESP_LOGI(TAG_CONFIG, "WifiPass       : %s", stUSerConfig.strWifiPassword);

    ESP_LOGI(TAG_CONFIG, "Ipaddress      : %s", stUSerConfig.stNetworkConfig.strIpAddr);                
    ESP_LOGI(TAG_CONFIG, "Gateway        : %s", stUSerConfig.stNetworkConfig.strGateway);
    ESP_LOGI(TAG_CONFIG, "NetMask        : %s", stUSerConfig.stNetworkConfig.strNetMAsk);
    if(stUSerConfig.stNetworkConfig.eStaticDynamic == STATIC_IP)
    {
        ESP_LOGI(TAG_CONFIG, "StaticDinamicIP: Static IP");
    }
    else
    {
        ESP_LOGI(TAG_CONFIG, "StaticDinamicIP: Dynamic IP");
    }
    if(stUSerConfig.eRtcManualAuto == RTC_MANUAL)
    {
        ESP_LOGI(TAG_CONFIG, "RtcManualAuto  : RTC");
    }
    else
    {
        ESP_LOGI(TAG_CONFIG, "RtcManualAuto  : NTP Server");                    
    }
    ESP_LOGI(TAG_CONFIG, "NtpServer      : %s", stUSerConfig.strNtpServer);
    ESP_LOGI(TAG_CONFIG, "Weather Status : %s", stUSerConfig.stWeatherConfig.bWeatherStatus == true ? "Enabled" : "Disabled");
    ESP_LOGI(TAG_CONFIG, "WeatherServer  : %s", stUSerConfig.stWeatherConfig.strWeatherServer);
    ESP_LOGI(TAG_CONFIG, "WeatherApiKey  : %s", stUSerConfig.stWeatherConfig.strWeatherApiKey);                    
    ESP_LOGI(TAG_CONFIG, "MqttStatus     : %s", stUSerConfig.stMqttConfig.bMqttEnable == true ? "Enabled" : "Disabled");       
    ESP_LOGI(TAG_CONFIG, "MqttServer     : %s", stUSerConfig.stMqttConfig.strMqttBrokerIpAddr);                
    ESP_LOGI(TAG_CONFIG, "MqttPort       : %d", stUSerConfig.stMqttConfig.u16MqttBrokerPort);                
    ESP_LOGI(TAG_CONFIG, "MqttUsername   : %s", stUSerConfig.stMqttConfig.strMqttUserID);
    ESP_LOGI(TAG_CONFIG, "MqttPassword   : %s", stUSerConfig.stMqttConfig.strMqttPassword);
    ESP_LOGI(TAG_CONFIG, "MqttClientId   : %s", stUSerConfig.stMqttConfig.strMqttClientID);                                        
    ESP_LOGI(TAG_CONFIG, "MqttTopic      : %s", stUSerConfig.stMqttConfig.strMqttTopic);        
    ESP_LOGI(TAG_CONFIG, "MqttSubscribe  : %s", stUSerConfig.stMqttConfig.strMqttSubscribe);
    ESP_LOGI(TAG_CONFIG, "#########################################################################################################");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Tab WiFi
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_tab_wifi_config(lv_event_t *e) 
{    
    objects_t objs = objects;   
    
    // Get Hostname
    if (strlen(stUSerConfig.strHostname) > 0)
    {
        lv_obj_t *ta = objs.txt_tab_wifi_hostname;
        lv_textarea_set_text(ta, stUSerConfig.strHostname);
    }
    // Get Wi-Fi SSID
    if (strlen(stUSerConfig.strWifiSsid) > 0)
    {
        lv_obj_t *ta = objs.drp_tab_wifi_ssid;
        lv_dropdown_clear_options(ta);
        lv_dropdown_add_option(ta, stUSerConfig.strWifiSsid, 0); // 0 è l'indice (posizione)
        ESP_LOGI(TAG_CONFIG, "WifiSsid: %s", stUSerConfig.strWifiSsid);
    }
    // Get Wi-Fi Password
    if (strlen(stUSerConfig.strWifiPassword) > 0)
    {
        lv_obj_t *ta = objs.txt_tab_wifi_password;       
        //lv_textarea_set_password_mode(ta, true);
        lv_textarea_set_text(ta, stUSerConfig.strWifiPassword);
        ESP_LOGI(TAG_CONFIG, "WifiPassword: %s", stUSerConfig.strWifiPassword);
    }
}

void action_scann_wifi_network(lv_event_t *e) 
{
    lv_event_code_t code     = lv_event_get_code(e);
    lv_obj_t *btn_WifiScan   = lv_event_get_target(e);   
    objects_t objs           = objects;  
    lv_obj_t *list_wifi_ssid = objs.drp_tab_wifi_ssid;
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

void action_tab_wifi_txt_hostname_cb(lv_event_t *e) 
{
    objects_t objs    = objects;
    kb = (lv_keyboard_t *)objs.kek_keyboard;
   
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

void action_txt_tab_wifi_password(lv_event_t *e) 
{      
    objects_t objs    = objects;
    /*lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;
   
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

void action_drp_tab_wifi_ssid(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj       = lv_event_get_target(e);    
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        ESP_LOGI(TAG_CONFIG, "%s ", wifi_scann_list[lv_dropdown_get_selected(obj)].ssid);  // Log RSSI (signal strength)
        memcpy(stUSerConfig.strWifiSsid, wifi_scann_list[lv_dropdown_get_selected(obj)].ssid, sizeof(stUSerConfig.strWifiSsid));
    }    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Tab Network
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_tab_network_config(lv_event_t *e) 
{
    objects_t objs = objects;
    // Get IP Address
    if (strlen(stUSerConfig.stNetworkConfig.strIpAddr) > 0)
    {
        lv_obj_t *ta = objs.txt_tab_net_ipaddress;
        lv_textarea_set_text(ta, stUSerConfig.stNetworkConfig.strIpAddr);
    }
    // Get Gateway
    if (strlen(stUSerConfig.stNetworkConfig.strGateway) > 0)
    {
        lv_obj_t *ta = objs.txt_tab_net_gateway;
        lv_textarea_set_text(ta, stUSerConfig.stNetworkConfig.strGateway);
    }
    // Get Netmask
    if (strlen(stUSerConfig.stNetworkConfig.strNetMAsk) > 0)
    {
        lv_obj_t *ta = objs.txt_tab_net_netmask;
        lv_textarea_set_text(ta, stUSerConfig.stNetworkConfig.strNetMAsk);
    }
    // Get Static/Dynamic IP
    lv_obj_t *sw = objs.sw_tab_net_static_ip_dynamic_ip;
    if(stUSerConfig.stNetworkConfig.eStaticDynamic == STATIC_IP)
    {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
        lv_obj_clear_state(objs.txt_tab_net_ipaddress, LV_STATE_DISABLED);
        lv_obj_clear_state(objs.txt_tab_net_gateway, LV_STATE_DISABLED);
        lv_obj_clear_state(objs.txt_tab_net_netmask, LV_STATE_DISABLED);
    }
    else
    {        
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
        disable_net_config(NULL);              
    }
}

void action_network_config(lv_event_t *e) 
{
    objects_t objs    = objects;
    /*lv_keyboard_t **/kb = (lv_keyboard_t *)objs.kek_keyboard;
    uint8_t userData  = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_BOTTOM_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1);

        //lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_NUMBER ); // Set keyboard to number mode for IP address input              
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        switch(userData)
        {
            case 1:
                // Ip Address
                memcpy(stUSerConfig.stNetworkConfig.strIpAddr, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strIpAddr));
                ESP_LOGI(TAG_CONFIG, "Click On IP Addr Textbox ");
                break;
            case 2:
                memcpy(stUSerConfig.stNetworkConfig.strNetMAsk, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strNetMAsk));
                ESP_LOGI(TAG_CONFIG, "Click On NetMask Textbox ");
                break;
            case 3:
                memcpy(stUSerConfig.stNetworkConfig.strGateway, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strGateway));
                ESP_LOGI(TAG_CONFIG, "Click On Gateway Textbox ");
                break;

            default:            
                break;
        }       
        ESP_LOGI(TAG_CONFIG, "Defocus On Network Config Textbox ");
    }
}

void action_sw_static_or_dynamic_ip(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        ESP_LOGI(TAG_CONFIG, "State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        if(lv_obj_has_state(obj, LV_STATE_CHECKED) == true) 
        {
            // Static  IP is selected
            tab_enable_net_config(NULL);
         
        }
        else
        {
            // DHCP is selected
            tab_disable_net_config(NULL);            
        }
        stUSerConfig.stNetworkConfig.eStaticDynamic = lv_obj_has_state(obj, LV_STATE_CHECKED) ? STATIC_IP : DYNAMIC_IP;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Tab Time
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void action_tab_time_config(lv_event_t *e) 
{
    struct tm currentTime;
    objects_t objs = objects;
    lv_obj_t *dropdown = objs.drop_day;
    uint8_t id = 0; 

    // Get RTC Manual/NTP Server
    lv_obj_t *sw = objs.sw_tab_time_config_rtc_or_ntp;
    if(stUSerConfig.eRtcManualAuto == RTC_MANUAL)
    {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
        // If NTP Server is selected, hide the date/time dropdowns and set RTC from NTP server
        lv_obj_add_flag(objs.drop_tab_time_config_day, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objs.drop_tab_time_config_month, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objs.drop_tab_time_config_year, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objs.drop_tab_time_config_hour, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objs.drop_tab_time_config_minute, LV_OBJ_FLAG_HIDDEN);
        // Disable the "Set Clock" button since we will get time from NTP server
        lv_obj_add_flag(objs.btn_tab_time_config_set_clock, LV_OBJ_FLAG_HIDDEN);        
    }
    // Get NTP Server
    if (strlen(stUSerConfig.strNtpServer) > 0)
    {
        lv_obj_t *ta = objs.tab_time_confiig_txt_ntp_server;
        lv_textarea_set_text(ta, stUSerConfig.strNtpServer);
    }

    if(stUSerConfig.stWeatherConfig.bWeatherStatus == true)
    {
        // enable weather Server/Key
        enable_netweather_config();
        lv_obj_t *sw = objs.tab_time_config_sw_wheater;
        lv_obj_add_state(sw, LV_STATE_CHECKED);
         ESP_LOGE(TAG_CONFIG, "stUSerConfig.stWeatherConfig.bWeatherStatus = true");
    }
    else
    {
        lv_obj_t *sw = objs.sw_wheater;
        disable_netweather_config();
        
        // disable weather Server/Key
        ///lv_obj_clear_state(objs.combo_weather_server, LV_STATE_DISABLED);
        //lv_obj_clear_state(objs.txt_weather_key, LV_STATE_DISABLED);
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
        ESP_LOGE(TAG_CONFIG, "stUSerConfig.stWeatherConfig.bWeatherStatus = false");
    }
    // Get Weather Server
    if (strlen(stUSerConfig.stWeatherConfig.strWeatherServer) > 0)
    {
        lv_obj_t *ta = objs.tab_time_combo_weather_server;
        lv_textarea_set_text(ta, stUSerConfig.stWeatherConfig.strWeatherServer);
    }
    // Get Weather API Key
    if (strlen(stUSerConfig.stWeatherConfig.strWeatherApiKey) > 0)
    {
        lv_obj_t *ta = objs.tab_time_txt_weather_key;
        lv_textarea_set_text(ta, stUSerConfig.stWeatherConfig.strWeatherApiKey);
    }
   
    // Get current time from RTC and populate the dropdowns
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


void action_weather_enable_disable(lv_event_t *e) 
{
    // TODO: Implement action sw_wheater here
    if (e == NULL) 
    {
        ESP_LOGE(TAG_CONFIG, "Event pointer is NULL");
        return;
    }
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj        = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) 
    {
        bool isChecked = lv_obj_has_state(obj, LV_STATE_CHECKED);
        stUSerConfig.stWeatherConfig.bWeatherStatus = isChecked;
        if(isChecked) 
        {
            // Enabled             
            tab_enable_netweather_config();
        } 
        else 
        {
            // Disabled
             tab_disable_netweather_config();
        }
        ESP_LOGI(TAG_CONFIG, "Weather Status: %s", isChecked ? "Enabled" : "Disabled");
    }
}

void action_sw_cmb_weather_server_selected(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj       = lv_event_get_target(e);    
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        // 1. Ottieni l'indice della voce selezionata (0, 1, 2...)
        uint16_t sel = lv_dropdown_get_selected(obj);

        // 2. Ottieni il testo della voce selezionata
        char buf[sizeof(stUSerConfig.stWeatherConfig.strWeatherServer)];
        lv_dropdown_get_selected_str(obj, buf, sizeof(buf));

        ESP_LOGI(TAG_CONFIG, "Selected %s ", buf);
        memcpy(stUSerConfig.stWeatherConfig.strWeatherServer,buf, sizeof(stUSerConfig.stWeatherConfig.strWeatherServer));
    } 
}

void action_tab_txt_weather_key(lv_event_t *e) 
{       
    objects_t objs    = objects;
    kb = (lv_keyboard_t *)objs.kek_keyboard;
   
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_obj_move_foreground(kb); 
        lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_TEXT_LOWER ); // Set keyboard to number mode for IP address input  
        ESP_LOGI(TAG_CONFIG, "Click On Weather Key ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        memcpy(stUSerConfig.stWeatherConfig.strWeatherApiKey, lv_textarea_get_text(ta), sizeof(stUSerConfig.stWeatherConfig.strWeatherApiKey));        
        ESP_LOGI(TAG_CONFIG, "Defocus On Weather Key ");
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Tab Home Assistant
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_tab_ha_config(lv_event_t *e)
{
    objects_t objs = objects;
    if(stUSerConfig.stMqttConfig.bMqttEnable == true)
    {
        enable_mqtt_config();
        lv_obj_t *sw = objs.tab_ha_sw_mqtt_status;
        lv_obj_add_state(sw, LV_STATE_CHECKED);     
    }
    else
    {
        lv_obj_t *sw = objs.tab_ha_sw_mqtt_status;
        lv_obj_clear_state(sw, LV_STATE_CHECKED);

    }
    if(stUSerConfig.stMqttConfig.bMqttEnable == true)
    {
        if (strlen(stUSerConfig.stMqttConfig.strMqttBrokerIpAddr) > 0)
        {
            lv_obj_t *ta = objs.tab_ha_txt_broker_ip;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttBrokerIpAddr);        
        }   
        uint8_t u8MacAdd[6]; 
        if(get_mac_address(u8MacAdd) == 0)
        {
            lv_obj_t *ta = objs.tab_ha_txt_mqttclient_id;
            lv_textarea_set_text(ta, (char*)&u8MacAdd[0]);     
        }
        else
        {
            if (strlen(stUSerConfig.stMqttConfig.strMqttClientID) > 0)
            {
                lv_obj_t *ta = objs.tab_ha_txt_mqttclient_id;
                lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttClientID);  
            }
        }
        if(strlen(stUSerConfig.stMqttConfig.strMqttPassword)>0)
        {
            lv_obj_t *ta = objs.tab_ha_txt_mqttpassword;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttPassword);
        }
        if(strlen(stUSerConfig.stMqttConfig.strMqttUserID)>0)
        {
            lv_obj_t *ta = objs.tab_ha_txt_mqttuserid;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttUserID);
        }
        if(strlen(stUSerConfig.stMqttConfig.strMqttTopic)>0)
        {
            lv_obj_t *ta = objs.tab_ha_txt_mqtttopic;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttTopic);
        }
        if(strlen(stUSerConfig.stMqttConfig.strMqttSubscribe)>0)
        {
            lv_obj_t *ta = objs.tab_ha_txt_mqttsubscribe;
            lv_textarea_set_text(ta, stUSerConfig.stMqttConfig.strMqttSubscribe);
        }  
        lv_obj_t *ta = objs.tab_ha_txt_broker_port; 
        char ptrTmpBuffer[10]; // Temporary buffer to hold string representation of port number
        sprintf(ptrTmpBuffer, "%d", stUSerConfig.stMqttConfig.u16MqttBrokerPort); // Convert port number to string for storage        
        lv_textarea_set_text(ta, ptrTmpBuffer);             
    }
}

void action_sw_mqtt_protocol_on_off(lv_event_t *e) 
{
    if (e == NULL) {
        ESP_LOGE(TAG_CONFIG, "Event pointer is NULL");
        return;
    }
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj        = lv_event_get_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) 
    {
        bool isChecked = lv_obj_has_state(obj, LV_STATE_CHECKED);
        stUSerConfig.stMqttConfig.bMqttEnable = isChecked;
        if(isChecked) 
        {
            // Enabled             
            enable_mqtt_config();
        } 
        else 
        {
            // Disabled
             disable_mqtt_config();
        }
        ESP_LOGI(TAG_CONFIG, "MQTT Status: %s", isChecked ? "Enabled" : "Disabled");
    }
}

void action_tab_mqtt_broker_port(lv_event_t *e) 
{
    objects_t objs    = objects;
    kb = (lv_keyboard_t *)objs.kek_keyboard;
    uint8_t userData  = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);

    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map_number, kb_ctrl_number);
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1);
    }
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        stUSerConfig.stMqttConfig.u16MqttBrokerPort = atoi(lv_textarea_get_text(ta));        
        ESP_LOGI(TAG_CONFIG, "Defocus On Mqtt Broker Port ");
    }

}
void action_tab_mqtt_config(lv_event_t *e) 
{
    objects_t objs    = objects;
    kb = (lv_keyboard_t *)objs.kek_keyboard;
    uint8_t userData  = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);

    if(code == LV_EVENT_FOCUSED) 
    {
        if(userData != 0) 
        {
            lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN); // mostra la tastiera
            lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
            lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
            lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_USER_1);        
        }
        else // Broker IP
        {
            lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
            lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
            lv_obj_move_foreground(kb); 
            lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_TEXT_LOWER ); // Set keyboard to number mode for IP address input          
        }
        ESP_LOGI(TAG_CONFIG, "Click On Mqtt Config ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        switch(userData)
        {
            case 0:
                // BrokerIP
                memcpy(stUSerConfig.stMqttConfig.strMqttBrokerIpAddr, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttBrokerIpAddr));
                ESP_LOGI(TAG_CONFIG, "Click On Broker ");
                break;
                /*
            case 1:
                memcpy(stUSerConfig.stMqttConfig.u16MqttBrokerPort, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.u16MqttBrokerPort));
                ESP_LOGI(TAG_CONFIG, "Click On Broker Port ");
                break;*/
            case 2:
                memcpy(stUSerConfig.stMqttConfig.strMqttClientID, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttClientID));
                ESP_LOGI(TAG_CONFIG, "Click On Client ID ");
                break;
            case 3:
                memcpy(stUSerConfig.stMqttConfig.strMqttPassword, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttPassword));
                ESP_LOGI(TAG_CONFIG, "Click On Password ");
                break;
            case 4:
                memcpy(stUSerConfig.stMqttConfig.strMqttUserID, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttUserID));
                ESP_LOGI(TAG_CONFIG, "Click On User ID ");
                break;
            case 5:
                memcpy(stUSerConfig.stMqttConfig.strMqttTopic, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttTopic));
                ESP_LOGI(TAG_CONFIG, "Click On Topic ");
                break;                
            case 6:
                memcpy(stUSerConfig.stMqttConfig.strMqttSubscribe, lv_textarea_get_text(ta), sizeof(stUSerConfig.stMqttConfig.strMqttSubscribe));
                ESP_LOGI(TAG_CONFIG, "Click On Topic ");
                break;                                

            default:            
                break;
        }       
        ESP_LOGI(TAG_CONFIG, "Defocus On Network Config Textbox ");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Main Tab
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void action_save_config_to_nvs(lv_event_t *e)
{
    nvs_handle_t writeHandle;
    USER_CONFIG objUserConfig;

    nvs_flash_erase_partition("nvs"); // remove NVS partition to ensure clean state before writing new config data
    nvs_flash_init();

    // Try to open config file
    if(nvs_open("config.txt", NVS_READWRITE, &writeHandle) != ESP_OK)
    {
        ESP_LOGE(TAG_CONFIG, "Error opening config.txt ");
        MsgConfigBox("Error opening configuration file");
        //MsgConfigBox();
    }
    else
    {
        ESP_LOGI(TAG_CONFIG, "Configuration file config.txt.");
        vGetConfig(&objUserConfig);
        create_progress_bar();                       
        
        nvs_set_str(writeHandle, "Hostname", objUserConfig.strHostname);
        update_bar(1); 
        // Forza LVGL a processare il disegno (solo se non usi i task)
        lv_timer_handler(); 
        close_pupup_bar();

        nvs_set_str(writeHandle, "WifiSsid", objUserConfig.strWifiSsid);
        nvs_set_str(writeHandle, "WifiPass", objUserConfig.strWifiPassword);

        nvs_set_str(writeHandle, "Ipaddress", objUserConfig.stNetworkConfig.strIpAddr);
        nvs_set_str(writeHandle, "Gateway", objUserConfig.stNetworkConfig.strGateway);
        nvs_set_str(writeHandle, "Netmask", objUserConfig.stNetworkConfig.strNetMAsk);

        // Static IP represented as 0, Dynamic IP represented as 1
        nvs_set_str(writeHandle, "StaticDinamicIP", objUserConfig.stNetworkConfig.eStaticDynamic == STATIC_IP ? "Static" : "Dynamic");
        
        // Clock Settings
        if(objUserConfig.eRtcManualAuto == RTC_FROM_NTP_SERVER)
        {
            // NTP Auto Clock Settings
            nvs_set_str(writeHandle, "RtcManualAuto", "NTP");
            nvs_set_str(writeHandle, "NtpServer", objUserConfig.strNtpServer);
        }
        else 
        {
            nvs_set_str(writeHandle, "RtcManualAuto","RTC"); 
            nvs_set_str(writeHandle, "NtpServer", " "); // Clear NTP server value if manual clock is selected
        }        
        // Weather Server Settings
        if(objUserConfig.stWeatherConfig.bWeatherStatus)
        {
            nvs_set_str(writeHandle, "WeatherStatus", "Enabled"); 
        }
        else
        {
            // Weather Server Enabled
            nvs_set_str(writeHandle, "WeatherStatus", "Disabled"); 
        }
        nvs_set_str(writeHandle, "WeatherServer", objUserConfig.stWeatherConfig.strWeatherServer);
        nvs_set_str(writeHandle, "WeatherApiKey", objUserConfig.stWeatherConfig.strWeatherApiKey);
     
        // Mqtt Home Assistant Settings        
        if(objUserConfig.stMqttConfig.bMqttEnable)
        {
            // Mqtt Protocol Enabled
            nvs_set_str(writeHandle, "MqttStatus", "Enabled"); 
        }
        else
        {
            nvs_set_str(writeHandle, "MqttStatus", "Disabled"); 
        } 
        char ptrTmpBuffer[6]; // Temporary buffer to hold string representation of port number
        sprintf(ptrTmpBuffer, "%d", objUserConfig.stMqttConfig.u16MqttBrokerPort); // Convert port number to string for storage

        nvs_set_str(writeHandle, "MqttServer"   , objUserConfig.stMqttConfig.strMqttBrokerIpAddr);
        nvs_set_str(writeHandle, "MqttPort"     , ptrTmpBuffer);
        nvs_set_str(writeHandle, "MqttUsername" , objUserConfig.stMqttConfig.strMqttUserID);
        nvs_set_str(writeHandle, "MqttPassword" , objUserConfig.stMqttConfig.strMqttPassword);       
        nvs_set_str(writeHandle, "MqttClientId" , objUserConfig.stMqttConfig.strMqttClientID);    
        nvs_set_str(writeHandle, "MqttTopic"    , objUserConfig.stMqttConfig.strMqttTopic);
        nvs_set_str(writeHandle, "MqttSubscribe", objUserConfig.stMqttConfig.strMqttSubscribe);

        nvs_commit(writeHandle); // Commit the changes to ensure they are saved
        nvs_close(writeHandle);  // Close the handle after writing              
    }
}

void action_erase_nvs_flash(lv_event_t *e) 
{
    ESP_LOGE(TAG_CONFIG, "Erasing Configuration File from NVS");
    create_confirm_msgbox();

    //nvs_flash_erase_partition("nvs"); // remove NVS partition to ensure clean state before writing new config data
    //nvs_flash_init();     
    //MsgInfoBox("Configuration File Erased");

   // MsgConfigBox();
}

void action_restart_micro(lv_event_t *e) 
{
    esp_restart();
}


