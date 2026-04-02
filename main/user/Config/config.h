#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string.h>
#include <time.h>
#include "wifi.h"
#include "utils.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"  // Include ESP32 Wi-Fi driver functions
#include "esp_log.h"   // Include ESP32 logging functions
#include "lvgl_port.h" 
#include "lvgl.h"
#include "ui.h"

#define DEFAULT_SCAN_LIST_SIZE 15 // Max number of APs to store (0 to 20)

typedef enum _RTC_MANUAL_AUTO
{
    RTC_MANUAL = 0,
    RTC_FROM_NTP_SERVER
} RTC_MANUAL_AUTO;

typedef enum _DYNAMIC_IP_STATIC_IP
{
    STATIC_IP = 0,
    DYNAMIC_IP
} DYNAMIC_IP_STATIC_IP;

typedef struct _WEATHER_
{
    char strWeatherServer[64];
    char strWeatherApiKey[32];
} WEATHER;

typedef struct _MQTT_CONFIG_
{
    bool bMqttEnable;
    char strMqttBrokerIpAddr[16];
    uint16_t u16MqttBrokerPort;
    char strMqttClientID[7];
    char strMqttUserID[32];
    char strMqttPassword[32];
    char strMqttTopic[32];
    char strMqttSubscribe[32];
} MQTT_CONFIG;

typedef struct _NETWORK_CONFIG
{
    char strIpAddr[16];
    char strNetMAsk[16];
    char strGateway[16];
    char strDns[16];
    DYNAMIC_IP_STATIC_IP eStaticDynamic;    
} NETWORK_CONFIG;

typedef struct _USER_CONFIG_
{
    // HostName of the device
    char strHostname[16];

    // WiFi Configuration
    char strWifiSsid[32];
    char strWifiPassword[32];

    // NETWORK Configuration
    NETWORK_CONFIG stNetworkConfig;

    // Clock Configuration
    struct tm stRtcClock;

    // RTC Manual or NTP Server Configuration
    RTC_MANUAL_AUTO eRtcManualAuto;    
    char strNtpServer[32];
    
    WEATHER stWeatherConfig;    
    
    MQTT_CONFIG stMqttConfig;
    //bool WiFi_is_connected;
    
} USER_CONFIG;

typedef enum _CONFIG_STATUS_
{
    CONFIG,
    NOT_CONFIG,
} CONFIG_STATUS;

CONFIG_STATUS getConfigStatus(void);
void setConfigStatus(CONFIG_STATUS in_eConfigStatus);

/// @brief External declarations
extern wifi_ap_record_t    ap_info[];  // Declare an array to store the AP records
extern esp_netif_ip_info_t ip_info;    // Stores IP information

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Initialize User Configuration  
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void iConfigInit(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Get Default User Configuration
/// pUserConfig Pointer to a USER_CONFIG structure where the default configuration will be stored
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void vGetConfig(USER_CONFIG *pUserConfig);

// Initialize Wi-Fi in STA mode with SSID, password, and auth mode
//void wifi_sta_init(uint8_t *ssid, uint8_t *pwd, wifi_auth_mode_t authmode);
  

typedef enum _DEVICE_STATUS_
{
    DEVICE_STATUS_OK,
    DEVICE_STATUS_ERROR,
} DEVICE_STATUS;

typedef struct _DEVICE_CONFIG
{
    DEVICE_STATUS bBME280Status;    
} DEVICE_CONFIG;

void initDeviceConfig(DEVICE_CONFIG* out_ptrDeviceConfig);
void setBME280Status(DEVICE_CONFIG* in_ptrDeviceConfig, DEVICE_STATUS in_bStatus);
DEVICE_STATUS getBME280Status(const DEVICE_CONFIG* in_ptrDeviceConfig);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////// File System Interface Section
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_KEY_VALUE_PAIRS 22
/*
typedef struct _CONFIG_KEY_VALUE_PAIR_
{
    char* key;
    char* value;
} CONFIG_KEY_VALUE_PAIR; 

char *strKeyArray[] = 
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
*/
int8_t iDownloadConfigFileFromNVS(const char* in_ptrFilename);
int8_t iUploadConfigFileToNVS(const char* in_ptrFilename, const char* in_ptrKey[], const char* in_ptrValue[], size_t in_iNumKeyValuePairs);

/*
int8_t check_for_config_file(const char* in_ptrFilename);
int8_t get_value_size_by_key(const char* in_ptrFilename, const char* in_ptrKey, size_t* out_ptrValueSize);
int8_t get_value_by_key(const char* in_ptrFilename, const char* in_ptrKey,char* out_ptrValue, size_t in_ptrValueSize);
int8_t delete_file(const char* filename);
int8_t save_wifi_credentials(const char* ssid, const char* pass);
int8_t save_ntp_server(const char* ntp_server);
int8_t read_config(const char* in_ptrFilename,const char *in_ptrKey, char* out_ptrValue, size_t *out_ptrMaxLen);
*/


#endif //_CONFIG_H_