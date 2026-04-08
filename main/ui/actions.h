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
extern void action_goto_settings_screen(lv_event_t * e);
extern void action_slider_temp1_change(lv_event_t * e);
extern void action_wifi_scann(lv_event_t * e);
extern void action_ssid_select(lv_event_t * e);
extern void action_wifi_txt_psw(lv_event_t * e);
extern void action_sw_manual_rtc_ntp_server(lv_event_t * e);
extern void action_sw_static_dynamic_ip(lv_event_t * e);
extern void action_txt_net_cb(lv_event_t * e);
extern void action_txt_ntp_server_cb(lv_event_t * e);
extern void action_txt_hostname_cb(lv_event_t * e);
extern void action_btn_real_time_set_clock_cb(lv_event_t * e);
extern void action_drop_date_time(lv_event_t * e);
extern void action_settings_screen_cb(lv_event_t * e);
extern void action_wifi_connect_cb(lv_event_t * e);
extern void action_btn_restart(lv_event_t * e);
extern void action_save_to_flash(lv_event_t * e);
extern void action_btn_erase_flash(lv_event_t * e);
extern void action_sw_wheater(lv_event_t * e);
extern void action_combo_weather_server(lv_event_t * e);
extern void action_txt_weather_key(lv_event_t * e);
extern void action_sw_mqtt_status(lv_event_t * e);
extern void action_mqtt_config(lv_event_t * e);
extern void action_txt_broker_port(lv_event_t * e);
extern void action_tab_main_config(lv_event_t * e);
extern void action_tab_wifi_config(lv_event_t * e);
extern void action_tab_network_config(lv_event_t * e);
extern void action_tab_time_config(lv_event_t * e);
extern void action_tab_ha_config(lv_event_t * e);
extern void action_scann_wifi_network(lv_event_t * e);
extern void action_tab_wifi_txt_hostname_cb(lv_event_t * e);
extern void action_txt_tab_wifi_password(lv_event_t * e);
extern void action_drp_tab_wifi_ssid(lv_event_t * e);
extern void action_network_config(lv_event_t * e);
extern void action_sw_static_or_dynamic_ip(lv_event_t * e);
extern void action_weather_enable_disable(lv_event_t * e);
extern void action_sw_cmb_weather_server_selected(lv_event_t * e);
extern void action_tab_txt_weather_key(lv_event_t * e);
extern void action_sw_mqtt_protocol_on_off(lv_event_t * e);
extern void action_tab_mqtt_config(lv_event_t * e);
extern void action_tab_mqtt_broker_port(lv_event_t * e);
extern void action_save_config_to_nvs(lv_event_t * e);
extern void action_erase_nvs_flash(lv_event_t * e);
extern void action_restart_micro(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/