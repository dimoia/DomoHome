#include <string.h>
#include "wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "actions.h"
#include "lvgl_port.h" 
#include "lvgl.h"
#include "ui.h"
#include "pcf8523.h"
#include "nvs_flash.h"
#include "nvs.h"

#define DEFAULT_SCAN_LIST_SIZE 15 // Max number of APs to store (0 to 20)
static wifi_ap_record_t wifi_scann_list[DEFAULT_SCAN_LIST_SIZE];  // Array to store the AP records
static USER_CONFIG stUSerConfig;
TaskHandle_t wifi_TaskHandle;
const char *TAG_WIFI = "wifi";    // Tag for Station mode (Wi-Fi client mode)
esp_netif_ip_info_t ip_info; // Stores the IP information once connected to Wi-Fi
wifi_ap_record_t    ap_info[];  // Declare an array to store the AP records
/* FreeRTOS event group to signal when we are connected/disconnected */
static EventGroupHandle_t s_wifi_event_group;


static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) 
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *) event_data;
        ESP_LOGI(TAG_AP, "Station "MACSTR" joined, AID=%d", MAC2STR(event->mac), event->aid);
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) 
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *) event_data;
        ESP_LOGI(TAG_AP, "Station "MACSTR" left, AID=%d, reason:%d", MAC2STR(event->mac), event->aid, event->reason);
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
        ESP_LOGI(TAG_WIFI, "Station started");
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(TAG_WIFI, "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_ASSIGNED_IP_TO_CLIENT)
    {
        const ip_event_assigned_ip_to_client_t *e = (const ip_event_assigned_ip_to_client_t *)event_data;
        ESP_LOGI(TAG_AP, "Assigned IP to client: " IPSTR ", MAC=" MACSTR ", hostname='%s'", IP2STR(&e->ip), MAC2STR(e->mac), e->hostname);
    }
}

// Initialize Wi-Fi for STA (Station) and AP (Access Point) modes
int8_t wifi_init(void)
{    
    int8_t iRet = 0;

    /* Clear stUSerConfig*/
    memset(&stUSerConfig, 0, sizeof(USER_CONFIG));

    iRet = esp_netif_init();
    if (iRet != ESP_OK)
    {
        ESP_LOGE(TAG_WIFI, "esp_netif_init failed: %d", iRet);
        iRet = -1;
    } 
    else
    {
        iRet = esp_event_loop_create_default();
        if (iRet != ESP_OK)
        {
            ESP_LOGE(TAG_WIFI, "esp_event_loop_create_default failed: %d", iRet);
            iRet = -1;
        }
        else
        {
#if 0            
            //Initialize NVS
            esp_err_t ret = nvs_flash_init();
            if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
            {
                ESP_ERROR_CHECK(nvs_flash_erase());
                ret = nvs_flash_init();
            }
            ESP_ERROR_CHECK(ret); // Check for NVS initialization errors

            /* Register Event handler */
            ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    ESP_EVENT_ANY_ID,
                    &wifi_event_handler,
                    NULL,
                    NULL));
            ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                    IP_EVENT_STA_GOT_IP,
                    &wifi_event_handler,
                    NULL,
                    NULL));
            ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                    IP_EVENT_ASSIGNED_IP_TO_CLIENT,
                    &wifi_event_handler,
                    NULL,
                    NULL));

#endif
            wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
            iRet = esp_wifi_init(&cfg);
            if (iRet != ESP_OK)
            {
                ESP_LOGE(TAG_WIFI, "esp_wifi_init failed: %d", iRet);
                iRet = -1;
            }
            else
            {
                ESP_LOGI(TAG_WIFI, "Wi-Fi initialized successfully");
                iRet = 0;
            }
        }
    }   
    return iRet;
}

