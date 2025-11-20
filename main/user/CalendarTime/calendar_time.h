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

#ifndef __CALENDAR_TIME_H
#define __CALENDAR_TIME_H

#include "esp_log.h"
#include "lvgl_port.h"    // Header for LVGL port initialization and locking
#include "actions.h"
#include <time.h>
#include "screens.h"
#include "ui.h"

void action_data_change(lv_event_t * e);
void initialize_data_and_time();
 
#endif  // __CALENDAR_TIME_H