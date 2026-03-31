#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_SETTINGS_SCREEN = 2,
    SCREEN_ID_HEATING_SCREEN = 3,
    SCREEN_ID_TEST = 4,
    _SCREEN_ID_LAST = 4
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *settings_screen;
    lv_obj_t *heating_screen;
    lv_obj_t *test;
    lv_obj_t *btn_goto_home;
    lv_obj_t *lbl_home;
    lv_obj_t *lbl_time;
    lv_obj_t *btn_heating_screen;
    lv_obj_t *obj0;
    lv_obj_t *btn_settings_screen;
    lv_obj_t *lbl_home_3;
    lv_obj_t *lbl_str_date;
    lv_obj_t *btn_energy_power;
    lv_obj_t *meter_temp;
    lv_obj_t *lbl_temp_f;
    lv_obj_t *lbl_temp_value_farheneit;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *lbl_temp;
    lv_obj_t *lbl_temp_value;
    lv_obj_t *lbl_temperature;
    lv_obj_t *meter_humidity;
    lv_obj_t *obj3;
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
    lv_obj_t *obj4;
    lv_obj_t *txt_ntp_server;
    lv_obj_t *sw_manual_automatic_time;
    lv_obj_t *sw_manual_rtc_ntp_server;
    lv_obj_t *drp_wifi_ssid;
    lv_obj_t *txt_wifi_psw;
    lv_obj_t *obj5;
    lv_obj_t *txt_hostname;
    lv_obj_t *txt_ipaddress;
    lv_obj_t *txt_netmask;
    lv_obj_t *txt_gateway;
    lv_obj_t *txt_dns;
    lv_obj_t *sw_static_ip_dynamic_ip;
    lv_obj_t *btn_wifi_scann;
    lv_obj_t *btn_set_clock;
    lv_obj_t *btn_wifi_connect;
    lv_obj_t *btn_connect;
    lv_obj_t *kek_keyboard;
    lv_obj_t *obj6;
    lv_obj_t *btn_apply;
    lv_obj_t *btn_cancell;
    lv_obj_t *drop_day;
    lv_obj_t *drop_month;
    lv_obj_t *drop_year;
    lv_obj_t *drop_hour;
    lv_obj_t *drop_minute;
    lv_obj_t *txt_broker_ip;
    lv_obj_t *txt_mqttuserid;
    lv_obj_t *txt_mqttpassword;
    lv_obj_t *txt_mqttclient_id;
    lv_obj_t *btn_restart;
    lv_obj_t *btn_goto_home_from_heater_screen;
    lv_obj_t *btn_goto_home_1;
    lv_obj_t *lbl_time_1;
    lv_obj_t *lbl_home_1;
    lv_obj_t *lbl_str_date_1;
    lv_obj_t *obj7;
    lv_obj_t *drop_day_1;
    lv_obj_t *lbl_from_to_temp_1;
    lv_obj_t *slider_temp_1;
    lv_obj_t *lider_timer_1;
    lv_obj_t *lbl_from_to_timer_1;
    lv_obj_t *obj8;
    lv_obj_t *obj9;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *obj12;
    lv_obj_t *obj13;
    lv_obj_t *drop_day_2;
    lv_obj_t *lbl_from_to_temp_2;
    lv_obj_t *slider_temp_2;
    lv_obj_t *lider_timer_2;
    lv_obj_t *lbl_from_to_timer_2;
    lv_obj_t *obj14;
    lv_obj_t *obj15;
    lv_obj_t *obj16;
    lv_obj_t *obj17;
    lv_obj_t *obj18;
    lv_obj_t *drop_day_3;
    lv_obj_t *lbl_from_to_temp_3;
    lv_obj_t *slider_temp_3;
    lv_obj_t *lider_timer_3;
    lv_obj_t *lbl_from_to_timer_3;
    lv_obj_t *obj19;
    lv_obj_t *obj20;
    lv_obj_t *obj21;
    lv_obj_t *obj22;
    lv_obj_t *obj23;
    lv_obj_t *drop_day_4;
    lv_obj_t *lbl_from_to_temp_4;
    lv_obj_t *slider_temp_4;
    lv_obj_t *lider_timer_4;
    lv_obj_t *lbl_from_to_timer_4;
    lv_obj_t *obj24;
    lv_obj_t *obj25;
    lv_obj_t *obj26;
    lv_obj_t *obj27;
    lv_obj_t *obj28;
    lv_obj_t *drop_day_5;
    lv_obj_t *lbl_from_to_temp_5;
    lv_obj_t *slider_temp_5;
    lv_obj_t *lider_timer_5;
    lv_obj_t *lbl_from_to_timer_5;
    lv_obj_t *obj29;
    lv_obj_t *obj30;
    lv_obj_t *obj31;
    lv_obj_t *obj32;
    lv_obj_t *obj33;
    lv_obj_t *drop_day_6;
    lv_obj_t *lbl_from_to_temp_6;
    lv_obj_t *slider_temp_6;
    lv_obj_t *lider_timer_6;
    lv_obj_t *lbl_from_to_timer_6;
    lv_obj_t *obj34;
    lv_obj_t *obj35;
    lv_obj_t *obj36;
    lv_obj_t *obj37;
    lv_obj_t *obj38;
    lv_obj_t *drop_day_7;
    lv_obj_t *lbl_from_to_temp_7;
    lv_obj_t *slider_temp_7;
    lv_obj_t *lider_timer_7;
    lv_obj_t *lbl_from_to_timer_7;
    lv_obj_t *obj39;
    lv_obj_t *obj40;
    lv_obj_t *obj41;
    lv_obj_t *obj42;
    lv_obj_t *obj43;
    lv_obj_t *obj44;
    lv_obj_t *lbl_c_4;
    lv_obj_t *lbl_temp_value_4;
    lv_obj_t *lbl_temperature_2;
    lv_obj_t *clk_meter;
    lv_obj_t *calendar_data;
    lv_obj_t *roller_hour;
    lv_obj_t *roller_minute;
    lv_obj_t *lbl_press_hpa_1;
    lv_obj_t *img_temperature;
    lv_obj_t *img_humidity;
    lv_obj_t *lbl_pressure_1;
    lv_obj_t *lbl_press_value_1;
    lv_obj_t *lbl_temperature_1;
    lv_obj_t *lbl_temp_value_1;
    lv_obj_t *lbl_humidity_1;
    lv_obj_t *lbl_humidity_value_1;
    lv_obj_t *lbl_c;
    lv_obj_t *lbl_percent;
    lv_obj_t *chart_humy;
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
    lv_meter_indicator_t *indicator15;
} screen_test_state_t;

extern screen_main_state_t screen_main_state;
extern screen_test_state_t screen_test_state;

void create_screen_main();
void tick_screen_main();

void create_screen_settings_screen();
void tick_screen_settings_screen();

void create_screen_heating_screen();
void tick_screen_heating_screen();

void create_screen_test();
void tick_screen_test();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/