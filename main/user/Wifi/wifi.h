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
#include "config.h"



//extern wifi_ap_record_t    ap_info[];  // Declare an array to store the AP records
//extern esp_netif_ip_info_t ip_info;    // Stores IP information

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize Wi-Fi for STA (Station) and AP (Access Point) modes
/// @param  none
/// @return -1 on error, 0 on success 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//int8_t wifi_init(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Scan for available Wi-Fi Access Points (APs) and populate a dropdown list
/// @param none
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//void wifi_scan(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Scan for available Wi-Fi Access Points (APs) and populate a dropdown list
/// @param out_stWifiScannList Pointer to an array of wifi_ap_record_t to store scanned APs
/// @param u8MaxApCount Maximum number of APs that can be stored in out_stWifiScannList
/// @return -1 no Acces Point found, otherwise the number of AP found.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int iWifiScan(wifi_ap_record_t out_stWifiScannList[], uint8_t u8MaxApCount);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief  Initialize Wi-Fi in STA mode with SSID, password, and auth mode
/// @param  ssid Pointer to the SSID (network name) of the Wi-Fi network
/// @param  pwd Pointer to the password of the Wi-Fi network    
/// @param  authmode Authentication mode for the Wi-Fi network
/// @return -1 on error, 0 on success
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int iWifiConnectInStationMode(uint8_t *ssid, uint8_t *pwd, wifi_auth_mode_t authmode);
  
 void start_wifi();
 //void vGetDefaultConfig(USER_CONFIG *pUserConfig);
 void stop_wifi();


extern wifi_ap_record_t ap_info[];  // Declare an array to store the AP records

extern esp_netif_ip_info_t ip_info; // Stores IP information

#define DEFAULT_SCAN_LIST_SIZE 15 // Max number of APs to store (0 to 20)

void wifi_init(void);  // Function to initialize Wi-Fi
void wifi_scan(void);  // Function to scan available Wi-Fi networks

// Initialize Wi-Fi in STA mode with SSID, password, and auth mode
int8_t wifi_sta_init(uint8_t *ssid, uint8_t *pwd, wifi_auth_mode_t authmode);
int8_t wifi_wait_connect();

#endif
