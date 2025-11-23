#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_data_change(lv_event_t * e);
extern void action_update_data_time(lv_event_t * e);
extern void action_hour_change(lv_event_t * e);
extern void action_minute_change(lv_event_t * e);
extern void action_goto_heating_screen(lv_event_t * e);
extern void action_goto_home(lv_event_t * e);
extern void action_slider_temp1_change(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/