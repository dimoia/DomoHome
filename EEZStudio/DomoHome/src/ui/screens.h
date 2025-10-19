#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *header;
    lv_obj_t *lbl_home;
    lv_obj_t *footer;
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
    lv_obj_t *wifi_on;
    lv_obj_t *wifi_off;
    lv_obj_t *ble_on;
    lv_obj_t *battery_off;
    lv_obj_t *panel_data_time;
    lv_obj_t *lbl_date;
    lv_obj_t *clk_meter;
    lv_obj_t *lbl_year;
    lv_obj_t *lbl_time;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/