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
extern void action_goto_settings_screen(lv_event_t * e);
extern void action_wifi_scann(lv_event_t * e);
extern void action_wifi_connect(lv_event_t * e);
extern void action_ssid_select(lv_event_t * e);
extern void action_wifi_txt_psw(lv_event_t * e);
extern void action_txt_ip_addr(lv_event_t * e);
extern void action_txt_net_mask(lv_event_t * e);
extern void action_txt_gate_way(lv_event_t * e);
extern void action_txt_d_ns(lv_event_t * e);
extern void action_txt_day(lv_event_t * e);
extern void action_txt_month(lv_event_t * e);
extern void action_txt_year(lv_event_t * e);
extern void action_txt_hour(lv_event_t * e);
extern void action_txt_minute(lv_event_t * e);
extern void action_txt_relatime_clock(lv_event_t * e);
extern void action_btn_apply(lv_event_t * e);
extern void action_btn_cancell(lv_event_t * e);
extern void action_sw_manual_rtc_ntp_server(lv_event_t * e);
extern void action_sw_static_dynamic_ip(lv_event_t * e);
extern void action_txt_net_cb(lv_event_t * e);
extern void action_txt_ntp_server_cb(lv_event_t * e);
extern void action_txt_hostname_cb(lv_event_t * e);
extern void action_btn_real_time_set_clock_cb(lv_event_t * e);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/