#include "sys_info.h"
#include "BME280.h"
#include "pcf8523.h"
#include "esp_task_wdt.h"
/* 
    # ESP-IDF Partition Table

    Name    , Type  ,SubType , Offset, Size, Flags
    nvs     , data  ,nvs     , 0x9000, 16K,
    otadata , data  ,ota     , 0xd000,  8K,
    phy_init, data  ,phy     , 0xf000,  4K,
    coredump, data  ,coredump, 0x10000, 64K,
    ota_0   , app   ,ota_0   , 0x20000,4M,
    ota_1   , app   ,ota_1   , 0x420000,4M,
    spiffs  , data  ,spiffs  , 0x820000,4M,
    fat     , data  ,fat     , 0xd20000,2880K
*/
static const char    *TAG = "DomoHomeMain"; // Tag used for ESP log output
static char strLogBuffer[4096] = {0};

void vPrintBootInfo( lv_obj_t *txt_boot_area )
{
    uint8_t base_mac_addr[6] = {0};
    esp_err_t ret = ESP_OK;
    esp_chip_info_t stChipInfos;
    const esp_app_desc_t *stAppInfo;
    
    lv_obj_invalidate(txt_boot_area);

    ESP_LOGI(TAG, "----------------DomoHome Boot Info---------------"); 
    sprintf(strLogBuffer, "DomoHome Booting Infos\n");
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

    sprintf(strLogBuffer, "\n");
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

    esp_chip_info(&stChipInfos);
    ESP_LOGI(TAG, "ESP32-%s chip, Number Of CPU core(s): %d, WiFi%s%s ",
             (stChipInfos.model == CHIP_ESP32S3) ? "S3" : "Unknown",
              stChipInfos.cores,
             (stChipInfos.features & CHIP_FEATURE_BT) ? "/BT" : "",         
                (stChipInfos.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    
    sprintf(strLogBuffer, "ESP32-%s chip, Number Of CPU core(s): %d, WiFi%s%s \n",
             (stChipInfos.model == CHIP_ESP32S3) ? "S3" : "Unknown",
              stChipInfos.cores,
             (stChipInfos.features & CHIP_FEATURE_BT) ? "/BT" : "",         
                (stChipInfos.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

    ESP_LOGI(TAG, "Silicon Revision: %d ", stChipInfos.revision);
    sprintf(strLogBuffer, "Silicon Revision: %d \n", stChipInfos.revision);
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

    // MAC Address
    if(esp_read_mac(base_mac_addr, ESP_MAC_BASE) == ESP_OK) 
    {
        ESP_LOGI(TAG, "Base MAC Address ESP_MAC_BASE: %02x:%02x:%02x:%02x:%02x:%02x",
             base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
             base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]); 

        sprintf(strLogBuffer, "Base MAC Address ESP_MAC_BASE                  : %02x:%02x:%02x:%02x:%02x:%02x\n",
                base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
                base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    } 
    else 
    {
        ESP_LOGE(TAG, "Failed to get base MAC address from EFUSE BLK0.");
        sprintf(strLogBuffer, "Failed to get base MAC address from EFUSE BLK0.\n");
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    }

    ret = esp_read_mac(base_mac_addr, ESP_MAC_BT);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to get base MAC address from EFUSE BLK3. (%s)", esp_err_to_name(ret));
        sprintf(strLogBuffer, "Failed to get base MAC address from EFUSE BLK3. (%s)\n", esp_err_to_name(ret));
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    } 
    else 
    { 
        ESP_LOGI(TAG, "Base MAC Address ESP_MAC_BT: %02x:%02x:%02x:%02x:%02x:%02x",
             base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
             base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);                         
        sprintf(strLogBuffer, "Base MAC Address ESP_MAC_BT                        : %02x:%02x:%02x:%02x:%02x:%02x\n",
                base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
                base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    }

    ret = esp_read_mac(base_mac_addr, ESP_MAC_WIFI_STA);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to get base MAC address from EFUSE BLK3. (%s)", esp_err_to_name(ret));
        sprintf(strLogBuffer, "Failed to get base MAC address from EFUSE BLK3. (%s)\n", esp_err_to_name(ret));
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    } 
    else 
    {
        ESP_LOGI(TAG, "Base MAC Address ESP_MAC_WIFI_STA: %02x:%02x:%02x:%02x:%02x:%02x",
             base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
             base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]); 

        sprintf(strLogBuffer, "Base MAC Address ESP_MAC_WIFI_STA          : %02x:%02x:%02x:%02x:%02x:%02x\n", 
                base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
                base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    }

    ret = esp_read_mac(base_mac_addr, ESP_MAC_WIFI_SOFTAP);
    if (ret != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to get base MAC address from EFUSE BLK3. (%s)", esp_err_to_name(ret));
        sprintf(strLogBuffer, "Failed to get base MAC address from EFUSE BLK3. (%s)\n", esp_err_to_name(ret));
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    } 
    else 
    {
        ESP_LOGI(TAG, "Base MAC Address ESP_MAC_WIFI_SOFTAP: %02x:%02x:%02x:%02x:%02x:%02x",
             base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
             base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]); 

        sprintf(strLogBuffer, "Base MAC Address ESP_MAC_WIFI_SOFTAP : %02x:%02x:%02x:%02x:%02x:%02x\n",
                base_mac_addr[0], base_mac_addr[1], base_mac_addr[2],
                base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    }

    // Flash size
    uint32_t flash_size = 0;
    esp_flash_get_size(NULL, (uint32_t *)&flash_size);
    ESP_LOGI(TAG, "Flash size: %" PRIu32 " bytes", flash_size);
    sprintf(strLogBuffer, "Flash size: %lu bytes\n", flash_size);
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

    ESP_LOGI(TAG, "Partitions Map");
    sprintf(strLogBuffer, "Partitions Map...\n");
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

   // #if 0

    esp_partition_iterator_t it;

    ESP_LOGI(TAG, "Iterating through app partitions...");
    sprintf(strLogBuffer, "Application Partitions...\n");
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

    it = esp_partition_find(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, NULL);

    // Loop through all matching partitions, in this case, all with the type 'data' until partition with desired 
    // label is found. Verify if its the same instance as the one found before.
    for (; it != NULL; it = esp_partition_next(it)) 
    {
       
        const esp_partition_t *part = esp_partition_get(it);
        ESP_LOGI(TAG, "\tfound partition '%s' Offset 0x%"PRIx32" Size 0x%"PRIx32, part->label, part->address, part->size);
        sprintf(strLogBuffer, "\tPartition '%s' Offset: 0x8%"PRIx32" Size: 0x%"PRIx32"\n", part->label, part->address, part->size);
        lv_textarea_add_text(txt_boot_area, strLogBuffer);       
    }
    // Release the partition iterator to release memory allocated for it
    esp_partition_iterator_release(it);

    ESP_LOGI(TAG, "Data Partitions...");
    sprintf(strLogBuffer, "Data Partitions......\n");
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

    it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);

    // Loop through all matching partitions, in this case, all with the type 'data' until partition with desired 
    // label is found. Verify if its the same instance as the one found before.
    for (; it != NULL; it = esp_partition_next(it)) 
    {        
        const esp_partition_t *part = esp_partition_get(it);
        ESP_LOGI(TAG, "\tfound partition '%s' at offset 0x%"PRIx32" with size 0x%"PRIx32, part->label, part->address, part->size);
        sprintf(strLogBuffer, "\tPartition '%s' at offset 0x%"PRIx32" with size 0x%"PRIx32"\n", part->label, part->address, part->size);
        lv_textarea_add_text(txt_boot_area, strLogBuffer);          
    }

    // Release the partition iterator to release memory allocated for it
    esp_partition_iterator_release(it);

    ESP_LOGI(TAG, "----------------End of partition listing---------------");
 // #endif  
    uint8_t u8DevId;
    int8_t u8Pbit = bme280_Pbit(&u8DevId);
    if(u8Pbit == 0)
    {
        ESP_LOGI(TAG, "BME280 Device ID: 0x%02X", u8DevId);
        sprintf(strLogBuffer, "Temperature/Humidity/Pressure Sensor (BME280) OK DeviceID: 0x%02X\n", u8DevId);
        lv_textarea_add_text(txt_boot_area, strLogBuffer);        
    }
    else
    {
        ESP_LOGE(TAG, "Failed to read BME280 Device ID OK");
        sprintf(strLogBuffer, "Temperature/Humidity/Pressure Sensor (BME280) KO\n");
        lv_textarea_add_text(txt_boot_area, strLogBuffer);        
    }

    if (Pcf8523_PBit_Battery_Status() == 1)
    {
        ESP_LOGI(TAG, "PCF8523 RTC Battery Status: OK");  
        sprintf(strLogBuffer, "PCF8523 RTC Battery Status: OK\n");
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    }
    else
    {
        ESP_LOGI(TAG, "PCF8523 RTC Battery Status: KO");  
        sprintf(strLogBuffer, "PCF8523 RTC Battery Status: KO\n");
        lv_textarea_add_text(txt_boot_area, strLogBuffer);
    }
    
    stAppInfo = esp_app_get_description();
    ESP_LOGI(TAG, "App version: %s", stAppInfo->version);  
    sprintf(strLogBuffer, "App version: %s\n", stAppInfo->version);
    lv_textarea_add_text(txt_boot_area, strLogBuffer);

    ESP_LOGI(TAG, "Start Application....");  
    lv_textarea_add_text(txt_boot_area, "Start Application....\n");

}