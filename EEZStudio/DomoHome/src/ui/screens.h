#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_HEATING_SCREEN = 1,
    SCREEN_ID_MAIN = 2,
    SCREEN_ID_SETTINGS_SCREEN = 3,
    SCREEN_ID_CONFIG_TERMOSTATO = 4,
    _SCREEN_ID_LAST = 4
};

typedef struct _objects_t {
    lv_obj_t *heating_screen;
    lv_obj_t *main;
    lv_obj_t *settings_screen;
    lv_obj_t *config_termostato;
    lv_obj_t *btn_goto_home_from_heater_screen;
    lv_obj_t *btn_goto_home_1;
    lv_obj_t *lbl_time_1;
    lv_obj_t *lbl_home_1;
    lv_obj_t *lbl_str_date_1;
    lv_obj_t *obj0;
    lv_obj_t *drop_day_monday;
    lv_obj_t *slider_temp_monday;
    lv_obj_t *slider_timer_monday;
    lv_obj_t *monday_time_min;
    lv_obj_t *monday_time_max;
    lv_obj_t *monday_temp_min;
    lv_obj_t *monday_temp_max;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *drop_day_tuesday;
    lv_obj_t *slider_temp_tuesday;
    lv_obj_t *slider_timer_tuesday;
    lv_obj_t *tuesday_time_min;
    lv_obj_t *tuesday_time_max;
    lv_obj_t *tuesday_temp_min;
    lv_obj_t *tuesday_temp_max;
    lv_obj_t *obj3;
    lv_obj_t *drop_day_wednesday;
    lv_obj_t *slider_temp_wednesday;
    lv_obj_t *slider_timer_wednesday;
    lv_obj_t *wednesday_time_min;
    lv_obj_t *wednesday_time_max;
    lv_obj_t *wednesday_temp_min;
    lv_obj_t *wednesday_temp_max;
    lv_obj_t *obj4;
    lv_obj_t *drop_day_thursday;
    lv_obj_t *slider_temp_thursday;
    lv_obj_t *slider_timer_thursday;
    lv_obj_t *thursday_time_min;
    lv_obj_t *thursday_time_max;
    lv_obj_t *thursday_temp_min;
    lv_obj_t *thursday_temp_max;
    lv_obj_t *obj5;
    lv_obj_t *drop_day_friday;
    lv_obj_t *slider_temp_friday;
    lv_obj_t *slider_timer_friday;
    lv_obj_t *friday_time_min;
    lv_obj_t *friday_time_max;
    lv_obj_t *friday_temp_min;
    lv_obj_t *friday_temp_max;
    lv_obj_t *obj6;
    lv_obj_t *drop_day_saturday;
    lv_obj_t *slider_temp_saturday;
    lv_obj_t *slider_timer_saturday;
    lv_obj_t *saturday_time_min;
    lv_obj_t *saturday_time_max;
    lv_obj_t *saturday_temp_min;
    lv_obj_t *saturday_temp_max;
    lv_obj_t *obj7;
    lv_obj_t *drop_day_sunday;
    lv_obj_t *slider_temp_sunday;
    lv_obj_t *slider_timer_sunday;
    lv_obj_t *sunday_time_min;
    lv_obj_t *sunday_time_max;
    lv_obj_t *sunday_temp_min;
    lv_obj_t *sunday_temp_max;
    lv_obj_t *btn_goto_home;
    lv_obj_t *lbl_home;
    lv_obj_t *lbl_time;
    lv_obj_t *btn_heating_screen;
    lv_obj_t *obj8;
    lv_obj_t *btn_settings_screen;
    lv_obj_t *lbl_home_3;
    lv_obj_t *lbl_str_date;
    lv_obj_t *btn_energy_power;
    lv_obj_t *meter_temp;
    lv_obj_t *lbl_temp_f;
    lv_obj_t *lbl_temp_value_farheneit;
    lv_obj_t *obj9;
    lv_obj_t *obj10;
    lv_obj_t *lbl_temp;
    lv_obj_t *lbl_temp_value;
    lv_obj_t *lbl_temperature;
    lv_obj_t *meter_humidity;
    lv_obj_t *obj11;
    lv_obj_t *lbl_humidity_percent;
    lv_obj_t *lbl_humi_value;
    lv_obj_t *lbl_humidity;
    lv_obj_t *meter_pressure;
    lv_obj_t *lbl_press_hpa;
    lv_obj_t *lbl_pressure;
    lv_obj_t *lbl_press_value;
    lv_obj_t *img_wifi;
    lv_obj_t *btn_goto_home_from_config_screen;
    lv_obj_t *btn_goto_home_2;
    lv_obj_t *lbl_home_2;
    lv_obj_t *obj12;
    lv_obj_t *txt_ntp_server;
    lv_obj_t *sw_manual_automatic_time;
    lv_obj_t *sw_manual_rtc_ntp_server;
    lv_obj_t *drp_wifi_ssid;
    lv_obj_t *txt_wifi_psw;
    lv_obj_t *obj13;
    lv_obj_t *txt_hostname;
    lv_obj_t *txt_ipaddress;
    lv_obj_t *txt_netmask;
    lv_obj_t *txt_gateway;
    lv_obj_t *sw_static_ip_dynamic_ip;
    lv_obj_t *btn_wifi_scann;
    lv_obj_t *btn_set_clock;
    lv_obj_t *kek_keyboard;
    lv_obj_t *obj14;
    lv_obj_t *btn_save_to_flash;
    lv_obj_t *btn_erase_flash;
    lv_obj_t *drop_day;
    lv_obj_t *drop_month;
    lv_obj_t *drop_year;
    lv_obj_t *drop_hour;
    lv_obj_t *drop_minute;
    lv_obj_t *txt_broker_ip;
    lv_obj_t *txt_mqttuserid;
    lv_obj_t *txt_mqttpassword;
    lv_obj_t *txt_mqttclient_id;
    lv_obj_t *combo_weather_server;
    lv_obj_t *obj15;
    lv_obj_t *btn_restart;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *txt_broker_port;
    lv_obj_t *txt_weather_key;
    lv_obj_t *sw_mqtt_status;
    lv_obj_t *txt_mqtttopic;
    lv_obj_t *txt_mqttsubscribe;
    lv_obj_t *sw_wheater;
    lv_obj_t *chart_humy;
    lv_obj_t *tab_configuration;
    lv_obj_t *tab_wifi_config;
    lv_obj_t *txt_tab_wifi_password;
    lv_obj_t *obj18;
    lv_obj_t *drp_tab_wifi_ssid;
    lv_obj_t *txt_tab_wifi_hostname;
    lv_obj_t *btn_scann_tab_wifi_config;
    lv_obj_t *tab_network_config;
    lv_obj_t *txt_tab_net_dns;
    lv_obj_t *txt_tab_net_ipaddress;
    lv_obj_t *sw_tab_net_static_ip_dynamic_ip;
    lv_obj_t *txt_tab_net_gateway;
    lv_obj_t *txt_tab_net_netmask;
    lv_obj_t *tab_time_config;
    lv_obj_t *obj19;
    lv_obj_t *obj20;
    lv_obj_t *obj21;
    lv_obj_t *tab_time_config_sw_wheater;
    lv_obj_t *tab_time_txt_weather_key;
    lv_obj_t *tab_time_combo_weather_server;
    lv_obj_t *sw_tab_time_config_rtc_or_ntp;
    lv_obj_t *tab_time_confiig_txt_ntp_server;
    lv_obj_t *drop_tab_time_config_minute;
    lv_obj_t *drop_tab_time_config_hour;
    lv_obj_t *drop_tab_time_config_year;
    lv_obj_t *drop_tab_time_config_month;
    lv_obj_t *drop_tab_time_config_day;
    lv_obj_t *btn_tab_time_config_set_clock;
    lv_obj_t *tab_ha_config;
    lv_obj_t *tab_ha_txt_mqttsubscribe;
    lv_obj_t *tab_ha_txt_mqtttopic;
    lv_obj_t *tab_ha_sw_mqtt_status;
    lv_obj_t *tab_ha_txt_broker_port;
    lv_obj_t *tab_ha_txt_mqttclient_id;
    lv_obj_t *tab_ha_txt_mqttpassword;
    lv_obj_t *tab_ha_txt_mqttuserid;
    lv_obj_t *tab_ha_txt_broker_ip;
    lv_obj_t *btn_save_to_nvs;
    lv_obj_t *btn_erase_nvs;
    lv_obj_t *btn_restart_device;
    lv_obj_t *btn_config_termostato_return_to_home;
    lv_obj_t *btn_goto_home_3;
    lv_obj_t *lbl_home_4;
} objects_t;

extern objects_t objects;

typedef struct {
    lv_meter_scale_t *scale;
    lv_meter_indicator_t *indicator;
    lv_meter_indicator_t *indicator1;
    lv_meter_indicator_t *indicator2;
    lv_meter_indicator_t *indicator3;
    lv_meter_indicator_t *indicator4;
    lv_meter_indicator_t *indicator5;
    lv_meter_indicator_t *indicator6;
    lv_meter_indicator_t *indicator7;
    lv_meter_indicator_t *indicator8;
    lv_meter_indicator_t *indicator9;
    lv_meter_scale_t *scale1;
    lv_meter_indicator_t *indicator10;
    lv_meter_indicator_t *indicator11;
    lv_meter_indicator_t *indicator12;
    lv_meter_indicator_t *indicator13;
    lv_meter_scale_t *scale2;
    lv_meter_indicator_t *indicator14;
} screen_main_state_t;

extern screen_main_state_t screen_main_state;

void create_screen_heating_screen();
void tick_screen_heating_screen();

void create_screen_main();
void tick_screen_main();

void create_screen_settings_screen();
void tick_screen_settings_screen();

void create_screen_config_termostato();
void tick_screen_config_termostato();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/