int iWifiScan(wifi_ap_record_t out_stWifiScannList[], uint8_t u8MaxApCount)
{    
    int iRet = 0;
    objects_t objs           = objects;  
    lv_obj_t *list_wifi_ssid = objs.drp_wifi_ssid;
    uint16_t number          = MIN(DEFAULT_SCAN_LIST_SIZE,u8MaxApCount);  // Maximum number of APs to be stored
    uint16_t ap_count        = 0;  // Variable to hold the actual number of APs found

    if(out_stWifiScannList == NULL) 
    {
        ESP_LOGE(TAG_WIFI, "wifi_scann_list is NULL");
        iRet = -1;
    }
    else
    {                

        // Set the WiFi operating mode as station, soft-AP, station+soft-AP or NAN. The default mode is station mode.
        if(ESP_OK !=  esp_wifi_set_mode(WIFI_MODE_STA))
        {
            ESP_LOGE(TAG_WIFI, "Failed to set WiFi mode to STA");
            iRet = -1;
        }
        else
        {
            ESP_LOGI(TAG_WIFI, "WiFi mode set to STA");

            /* Start WiFi according to current configuration If mode is WIFI_MODE_STA, it creates station control block and starts station 
               If mode is WIFI_MODE_AP, it creates soft-AP control block and starts soft-AP If mode is WIFI_MODE_APSTA, it creates soft-AP 
               and station control block and starts soft-AP and station If mode is WIFI_MODE_NAN, it creates NAN control block and starts NAN.*/
            if(ESP_OK != esp_wifi_start())
            {
                ESP_LOGE(TAG_WIFI, "Failed to start WiFi");
                iRet = -1;
            }
            else
            {
                ESP_LOGI(TAG_WIFI, "WiFi started successfully");

                // Scan all available APs.
                // The scan is blocking, the function will not return until the scan is done.
                if(ESP_OK !=  esp_wifi_scan_start(NULL, true))
                {
                    ESP_LOGE(TAG_WIFI, "Failed to scan WiFi scan");
                    iRet = -1;
                }
                else
                {
                    ESP_LOGI(TAG_WIFI, "WiFi scan started successfully");
                    ESP_LOGI(TAG_WIFI, "Max AP number wifi_scann_list can hold = %u", number);  // Log the max AP number that can be stored

                    // Get number of APs found in last scan.
                    if(ESP_OK != esp_wifi_scan_get_ap_num(&ap_count))
                    {
                        ESP_LOGE(TAG_WIFI, "Failed to get number of APs found in last scan");
                        iRet = -1;
                    }
                    else
                    {
                        memset(wifi_scann_list, 0, sizeof(wifi_scann_list));  // Clear the wifi_scann_list array

                        esp_wifi_scan_get_ap_records(&number, wifi_scann_list);  // Get the AP records into wifi_scann_list array
                        //ESP_LOGI(TAG_WIFI, "Number of APs found in last scan: %u", ap_count);
                        ESP_LOGI(TAG_WIFI, "Total APs scanned = %u, actual AP number wifi_scann_list holds = %u", ap_count, number);  // Log total and actual scanned APs

                        memcpy(out_stWifiScannList, wifi_scann_list, sizeof(wifi_ap_record_t) * number);
                                                
                        esp_wifi_stop(); // Stop WiFi to save power
                        iRet = number;
                    }
                }
            }           
        }       
    }
    return iRet;
}

void softap_set_dns_addr(esp_netif_t *esp_netif_sta)
{
    esp_netif_dns_info_t dns;
    esp_netif_get_dns_info(esp_netif_sta,ESP_NETIF_DNS_MAIN,&dns);
    uint8_t dhcps_offer_option = DHCPS_OFFER_DNS;
}

