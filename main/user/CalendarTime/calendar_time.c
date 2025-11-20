#include "pcf8523.h"
#include "calendar_time.h"

static const char    *TAG = "CalendarTime"; // Tag used for ESP log output

static uint32_t current_year  = 1900;
static uint32_t current_month = 1;
static uint32_t current_day   = 1;

void action_data_change(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t date;
        if(lv_calendar_get_pressed_date(obj, &date)) {
            LV_LOG_USER("Clicked date: %02d.%02d.%d", date.day, date.month, date.year);
            
        }
    }
}
void action_update_data_time(lv_event_t *e) {
    
}

void action_hour_change(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Selected hour: %s\n", buf);
    }
}

void action_minute_change(lv_event_t *e) {
     lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        LV_LOG_USER("Selected Minute: %s\n", buf);
    }
}

void initialize_data_and_time( )
{    

    lv_obj_t  * calendar = lv_calendar_create(lv_scr_act());
    lv_obj_set_size(calendar, 185, 185);
    lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 100);
    lv_obj_add_event_cb(calendar, action_data_change, LV_EVENT_ALL, NULL);

    lv_obj_t * roller1 = lv_roller_create(lv_scr_act());
    lv_roller_set_options(roller1, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23", LV_ROLLER_MODE_INFINITE);
    lv_obj_set_width(roller1, 60);
    lv_obj_align(roller1, LV_ALIGN_CENTER, 200, 200);
    lv_obj_add_event_cb(roller1, action_hour_change, LV_EVENT_VALUE_CHANGED, NULL);

     lv_obj_t * roller2 = lv_roller_create(lv_scr_act());
    lv_roller_set_options(roller2, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59", LV_ROLLER_MODE_INFINITE);
    lv_obj_set_width(roller2, 60);
    lv_obj_align(roller2, LV_ALIGN_CENTER, 1200, 200);
    lv_obj_add_event_cb(roller2, action_minute_change, LV_EVENT_VALUE_CHANGED, NULL);


  // #if 0
    objects_t objs = objects;
    lv_obj_t *ptr_lvCalendar_Data  = objs.calendar_data;

    struct tm stCurrDataAndTime;
    if( Pcf8523_Get_Time(&stCurrDataAndTime) < 0 ) 
    {
        ESP_LOGE(TAG, "Failed to get current time from PCF8523");        
         lv_calendar_set_today_date(calendar, current_year, current_month, current_day);
         lv_calendar_set_showed_date(calendar, current_year, current_month);
    //    lv_calendar_set_today_date(ptr_lvCalendar_Data,current_year, current_month, current_day);    
      //  lv_calendar_set_showed_date(ptr_lvCalendar_Data, 2021, 02);    
    }
    else 
    {
        //lv_calendar_set_today_date(ptr_lvCalendar_Data, stCurrDataAndTime.tm_year, stCurrDataAndTime.tm_mon, stCurrDataAndTime.tm_mday);
         lv_calendar_set_today_date(calendar, stCurrDataAndTime.tm_year, stCurrDataAndTime.tm_mon, stCurrDataAndTime.tm_mday);
         lv_calendar_set_showed_date(calendar, stCurrDataAndTime.tm_year, stCurrDataAndTime.tm_mon);

        lv_calendar_header_arrow_create(calendar);
        lv_calendar_set_showed_date(calendar, stCurrDataAndTime.tm_year, stCurrDataAndTime.tm_mon);

        current_year  = stCurrDataAndTime.tm_year;
        current_month = stCurrDataAndTime.tm_mon;
        current_day   = stCurrDataAndTime.tm_mday;
        ESP_LOGI(TAG, "Current time from PCF8523: %04d-%02d-%02d %02d:%02d:%02d",
                 stCurrDataAndTime.tm_year,
                 stCurrDataAndTime.tm_mon,
                 stCurrDataAndTime.tm_mday,
                 stCurrDataAndTime.tm_hour,
                 stCurrDataAndTime.tm_min,
                 stCurrDataAndTime.tm_sec);
    }
                 //#endif

   
}
