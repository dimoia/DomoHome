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

/// @brief Enumeration for RTC Manual or NTP Server Configuration
typedef enum _RTC_MANUAL_AUTO
{
    RTC_MANUAL = 0,
    RTC_FROM_NTP_SERVER
} RTC_MANUAL_AUTO;

/// @brief Enumeration for Dynamic or Static IP Configuration
typedef enum _DYNAMIC_IP_STATIC_IP
{
    STATIC_IP = 0,
    DYNAMIC_IP
} DYNAMIC_IP_STATIC_IP;

/// @brief Structure to hold Network Configuration
typedef struct _NETWORK_CONFIG
{
    char strIpAddr[16];
    char strNetMAsk[16];
    char strGateway[16];
    char strDns[16];
    RTC_MANUAL_AUTO eStaticDynamic;    
} NETWORK_CONFIG;

/// @brief Structure to hold User Configuration
typedef struct _USER_CONFIG
{
    char strHostname[32];
    char strWifiSsid[32];
    char strWifiPassword[64];
    struct tm stRtcClock;
    char strNtpServer[64];
    RTC_MANUAL_AUTO eRtcManualAuto;
    NETWORK_CONFIG stNetworkConfig;
    
} USER_CONFIG;

/// @brief External declarations
extern wifi_ap_record_t    ap_info[];  // Declare an array to store the AP records
extern esp_netif_ip_info_t ip_info;    // Stores IP information

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize User Configuration  
/// @param  none
/// @return -1 on error, 0 on success 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int8_t iConfigInit(void); 

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Get Default User Configuration
/// @param  pUserConfig Pointer to USER_CONFIG structure to be populated
/// @return none
//////////////////////////////////////////////////////////////////////////////////////////////////////////
void vGetDefaultConfig(USER_CONFIG *pUserConfig);

// Initialize Wi-Fi in STA mode with SSID, password, and auth mode
//void wifi_sta_init(uint8_t *ssid, uint8_t *pwd, wifi_auth_mode_t authmode);
  

#endif //_CONFIG_H_