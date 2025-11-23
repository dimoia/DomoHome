#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *heating_screen;
    lv_obj_t *main;
    lv_obj_t *settings_screen;
    lv_obj_t *btn_goto_home_from_heater_screen;
    lv_obj_t *btn_goto_home_1;
    lv_obj_t *lbl_time_1;
    lv_obj_t *lbl_home_1;
    lv_obj_t *lbl_str_date_1;
    lv_obj_t *obj0;
    lv_obj_t *drop_day_1;
    lv_obj_t *lbl_from_to_temp_1;
    lv_obj_t *slider_temp_1;
    lv_obj_t *lider_timer_1;
    lv_obj_t *lbl_from_to_timer_1;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *btn_goto_home;
    lv_obj_t *lbl_home;
    lv_obj_t *lbl_time;
    lv_obj_t *temp_humi_panel;
    lv_obj_t *lbl_press_hpa;
    lv_obj_t *chart_temp_humy;
    lv_obj_t *img_humi;
    lv_obj_t *img_temperature;
    lv_obj_t *img_humidity;
    lv_obj_t *lbl_pressure;
    lv_obj_t *lbl_press_value;
    lv_obj_t *lbl_temperature;
    lv_obj_t *lbl_temp_value;
    lv_obj_t *lbl_humidity;
    lv_obj_t *lbl_humidity_value;
    lv_obj_t *lbl_c;
    lv_obj_t *lbl_percent;
    lv_obj_t *chart_humy;
    lv_obj_t *conteiner_time;
    lv_obj_t *clk_meter;
    lv_obj_t *roller_hour;
    lv_obj_t *roller_minute;
    lv_obj_t *calendar_data;
    lv_obj_t *btn_heating_screen;
    lv_obj_t *obj9;
    lv_obj_t *lbl_str_date;
    lv_obj_t *btn_goto_home_from_heater_screen_1;
    lv_obj_t *btn_goto_home_2;
    lv_obj_t *lbl_home_2;
    lv_obj_t *lbl_time_2;
    lv_obj_t *lbl_str_date_2;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_HEATING_SCREEN = 1,
    SCREEN_ID_MAIN = 2,
    SCREEN_ID_SETTINGS_SCREEN = 3,
};

void create_screen_heating_screen();
void tick_screen_heating_screen();

void create_screen_main();
void tick_screen_main();

void create_screen_settings_screen();
void tick_screen_settings_screen();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/