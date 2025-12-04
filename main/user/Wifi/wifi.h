#ifndef _WIFI_
#define _WIFI_

#include <string.h>
#include <time.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"  // Include ESP32 Wi-Fi driver functions
#include "esp_log.h"   // Include ESP32 logging functions

typedef enum _RTC_MANUAL_AUTO
{
    RTC_MANUAL = 0,
    RTC_FROM_NTP_SERVER
} RTC_MANUAL_AUTO;

typedef enum __DYNAMIC_IP_STATIC_IP
{
    STATIC_IP = 0,
    DYNAMIC_IP
} RTC_MANUAL_AUTO;

typedef struct _NETWORK_CONFIG
{
    char strIpAddr[16];
    char strNetMAsk[16];
    char strGateway[16];
    char strDns[16];
    RTC_MANUAL_AUTO eStaticDynamic;    
} NETWORK_CONFIG;

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
USER_CONFIG stUSerConfig;

extern wifi_ap_record_t ap_info[];  // Declare an array to store the AP records

extern esp_netif_ip_info_t ip_info; // Stores IP information

#define DEFAULT_SCAN_LIST_SIZE 15 // Max number of APs to store (0 to 20)

int8_t wifi_init(void);  // Function to initialize Wi-Fi
void wifi_scan(void);  // Function to scan available Wi-Fi networks

// Initialize Wi-Fi in STA mode with SSID, password, and auth mode
void wifi_sta_init(uint8_t *ssid, uint8_t *pwd, wifi_auth_mode_t authmode);
  
#endif