int iWifiConnectInStationMode(uint8_t *ssid, uint8_t *pwd, wifi_auth_mode_t authmode)
{
    int iRet = 0;
    wifi_config_t wifi_sta_config = {
        .sta = {
            .ssid               = ssid,
            .password           = pwd,
            .scan_method        = CONFIG_ESP_WIFI_AP_CHANNEL,
            .failure_retry_cnt  = CONFIG_ESP_MAXIMUM_STA_RETRY,
            /* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (password len => 8).
             * If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
             * to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
            * WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
             */
            .threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .sae_pwe_h2e        = WPA3_SAE_PWE_BOTH,
        },
    };
    if(ERR_OK != esp_wifi_set_config(WIFI_IF_STA, &wifi_sta_config))
    {
        ESP_LOGE(TAG_WIFI, "Failed to set WiFi mode to STA");
        iRet = -1;
    } 
    else
    {
        ESP_LOGI(TAG_WIFI, "WiFi mode set to STA");

        /* Start WiFi */
        if(ERR_OK != esp_wifi_start())
        {
            ESP_LOGE(TAG_WIFI, "Failed to start WiFi mode to STA");
            iRet = -1;
        } 
        else
        {
          
            ESP_LOGI(TAG_WIFI, "WiFi connected to AP successfully");
            /*
            * Wait until either the connection is established (WIFI_CONNECTED_BIT) or
            * connection failed for the maximum number of re-tries (WIFI_FAIL_BIT).
            * The bits are set by event_handler() (see above)
            */
            EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                                    WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                                    pdFALSE,
                                                    pdFALSE,
                                                    portMAX_DELAY);

            /* xEventGroupWaitBits() returns the bits before the call returned,
            * hence we can test which event actually happened. */
            if (bits & WIFI_CONNECTED_BIT)
            {
                ESP_LOGI(TAG_WIFI, "connected to ap SSID:%s password:%s", EXAMPLE_ESP_WIFI_STA_SSID, EXAMPLE_ESP_WIFI_STA_PASSWD);
                softap_set_dns_addr(esp_netif_sta);

                /* Set sta as the default interface */
                esp_netif_set_default_netif(esp_netif_sta);

            } 
            else if (bits & WIFI_FAIL_BIT) 
            {
                ESP_LOGI(TAG_WIFI, "Failed to connect to SSID:%s, password:%s", EXAMPLE_ESP_WIFI_STA_SSID, EXAMPLE_ESP_WIFI_STA_PASSWD);
                iRet = -1;
            } 
            else 
            {
                ESP_LOGE(TAG_WIFI, "UNEXPECTED EVENT");
                iRet = -1;
            }            
        }
        
    }      
    return iRet;
}
#if 0

