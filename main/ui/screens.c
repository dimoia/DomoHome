#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

static lv_meter_scale_t * scale0;
static lv_meter_indicator_t * indicator1;
static lv_meter_indicator_t * indicator2;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // Header
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.header = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 1024, 50);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 435, 13);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_main_home);
                }
                {
                    // lbl_home
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_home = obj;
                    lv_obj_set_pos(obj, 466, 15);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffbfbfb), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "HOME");
                }
            }
        }
        {
            // Footer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.footer = obj;
            lv_obj_set_pos(obj, 0, 551);
            lv_obj_set_size(obj, 1024, 49);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // tempHumi_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.temp_humi_panel = obj;
            lv_obj_set_pos(obj, 5, 53);
            lv_obj_set_size(obj, 461, 270);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // lbl_press_hpa
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_press_hpa = obj;
                    lv_obj_set_pos(obj, 163, 207);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, " [hPa]");
                }
                {
                    // chart_temp_humy
                    lv_obj_t *obj = lv_chart_create(parent_obj);
                    objects.chart_temp_humy = obj;
                    lv_obj_set_pos(obj, 21, 18);
                    lv_obj_set_size(obj, 209, 76);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff010000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff7f0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // img_humi
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.img_humi = obj;
                    lv_obj_set_pos(obj, 21, -10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffada6a6), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_x(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_ofs_y(obj, 22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_spread(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_shadow_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Temperature/Humidity");
                }
                {
                    // img_temperature
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.img_temperature = obj;
                    lv_obj_set_pos(obj, 265, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_temp);
                }
                {
                    // img_humidity
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    objects.img_humidity = obj;
                    lv_obj_set_pos(obj, 269, 129);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_humidity);
                }
                {
                    // Lbl_pressure
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_pressure = obj;
                    lv_obj_set_pos(obj, 29, 205);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Pressure");
                }
                {
                    // Lbl_press_value
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_press_value = obj;
                    lv_obj_set_pos(obj, 106, 204);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "0000");
                }
                {
                    // Lbl_temperature
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temperature = obj;
                    lv_obj_set_pos(obj, 325, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Temperature");
                }
                {
                    // Lbl_temp_value
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temp_value = obj;
                    lv_obj_set_pos(obj, 329, 46);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "00.0");
                }
                {
                    // Lbl_Humidity
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_humidity = obj;
                    lv_obj_set_pos(obj, 333, 129);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Humidity");
                }
                {
                    // Lbl_humidity_value
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_humidity_value = obj;
                    lv_obj_set_pos(obj, 333, 150);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "00.0");
                }
                {
                    // lbl_c
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_c = obj;
                    lv_obj_set_pos(obj, 372, 49);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, " [°C]");
                }
                {
                    // lbl_percent
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_percent = obj;
                    lv_obj_set_pos(obj, 376, 153);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "[%] ");
                }
                {
                    // chart_humy
                    lv_obj_t *obj = lv_chart_create(parent_obj);
                    objects.chart_humy = obj;
                    lv_obj_set_pos(obj, 21, 113);
                    lv_obj_set_size(obj, 209, 80);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff010000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff7f0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
            }
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 866, 375);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Button");
                }
            }
        }
        {
            // wifiOn
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.wifi_on = obj;
            lv_obj_set_pos(obj, 976, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_wifi_on);
        }
        {
            // wifiOff
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.wifi_off = obj;
            lv_obj_set_pos(obj, 944, 426);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_wifi_off);
        }
        {
            // bleOn
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.ble_on = obj;
            lv_obj_set_pos(obj, 934, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_ico_bluetoot);
        }
        {
            // batteryOff
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.battery_off = obj;
            lv_obj_set_pos(obj, 894, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_ico_no_battery);
        }
        {
            // panel_dataTime
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.panel_data_time = obj;
            lv_obj_set_pos(obj, 487, 53);
            lv_obj_set_size(obj, 403, 270);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // lbl_Date
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_date = obj;
                    lv_obj_set_pos(obj, 180, 148);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "<Used to set date>");
                }
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 234, -7);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_weater);
                }
                {
                    // clkMeter
                    lv_obj_t *obj = lv_meter_create(parent_obj);
                    objects.clk_meter = obj;
                    lv_obj_set_pos(obj, -22, -18);
                    lv_obj_set_size(obj, 220, 220);
                    {
                        lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                        scale0 = scale;
                        lv_meter_set_scale_ticks(obj, scale, 60, 1, 10, lv_color_hex(0xfffffefe));
                        lv_meter_set_scale_major_ticks(obj, scale, 5, 3, 10, lv_color_hex(0xff000000), 5);
                        lv_meter_set_scale_range(obj, scale, 1, 13, 360, 297);
                        {
                            lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 4, lv_color_hex(0xff9e1c00), -28);
                            indicator1 = indicator;
                            lv_meter_set_indicator_value(obj, indicator, 6);
                        }
                        {
                            lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 3, lv_color_hex(0xff9e1c00), -18);
                            indicator2 = indicator;
                            lv_meter_set_indicator_value(obj, indicator, 30);
                        }
                    }
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffd0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_arc_color(obj, lv_color_hex(0xffff1010), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff8f6f6), LV_PART_TICKS | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_ITEMS | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb4b4b4), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
                {
                    // lbl_Year
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_year = obj;
                    lv_obj_set_pos(obj, 246, 176);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Year");
                }
            }
        }
        {
            // settings_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_panel = obj;
            lv_obj_set_pos(obj, 292, 357);
            lv_obj_set_size(obj, 195, 194);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_roller_create(parent_obj);
            lv_obj_set_pos(obj, 675, 367);
            lv_obj_set_size(obj, 80, 100);
            lv_roller_set_options(obj, "Option 1\nOption 2\nOption 3", LV_ROLLER_MODE_NORMAL);
        }
        {
            // lbl_time
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_time = obj;
            lv_obj_set_pos(obj, 560, 278);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<Time>");
        }
        {
            lv_obj_t *obj = lv_canvas_create(parent_obj);
            lv_obj_set_pos(obj, 36, 357);
            lv_obj_set_size(obj, 180, 101);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 60, 18);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_settings);
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}
