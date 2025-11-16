/*****************************************************************************
 * | File         :   sd.h
 * | Author       :   Waveshare team
 * | Function     :   SD card configuration header file
 * | Info         :
 * |                 This header file declares the initialization interface for the SD card.
 * |                 
 * |                 
 * ----------------
 * | This version :   V1.0
 * | Date         :   2024-11-28
 * | Info         :   Basic version
 *
 ******************************************************************************/

 #ifndef __SYS_INFO_H
 #define __SYS_INFO_H

#include "esp_log.h"
#include "esp_partition.h"
#include "screens.h"
#include "esp_mac.h"
#include "esp_system.h"    // Declare esp_chip_info and other system APIs
#include "esp_chip_info.h" // Declare esp_chip_info_t type
#include "esp_ota_ops.h"   // Declare esp_ota_get_app_description
#include "esp_spi_flash.h" // Declare spi_flash_get_chip_size
#include "esp_flash.h"     // Declare esp_flash_get_size and modern flash API
#include <inttypes.h>

 void vPrintBootInfo( lv_obj_t *txt_boot_area );
 
 #endif  // __SYS_INFO_H