// Function to wait for Wi-Fi connection and obtain IP address
void wifi_task(wifi_config_t *wifi_config)
{
    static int s_retry_num = 0;  // Counter to track the number of connection retries
    wifi_config_t sta_config;

    // Set the Wi-Fi configuration for the station (STA) interface
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, wifi_config));
    esp_netif_create_default_wifi_sta();  // Create the default network interface for Wi-Fi STA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));  // Set Wi-Fi mode to Station (STA) initially
    esp_wifi_start(); // Start the Wi-Fi with the new configuration
    ESP_ERROR_CHECK(esp_wifi_connect());  // Attempt to connect to the Wi-Fi AP

    // Get the current Wi-Fi station configuration (SSID, password, etc.)
    ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &sta_config));

    while (1)
    {
        // Get the network interface handle for the default Wi-Fi STA interface
        esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        if (netif)
        {
            // Get the IP information associated with the Wi-Fi STA interface
            esp_err_t ret = esp_netif_get_ip_info(netif, &ip_info);
            if (ret == ESP_OK && ip_info.ip.addr != 0) {
                // If successfully connected and an IP address is obtained
                ESP_LOGI("WiFi", "Connected with IP: " IPSTR, IP2STR(&ip_info.ip));
                ESP_LOGI(TAG_WIFI, "Connected to AP SSID:%s, password:%s", sta_config.sta.ssid, sta_config.sta.password);
                
                char ip[20];
                // Clear the top section of the screen to display connection info
                //Paint_ClearWindows(440, 160, 800, 185, WHITE);
                snprintf(ip, sizeof(ip), "IP " IPSTR, IP2STR(&ip_info.ip));  // Format the IP address
              //  Paint_DrawString_EN(440, 160, ip, &Font24, BLACK, WHITE);  // Display IP on the screen
              //  Paint_DrawString_EN(440, 200, (const char *)sta_config.sta.ssid, &Font24, BLACK, WHITE); // Display SSID
              //  Paint_DrawString_EN(440, 240, (const char *)sta_config.sta.password, &Font24, BLACK, WHITE); // Display password

                s_retry_num = 0;  // Reset retry counter on successful connection
                break;  // Exit the loop since the connection is successful
            } else {
                // Log the failure to connect or obtain an IP address
                ESP_LOGI(TAG_WIFI, "Failed to connect to the AP");

                // Retry connection if the retry counter is less than 5
                if (s_retry_num < 5)
                {
                    s_retry_num++;
                    ESP_LOGI(TAG_WIFI, "Retrying to connect to the AP");
                }
                else {
                    // Reset retry counter after 5 attempts and log the failure
                    s_retry_num = 0;

                    ESP_LOGI(TAG_WIFI, "Failed to connect to SSID:%s, password:%s",
                            sta_config.sta.ssid, sta_config.sta.password);
                    
                    // Clear the top section of the screen to display failure message
                 //   Paint_ClearWindows(440, 160, 800, 185, WHITE);
                 //   Paint_DrawString_EN(440, 160, "Failed to connect to the AP.", &Font24, BLACK, WHITE);        
                    break;  // Exit the loop after failed retries
                }

                // Wait for 1 second before retrying
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
        else 
        {
            // Log an error if the network interface handle is not found
            ESP_LOGE("WiFi", "Netif handle not found");
        }

        // Short delay (10ms) before checking the connection status again
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}



// Function to initialize Wi-Fi in Station mode (STA mode) and connect to an AP
void wifi_sta_init(uint8_t *ssid, uint8_t *pwd, wifi_auth_mode_t authmode)
{
    wifi_config_t wifi_config = {              \
        .sta = {                                \
            .threshold.authmode = authmode,     \
        },                                      \
    };

    // Copy SSID and password to the Wi-Fi configuration structure
    strcpy((char *)wifi_config.sta.ssid, (const char *)ssid);
    strcpy((char *)wifi_config.sta.password, (const char *)pwd);

    //xTaskCreate(wifi_task, "wifi_task", 6 * 1024, &wifi_config, 9, &wifi_TaskHandle);

   // wifi_wait_connect();  // Wait for the connection to establish and get IP address
}


void wifi_scan()
{
    objects_t objs           = objects;  
    lv_obj_t *list_wifi_ssid = objs.drp_wifi_ssid;
   // lv_obj_t *btn_wifi_scann = objs.btn_wifi_scann;

    uint16_t number           = DEFAULT_SCAN_LIST_SIZE;  // Maximum number of APs to be stored
    char wifiAppInfo[128]; // Array to hold SSID strings of found APs

    uint16_t ap_count = 0;  // Variable to hold the actual number of APs found
    memset(wifi_scann_list, 0, sizeof(wifi_scann_list));  // Clear the wifi_scann_list array

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));  // Set Wi-Fi mode to Station (STA) initially
    ESP_ERROR_CHECK(esp_wifi_start());  // Start the Wi-Fi driver
    
    //lv_obj_add_state(btn_wifi_scann, LV_STATE_DISABLED); // Disable the scan button during scanning

    esp_wifi_scan_start(NULL, true);  // Start Wi-Fi scanning (pass NULL to scan all channels)
    
    //lv_obj_clear_state(btn_wifi_scann, LV_STATE_DISABLED); // Re-enable the scan button after scanning is complete

    ESP_LOGI(TAG_WIFI, "Max AP number wifi_scann_list can hold = %u", number);  // Log the max AP number that can be stored
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));  // Get the actual number of APs found
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, wifi_scann_list));  // Get the AP records into wifi_scann_list array

    ESP_LOGI(TAG_WIFI, "Total APs scanned = %u, actual AP number wifi_scann_list holds = %u", ap_count, number);  // Log total and actual scanned APs

    //Clear all options in a drop-down list.
    lv_dropdown_clear_options(list_wifi_ssid);

    // Loop through each AP and log details
    for (int i = 0; i < number; i++) 
    {
        memset(wifiAppInfo, 0, sizeof(wifiAppInfo));
        sprintf(wifiAppInfo, "%s (RSSI: %d CH: %d)", wifi_scann_list[i].ssid,wifi_scann_list[i].rssi,wifi_scann_list[i].primary);

        lv_dropdown_add_option(list_wifi_ssid, wifiAppInfo/*&wifi_scann_list[i].ssid*/, i);
        ESP_LOGI(TAG_WIFI, "SSID \t\t%s", wifi_scann_list[i].ssid);  // Log SSID (network name)
        ESP_LOGI(TAG_WIFI, "RSSI \t\t%d", wifi_scann_list[i].rssi);  // Log RSSI (signal strength)
    

        // Print authentication mode of the AP
      //  print_auth_mode(wifi_scann_list[i].authmode);
#if 0
        // If the authentication mode is not WEP, print cipher types
        if (wifi_scann_list[i].authmode != WIFI_AUTH_WEP) {
            print_cipher_type(wifi_scann_list[i].pairwise_cipher, wifi_scann_list[i].group_cipher);
        }
#endif
        ESP_LOGI(TAG_WIFI, "Channel \t\t%d", wifi_scann_list[i].primary);  // Log channel number
    }
}
#endif


