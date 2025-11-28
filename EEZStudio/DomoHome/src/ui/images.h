#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern const lv_img_dsc_t img_bg_main_screen;
extern const lv_img_dsc_t img_ico_temp;
extern const lv_img_dsc_t img_ico_humidity;
extern const lv_img_dsc_t img_ico_main_home;
extern const lv_img_dsc_t img_wifi_off;
extern const lv_img_dsc_t img_wifi_on;
extern const lv_img_dsc_t img_ico_bluetoot;
extern const lv_img_dsc_t img_ico_no_battery;
extern const lv_img_dsc_t img_ico_weater;
extern const lv_img_dsc_t img_ico_settings;
extern const lv_img_dsc_t img_ico_pressure;
extern const lv_img_dsc_t img_ico_heating_64x64;
extern const lv_img_dsc_t img_ico_heating_32x32;
extern const lv_img_dsc_t img_ico_energy_power_64x64;
extern const lv_img_dsc_t img_ico_humidity_64_64;
extern const lv_img_dsc_t img_ico_high_temp;
extern const lv_img_dsc_t img_ico_low_temperature;
extern const lv_img_dsc_t img_ico_low_humidity_32_32;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[18];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/