/*
void action_wifi_scann(lv_event_t *e) 
{
    lv_event_code_t code   = lv_event_get_code(e);
    lv_obj_t *btn_WifiScan = lv_event_get_target(e);   
    if(code == LV_EVENT_CLICKED) 
    {
        LV_LOG_USER("Clicked");
        lvgl_port_lock(-1);
        lv_obj_add_state(btn_WifiScan, LV_STATE_DISABLED); // Disable the scan button during scanning        
        lvgl_port_unlock();

        wifi_scan();
        lvgl_port_lock(-1);
        lv_obj_clear_state(btn_WifiScan, LV_STATE_DISABLED); // Re-enable the scan button after scanning is complete
        lvgl_port_unlock();
    }
}
*/
#if 0
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
        ESP_LOGI(TAG_WIFI, "Click On Wifi Psw Textbox ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        memcpy(stUSerConfig.strWifiPassword, lv_textarea_get_text(ta), sizeof(stUSerConfig.strWifiPassword));        
        ESP_LOGI(TAG_WIFI, "Defocus On Wifi Psw Textbox ");
    }
}
#endif
void action_txt_net_cb(lv_event_t *e) 
{
    objects_t objs    = objects;
    lv_keyboard_t *kb = (lv_keyboard_t *)objs.kek_keyboard;
    uint8_t userData = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_set_size((lv_obj_t *)kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to((lv_obj_t *)kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea((lv_obj_t *)kb, ta);
        lv_keyboard_set_mode((lv_obj_t *)kb, LV_KEYBOARD_MODE_NUMBER ); // Set keyboard to number mode for IP address input              
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea((lv_obj_t *)kb, NULL);
        lv_obj_add_flag((lv_obj_t *)kb, LV_OBJ_FLAG_HIDDEN);
        if(userData == 1)
        {
            // Ip Address
            memcpy(stUSerConfig.stNetworkConfig.strIpAddr, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strIpAddr));
            ESP_LOGI(TAG_WIFI, "Click On IP Addr Textbox ");
        }
        else if(userData == 2)
        {
            memcpy(stUSerConfig.stNetworkConfig.strNetMAsk, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strNetMAsk));
            ESP_LOGI(TAG_WIFI, "Click On NetMask Textbox ");
        }
        else if(userData == 3)
        {
            memcpy(stUSerConfig.stNetworkConfig.strGateway, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strGateway));
            ESP_LOGI(TAG_WIFI, "Click On Gateway Textbox ");
        }
        else if(userData == 4)
        {
            memcpy(stUSerConfig.stNetworkConfig.strDns, lv_textarea_get_text(ta), sizeof(stUSerConfig.stNetworkConfig.strDns));
            ESP_LOGI(TAG_WIFI, "Click On Dns Textbox ");
        }      
    }
}
#if 0
void action_txt_ip_addr(lv_event_t *e) 
{
    objects_t objs    = objects;
    lv_keyboard_t *kb = objs.kek_keyboard;
   
    lv_obj_set_size(kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to(kb, lv_scr_act(), LV_ALIGN_BOTTOM_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea(kb, ta);
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER ); // Set keyboard to number mode for IP address input
        ESP_LOGI(TAG_WIFI, "Click On IP Addr Textbox ");
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        ESP_LOGI(TAG_WIFI, "Defocus On IP Addr Textbox ");
    }
}

void action_txt_day(lv_event_t *e) 
{
    objects_t objs    = objects;
    lv_keyboard_t *kb = objs.kek_keyboard;
    uint8_t userData = (uint32_t)(uintptr_t)lv_event_get_user_data(e);
    lv_obj_set_size(kb, lv_pct(100), lv_pct(40)); // Set size
    lv_obj_align_to(kb, lv_scr_act(), LV_ALIGN_TOP_MID, 0, 0); // Align to bottom

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *ta         = lv_event_get_target(e);
 
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) 
    {
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_keyboard_set_textarea(kb, ta);
        lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER ); // Set keyboard to number mode for IP address input

        if(userData == 1)
        {
            ESP_LOGI(TAG_WIFI, "Click On Day Textbox ");
        }
        else if(userData == 2)
        {
            ESP_LOGI(TAG_WIFI, "Click On Month Textbox ");
        }
        else if(userData == 3)
        {
            ESP_LOGI(TAG_WIFI, "Click On Year Textbox ");
        }
        else if(userData == 4)
        {
            ESP_LOGI(TAG_WIFI, "Click On Hour Textbox ");
        }
        else if(userData == 5)
        {
            ESP_LOGI(TAG_WIFI, "Click On Minute Textbox ");
        }    

        
    }  
    if(code == LV_EVENT_DEFOCUSED) 
    {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        ESP_LOGI(TAG_WIFI, "Defocus On IP Addr Textbox ");
    }
}
#endif

// Select SSID from Dropdown
#if 0
void action_ssid_select(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj       = lv_event_get_target(e);    
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
       // char buf[64];
        //lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
        
       // LV_LOG_USER("Option: %s", buf);
        ESP_LOGI(TAG_WIFI, "%s ", wifi_scann_list[lv_dropdown_get_selected(obj)].ssid);  // Log RSSI (signal strength)
        memcpy(stUSerConfig.strWifiSsid, wifi_scann_list[lv_dropdown_get_selected(obj)].ssid, sizeof(stUSerConfig.strWifiSsid));
    }    
}
#endif



void action_btn_apply(lv_event_t *e) {
    // TODO: Implement action btn_apply here
}

void action_btn_cancell(lv_event_t *e) {
    // TODO: Implement action btn_cancell here
}



void action_sw_static_dynamic_ip(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        ESP_LOGI(TAG_WIFI, "State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        stUSerConfig.stNetworkConfig.eStaticDynamic = lv_obj_has_state(obj, LV_STATE_CHECKED) ? DYNAMIC_IP : STATIC_IP;
    }
}

void action_txt_ntp_server_cb(lv_event_t *e)
 {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);
    if(code == LV_EVENT_FOCUSED) 
    {
        ESP_LOGI(TAG_WIFI, "Click On Ntp Server Textbox ");
        memcpy(stUSerConfig.strNtpServer, lv_textarea_get_text(obj), sizeof(stUSerConfig.strNtpServer));
    }   
}

void action_txt_hostname_cb(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);
    if(code == LV_EVENT_FOCUSED) 
    {
        ESP_LOGI(TAG_WIFI, "Click On Hostname Textbox ");
        memcpy(stUSerConfig.strHostname, lv_textarea_get_text(obj), sizeof(stUSerConfig.strHostname));
    }
}
#if 0
// Save Configuration
void action_set_clock(lv_event_t *e) {
    // TODO: Implement action save_configuration here
    ESP_LOGI(TAG_WIFI, "Save Config Clicked ");
}
#endif 
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// RealtimeClock Related Actions
///////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Calculate Day of Week from Date
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
static int dayofweek(int day,int month,int year)
{
    int arr[] = {0,3,2,5,3,5,1,4,6,2,4};
    if(month<3)
        year--;
    return ((year+year/4-year/100+year/400+arr[month-1]+day)%7);
}
    #endif
///////////////////////////////////////////////////////////////////////////////////////////////////////
/// Action Callback When RealTme Settings Screen is loaded
///////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
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
        ESP_LOGE(TAG_WIFI, "Failed to get time from PCF8563");
    } else 
    {
        stUSerConfig.stRtcClock.tm_year = currentTime.tm_year;
        stUSerConfig.stRtcClock.tm_mon  = currentTime.tm_mon;   
        stUSerConfig.stRtcClock.tm_mday = currentTime.tm_mday;
        stUSerConfig.stRtcClock.tm_hour = currentTime.tm_hour;
        stUSerConfig.stRtcClock.tm_min  = currentTime.tm_min;
        stUSerConfig.stRtcClock.tm_sec  = currentTime.tm_sec;

        
        ESP_LOGI(TAG_WIFI, "Current Date/Time: %04d-%02d-%02d %02d:%02d:%02d",
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
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////
/// Action Callback When RealTme Set Clock Button is clicked
//////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
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
            ESP_LOGI(TAG_WIFI, "Set RTC Manual ");

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
            ESP_LOGI(TAG_WIFI, "Set RTC From NTP Server ");
        }
    }
}
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////
/// Action Callback When RealTme Switch Manual/NTP Server is clicked
//////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
void action_sw_manual_rtc_ntp_server(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t        *obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
        ESP_LOGI(TAG_WIFI, "State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        stUSerConfig.eRtcManualAuto = lv_obj_has_state(obj, LV_STATE_CHECKED) ? RTC_MANUAL : RTC_FROM_NTP_SERVER;
    }
}
    #endif
///////////////////////////////////////////////////////////////////////////////////////////////////////
/// Action Callback When RealTme Date/Time Dropdown is changed
///////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
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
                ESP_LOGI(TAG_WIFI, "Day Selected ");                
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_WIFI, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_mday = atoi(strBuffer);                
                break;
            case 2:
                ESP_LOGI(TAG_WIFI, "Month Selected ");                
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_WIFI, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_mon = lv_dropdown_get_selected(obj) + 1; // Months are 0-11 in struct tm               
                break;
            case 3:
                ESP_LOGI(TAG_WIFI, "Year Selected ");                
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_WIFI, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_year = atoi(strBuffer);                
                break;
            case 4:
                ESP_LOGI(TAG_WIFI, "Hour Selected ");
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_WIFI, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_hour = atoi(strBuffer);
                break;
            case 5:
                ESP_LOGI(TAG_WIFI, "Minute Selected ");
                lv_dropdown_get_selected_str(obj,strBuffer,sizeof(strBuffer));
                ESP_LOGI(TAG_WIFI, "%s ",strBuffer);
                stUSerConfig.stRtcClock.tm_min = atoi(strBuffer);
                break;    
            default:
                break;
        }        
    } 
}
#endif
