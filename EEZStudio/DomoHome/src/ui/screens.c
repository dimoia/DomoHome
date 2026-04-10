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

screen_main_state_t screen_main_state;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_heating_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.heating_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    lv_obj_add_event_cb(obj, action_heating_screen_config, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_grad_stop(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // btn_goto_home_from_heater_screen
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_goto_home_from_heater_screen = obj;
            lv_obj_set_pos(obj, 484, 4);
            lv_obj_set_size(obj, 37, 33);
            lv_obj_add_event_cb(obj, action_goto_home, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_img_src(obj, &img_ico_main_home, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // btn_goto_home_1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.btn_goto_home_1 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_ico_main_home, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // lbl_time_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_time_1 = obj;
            lv_obj_set_pos(obj, 910, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<Time>");
        }
        {
            // lbl_home_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_home_1 = obj;
            lv_obj_set_pos(obj, 519, 10);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffbfbfb), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "HOME");
        }
        {
            // lbl_strDate_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_str_date_1 = obj;
            lv_obj_set_pos(obj, 18, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<used to set date>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 19, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Monday");
        }
        {
            // drop_day_monday
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_monday = obj;
            lv_obj_set_pos(obj, 146, 75);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_heating_sw, LV_EVENT_VALUE_CHANGED, (void *)1);
            lv_obj_add_event_cb(obj, action_heating_sw_init, LV_EVENT_SCREEN_LOADED, (void *)1);
        }
        {
            // slider_temp_monday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_monday = obj;
            lv_obj_set_pos(obj, 403, 90);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_heating_slider_temperature, LV_EVENT_VALUE_CHANGED, (void *)1);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // slider_timer_monday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_timer_monday = obj;
            lv_obj_set_pos(obj, 723, 90);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_add_event_cb(obj, action_heating_slider_timer, LV_EVENT_VALUE_CHANGED, (void *)1);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // monday_time_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.monday_time_min = obj;
            lv_obj_set_pos(obj, 692, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // monday_time_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.monday_time_max = obj;
            lv_obj_set_pos(obj, 964, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            // monday_temp_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.monday_temp_min = obj;
            lv_obj_set_pos(obj, 368, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // monday_temp_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.monday_temp_max = obj;
            lv_obj_set_pos(obj, 637, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 8, 49);
            lv_obj_set_size(obj, 1007, 472);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff63801), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 17, 144);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Tuesday");
        }
        {
            // drop_day_tuesday
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_tuesday = obj;
            lv_obj_set_pos(obj, 146, 135);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_heating_sw, LV_EVENT_VALUE_CHANGED, (void *)1);
        }
        {
            // slider_temp_tuesday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_tuesday = obj;
            lv_obj_set_pos(obj, 403, 149);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_heating_slider_temperature, LV_EVENT_VALUE_CHANGED, (void *)2);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // slider_timer_tuesday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_timer_tuesday = obj;
            lv_obj_set_pos(obj, 723, 149);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_add_event_cb(obj, action_heating_slider_timer, LV_EVENT_VALUE_CHANGED, (void *)1);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // tuesday_time_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.tuesday_time_min = obj;
            lv_obj_set_pos(obj, 692, 143);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // tuesday_time_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.tuesday_time_max = obj;
            lv_obj_set_pos(obj, 964, 143);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            // tuesday_temp_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.tuesday_temp_min = obj;
            lv_obj_set_pos(obj, 368, 143);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // tuesday_temp_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.tuesday_temp_max = obj;
            lv_obj_set_pos(obj, 637, 143);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 19, 204);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Wednesday");
        }
        {
            // drop_day_wednesday
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_wednesday = obj;
            lv_obj_set_pos(obj, 146, 195);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_heating_sw, LV_EVENT_VALUE_CHANGED, (void *)3);
        }
        {
            // slider_temp_wednesday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_wednesday = obj;
            lv_obj_set_pos(obj, 407, 209);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_heating_slider_temperature, LV_EVENT_VALUE_CHANGED, (void *)3);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // slider_timer_wednesday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_timer_wednesday = obj;
            lv_obj_set_pos(obj, 723, 209);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_add_event_cb(obj, action_heating_slider_timer, LV_EVENT_VALUE_CHANGED, (void *)3);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // wednesday_time_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.wednesday_time_min = obj;
            lv_obj_set_pos(obj, 692, 203);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // wednesday_time_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.wednesday_time_max = obj;
            lv_obj_set_pos(obj, 964, 203);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            // wednesday_temp_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.wednesday_temp_min = obj;
            lv_obj_set_pos(obj, 368, 203);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // wednesday_temp_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.wednesday_temp_max = obj;
            lv_obj_set_pos(obj, 637, 203);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj4 = obj;
            lv_obj_set_pos(obj, 17, 263);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Thursday");
        }
        {
            // drop_day_thursday
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_thursday = obj;
            lv_obj_set_pos(obj, 146, 255);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_heating_sw, LV_EVENT_VALUE_CHANGED, (void *)4);
        }
        {
            // slider_temp_thursday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_thursday = obj;
            lv_obj_set_pos(obj, 403, 269);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_heating_slider_temperature, LV_EVENT_VALUE_CHANGED, (void *)4);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // slider_timer_thursday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_timer_thursday = obj;
            lv_obj_set_pos(obj, 723, 269);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_add_event_cb(obj, action_heating_slider_timer, LV_EVENT_VALUE_CHANGED, (void *)4);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // thursday_time_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.thursday_time_min = obj;
            lv_obj_set_pos(obj, 692, 263);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // thursday_time_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.thursday_time_max = obj;
            lv_obj_set_pos(obj, 964, 263);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            // thursday_temp_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.thursday_temp_min = obj;
            lv_obj_set_pos(obj, 368, 263);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // thursday_temp_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.thursday_temp_max = obj;
            lv_obj_set_pos(obj, 637, 263);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj5 = obj;
            lv_obj_set_pos(obj, 18, 323);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Friday");
        }
        {
            // drop_day_friday
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_friday = obj;
            lv_obj_set_pos(obj, 146, 314);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_heating_sw, LV_EVENT_VALUE_CHANGED, (void *)5);
        }
        {
            // slider_temp_friday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_friday = obj;
            lv_obj_set_pos(obj, 403, 326);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_heating_slider_temperature, LV_EVENT_VALUE_CHANGED, (void *)5);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // slider_timer_friday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_timer_friday = obj;
            lv_obj_set_pos(obj, 723, 326);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_add_event_cb(obj, action_heating_slider_timer, LV_EVENT_VALUE_CHANGED, (void *)5);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // friday_time_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.friday_time_min = obj;
            lv_obj_set_pos(obj, 692, 320);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // friday_time_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.friday_time_max = obj;
            lv_obj_set_pos(obj, 964, 320);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            // friday_temp_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.friday_temp_min = obj;
            lv_obj_set_pos(obj, 368, 320);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // friday_temp_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.friday_temp_max = obj;
            lv_obj_set_pos(obj, 637, 320);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj6 = obj;
            lv_obj_set_pos(obj, 19, 383);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Saturday");
        }
        {
            // drop_day_saturday
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_saturday = obj;
            lv_obj_set_pos(obj, 146, 374);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_heating_sw, LV_EVENT_VALUE_CHANGED, (void *)6);
        }
        {
            // slider_temp_saturday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_saturday = obj;
            lv_obj_set_pos(obj, 403, 387);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_heating_slider_temperature, LV_EVENT_SCROLL, (void *)6);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // slider_timer_saturday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_timer_saturday = obj;
            lv_obj_set_pos(obj, 723, 387);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_add_event_cb(obj, action_heating_slider_timer, LV_EVENT_VALUE_CHANGED, (void *)6);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // saturday_time_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.saturday_time_min = obj;
            lv_obj_set_pos(obj, 692, 381);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // saturday_time_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.saturday_time_max = obj;
            lv_obj_set_pos(obj, 964, 381);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            // saturday_temp_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.saturday_temp_min = obj;
            lv_obj_set_pos(obj, 368, 381);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // saturday_temp_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.saturday_temp_max = obj;
            lv_obj_set_pos(obj, 637, 381);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj7 = obj;
            lv_obj_set_pos(obj, 17, 443);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Sunday");
        }
        {
            // drop_day_sunday
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_sunday = obj;
            lv_obj_set_pos(obj, 146, 434);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_heating_sw, LV_EVENT_VALUE_CHANGED, (void *)7);
        }
        {
            // slider_temp_sunday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_sunday = obj;
            lv_obj_set_pos(obj, 403, 446);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_heating_slider_temperature, LV_EVENT_VALUE_CHANGED, (void *)7);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // slider_timer_sunday
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_timer_sunday = obj;
            lv_obj_set_pos(obj, 723, 446);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_add_event_cb(obj, action_heating_slider_timer, LV_EVENT_VALUE_CHANGED, (void *)7);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // sunday_time_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.sunday_time_min = obj;
            lv_obj_set_pos(obj, 692, 440);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // sunday_time_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.sunday_time_max = obj;
            lv_obj_set_pos(obj, 964, 440);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            // sunday_temp_min
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.sunday_temp_min = obj;
            lv_obj_set_pos(obj, 368, 440);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            // sunday_temp_max
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.sunday_temp_max = obj;
            lv_obj_set_pos(obj, 637, 440);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 403, 530);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_heating_save_config, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Save Config");
                }
            }
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 562, 530);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_heating_reset_config, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Reset");
                }
            }
        }
    }
    
    tick_screen_heating_screen();
}

void tick_screen_heating_screen() {
}

void create_screen_main() {
    screen_main_state_t *state = &screen_main_state;
    (void)state;
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 509, 5);
            lv_obj_set_size(obj, 37, 33);
            lv_obj_set_style_bg_img_src(obj, &img_ico_main_home, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // btn_goto_home
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.btn_goto_home = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_ico_main_home, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // lbl_home
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_home = obj;
            lv_obj_set_pos(obj, 549, 10);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffbfbfb), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "HOME");
        }
        {
            // lbl_time
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_time = obj;
            lv_obj_set_pos(obj, 930, 574);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<Time>");
        }
        {
            // btn_heating_Screen
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_heating_screen = obj;
            lv_obj_set_pos(obj, 17, 54);
            lv_obj_set_size(obj, 128, 128);
            lv_obj_add_event_cb(obj, action_goto_heating_screen, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_img_src(obj, &img_ico_heating_64x64, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff00ca15), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj8 = obj;
                    lv_obj_set_pos(obj, 0, 50);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff00fff4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_SUBTRACTIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Heating");
                }
            }
        }
        {
            // btn_settings_Screen
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_settings_screen = obj;
            lv_obj_set_pos(obj, 18, 367);
            lv_obj_set_size(obj, 128, 128);
            lv_obj_add_event_cb(obj, action_goto_settings_screen, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_img_src(obj, &img_ico_settings, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // lbl_home_3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_home_3 = obj;
                    lv_obj_set_pos(obj, -19, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xfffbfbfb), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "DomoHome");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -1, 45);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Settings");
                }
            }
        }
        {
            // lbl_strDate
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_str_date = obj;
            lv_obj_set_pos(obj, 18, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<used to set date>");
        }
        {
            // btn_energy_power
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_energy_power = obj;
            lv_obj_set_pos(obj, 17, 209);
            lv_obj_set_size(obj, 128, 128);
            lv_obj_set_style_bg_img_src(obj, &img_ico_energy_power_64x64, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 48);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Power");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -1, -46);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Energy");
                }
            }
        }
        {
            // meter_temp
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.meter_temp = obj;
            lv_obj_set_pos(obj, 163, 83);
            lv_obj_set_size(obj, 400, 400);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                state->scale = scale;
                lv_meter_set_scale_ticks(obj, scale, 40, 15, 9, lv_color_hex(0xfff84444));
                lv_meter_set_scale_major_ticks(obj, scale, 4, 9, 9, lv_color_hex(0xfffff174), 20);
                lv_meter_set_scale_range(obj, scale, 0, 40, 180, 180);
                {
                    state->indicator = lv_meter_add_needle_img(obj, scale, &img_img_ball_temp_indicator, -167, 5);
                    lv_meter_set_indicator_value(obj, state->indicator, 0);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffd4fcfe), lv_color_hex(0xffb3e3f4), false, 0);
                    state->indicator1 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 25);
                    lv_meter_set_indicator_end_value(obj, indicator, -4);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffb3e3f4), lv_color_hex(0xff38def7), true, 0);
                    state->indicator2 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, -3);
                    lv_meter_set_indicator_end_value(obj, indicator, 2);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff38def7), lv_color_hex(0xff00b1e5), true, 0);
                    state->indicator3 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 3);
                    lv_meter_set_indicator_end_value(obj, indicator, 8);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff00b1e5), lv_color_hex(0xfffad475), true, 0);
                    state->indicator4 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 9);
                    lv_meter_set_indicator_end_value(obj, indicator, 14);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xfffad475), lv_color_hex(0xffffa932), false, 0);
                    state->indicator5 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 15);
                    lv_meter_set_indicator_end_value(obj, indicator, 20);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffffa932), lv_color_hex(0xffd17000), true, 0);
                    state->indicator6 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 21);
                    lv_meter_set_indicator_end_value(obj, indicator, 25);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffd17000), lv_color_hex(0xffff4820), true, 0);
                    state->indicator7 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 26);
                    lv_meter_set_indicator_end_value(obj, indicator, 32);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffff4820), lv_color_hex(0xffac0000), true, 0);
                    state->indicator8 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 33);
                    lv_meter_set_indicator_end_value(obj, indicator, 38);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffac0000), lv_color_hex(0xff620000), true, 0);
                    state->indicator9 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 39);
                    lv_meter_set_indicator_end_value(obj, indicator, 50);
                }
            }
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xff00ca15), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff7d7d7d), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_TICKS | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 145, 282);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_temp);
                }
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, -7, 193);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_low_temperature);
                }
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 334, 193);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_high_temp);
                }
                {
                    // lbl_tempF
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temp_f = obj;
                    lv_obj_set_pos(obj, 206, 200);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "°F");
                }
                {
                    // Lbl_temp_value_farheneit
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temp_value_farheneit = obj;
                    lv_obj_set_pos(obj, 104, 202);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "00.0");
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj9 = obj;
                    lv_obj_set_pos(obj, 97, 92);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 0 },
                        { 160, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffff4343), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj10 = obj;
                    lv_obj_set_pos(obj, 106, 188);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 0 },
                        { 150, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xffff2828), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xffff5252), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // lbl_temp
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temp = obj;
                    lv_obj_set_pos(obj, 200, 122);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "°C");
                }
                {
                    // Lbl_temp_value
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temp_value = obj;
                    lv_obj_set_pos(obj, 101, 121);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "00.0");
                }
                {
                    // Lbl_temperature
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temperature = obj;
                    lv_obj_set_pos(obj, 97, 65);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Temperature");
                }
            }
        }
        {
            // meter_humidity
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.meter_humidity = obj;
            lv_obj_set_pos(obj, 605, 83);
            lv_obj_set_size(obj, 400, 400);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                state->scale1 = scale;
                lv_meter_set_scale_ticks(obj, scale, 50, 10, 9, lv_color_hex(0xff050505));
                lv_meter_set_scale_major_ticks(obj, scale, 5, 10, 8, lv_color_hex(0xff000000), 20);
                lv_meter_set_scale_range(obj, scale, 0, 100, 180, 180);
                {
                    state->indicator10 = lv_meter_add_needle_img(obj, scale, &img_img_ball_humidity_16_16, -167, 6);
                    lv_meter_set_indicator_value(obj, state->indicator10, 0);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xfff86034), lv_color_hex(0xfffaa992), false, 0);
                    state->indicator11 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 0);
                    lv_meter_set_indicator_end_value(obj, indicator, 35);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff6ff789), lv_color_hex(0xff00de49), true, 0);
                    state->indicator12 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 36);
                    lv_meter_set_indicator_end_value(obj, indicator, 65);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff3dcbfe), lv_color_hex(0xff0089ba), true, 0);
                    state->indicator13 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 66);
                    lv_meter_set_indicator_end_value(obj, indicator, 100);
                }
            }
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff7d7d7d), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_TICKS | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 146, 165);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_humidity);
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.obj11 = obj;
                    lv_obj_set_pos(obj, 118, 92);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 0 },
                        { 130, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    lv_obj_set_style_line_color(obj, lv_color_hex(0xff44abff), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, -11, 192);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_low_humidity_32_32);
                }
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 341, 197);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_humidity_64_64);
                }
                {
                    // lbl_humidity_percent
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_humidity_percent = obj;
                    lv_obj_set_pos(obj, 221, 113);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "%");
                }
                {
                    // Lbl_humi_value
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_humi_value = obj;
                    lv_obj_set_pos(obj, 118, 113);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_44, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "00.0");
                }
                {
                    // Lbl_humidity
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_humidity = obj;
                    lv_obj_set_pos(obj, 126, 65);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Humidity");
                }
            }
        }
        {
            // meter_pressure
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.meter_pressure = obj;
            lv_obj_set_pos(obj, 465, 335);
            lv_obj_set_size(obj, 250, 250);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                state->scale2 = scale;
                lv_meter_set_scale_ticks(obj, scale, 41, 1, 5, lv_color_hex(0xffffffff));
                lv_meter_set_scale_major_ticks(obj, scale, 8, 3, 10, lv_color_hex(0xff55f800), 15);
                lv_meter_set_scale_range(obj, scale, 870, 1095, 180, 120);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 3, lv_color_hex(0xffa6a6a6), -20);
                    state->indicator14 = indicator;
                    lv_meter_set_indicator_value(obj, indicator, 870);
                }
            }
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 86, 138);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_src(obj, &img_ico_pressure);
                    lv_img_set_zoom(obj, 200);
                }
                {
                    // lbl_press_hpa
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_press_hpa = obj;
                    lv_obj_set_pos(obj, 145, 111);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "hPa");
                }
                {
                    // Lbl_pressure
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_pressure = obj;
                    lv_obj_set_pos(obj, 54, 55);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Pressure");
                }
                {
                    // Lbl_press_value
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_press_value = obj;
                    lv_obj_set_pos(obj, 145, 84);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "0000");
                }
            }
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 745, 348);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_img_meteo_sun_clougy);
        }
        {
            // img_wifi
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.img_wifi = obj;
            lv_obj_set_pos(obj, 988, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_img_wifi_off);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 904, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_img_battery_not_present);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 938, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_img_bluetooth_disabled);
            lv_img_set_zoom(obj, 200);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 861, 6);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_img_sdcard);
            lv_img_set_angle(obj, 1);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    screen_main_state_t *state = &screen_main_state;
    (void)state;
}

void create_screen_settings_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    lv_obj_add_event_cb(obj, action_settings_screen_cb, LV_EVENT_SCREEN_LOADED, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // btn_goto_home_from_config_screen
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_goto_home_from_config_screen = obj;
            lv_obj_set_pos(obj, 484, 4);
            lv_obj_set_size(obj, 37, 33);
            lv_obj_add_event_cb(obj, action_goto_home, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_img_src(obj, &img_ico_main_home, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // btn_goto_home_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.btn_goto_home_2 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_ico_main_home, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // lbl_home_2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_home_2 = obj;
            lv_obj_set_pos(obj, 519, 10);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffbfbfb), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "HOME");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj12 = obj;
            lv_obj_set_pos(obj, 19, 252);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00a4ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Clock Setting");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 19, 283);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Day");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 92, 283);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Month");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 215, 283);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Year");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 309, 283);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Hour");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 390, 283);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Minute");
        }
        {
            // txt_ntp_server
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_ntp_server = obj;
            lv_obj_set_pos(obj, 775, 307);
            lv_obj_set_size(obj, 229, 42);
            lv_textarea_set_max_length(obj, 32);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_ntp_server_cb, LV_EVENT_FOCUSED, (void *)0);
            lv_obj_add_event_cb(obj, action_txt_ntp_server_cb, LV_EVENT_DEFOCUSED, (void *)0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 776, 283);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Ntp Server");
        }
        {
            // sw_manual_automatic_time
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.sw_manual_automatic_time = obj;
            lv_obj_set_pos(obj, 631, 316);
            lv_obj_set_size(obj, 56, 25);
            lv_obj_add_event_cb(obj, action_sw_manual_rtc_ntp_server, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // sw_ManualRTC_NtpServer
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.sw_manual_rtc_ntp_server = obj;
            lv_obj_set_pos(obj, 579, 319);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "RTC");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 698, 319);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Ntp");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 253, 48);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "WiFi  SSID");
        }
        {
            // drp_wifi_ssid
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drp_wifi_ssid = obj;
            lv_obj_set_pos(obj, 250, 75);
            lv_obj_set_size(obj, 343, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_ssid_select, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // txt_wifiPsw
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_wifi_psw = obj;
            lv_obj_set_pos(obj, 606, 75);
            lv_obj_set_size(obj, 281, 42);
            lv_textarea_set_max_length(obj, 32);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, true);
            lv_obj_add_event_cb(obj, action_wifi_txt_psw, LV_EVENT_FOCUSED, (void *)0);
            lv_obj_add_event_cb(obj, action_wifi_txt_psw, LV_EVENT_DEFOCUSED, (void *)0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 606, 48);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "WiFi Password");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj13 = obj;
            lv_obj_set_pos(obj, 18, 48);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00a4ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Hostname");
        }
        {
            // txt_hostname
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_hostname = obj;
            lv_obj_set_pos(obj, 18, 75);
            lv_obj_set_size(obj, 224, 42);
            lv_textarea_set_max_length(obj, 16);
            lv_textarea_set_placeholder_text(obj, "Termostato");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_hostname_cb, LV_EVENT_FOCUSED, (void *)0);
            lv_obj_add_event_cb(obj, action_txt_hostname_cb, LV_EVENT_DEFOCUSED, (void *)0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 19, 163);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "IPAddress");
        }
        {
            // txt_ipaddress
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_ipaddress = obj;
            lv_obj_set_pos(obj, 19, 188);
            lv_obj_set_size(obj, 130, 42);
            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
            lv_textarea_set_max_length(obj, 15);
            lv_textarea_set_placeholder_text(obj, "192.168.8.10");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)1);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)1);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 162, 163);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Netmask");
        }
        {
            // txt_netmask
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_netmask = obj;
            lv_obj_set_pos(obj, 163, 188);
            lv_obj_set_size(obj, 130, 42);
            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
            lv_textarea_set_max_length(obj, 15);
            lv_textarea_set_placeholder_text(obj, "255.255.255.0");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)2);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)2);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 309, 163);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Gateway");
        }
        {
            // txt_gateway
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_gateway = obj;
            lv_obj_set_pos(obj, 310, 188);
            lv_obj_set_size(obj, 130, 42);
            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
            lv_textarea_set_max_length(obj, 15);
            lv_textarea_set_placeholder_text(obj, "192.168.8.1");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)3);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)3);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // sw_staticIP_DynamicIP
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.sw_static_ip_dynamic_ip = obj;
            lv_obj_set_pos(obj, 550, 196);
            lv_obj_set_size(obj, 56, 25);
            lv_obj_add_event_cb(obj, action_sw_static_dynamic_ip, LV_EVENT_VALUE_CHANGED, (void *)0);
            lv_obj_add_state(obj, LV_STATE_CHECKED);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 616, 198);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Static IP");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 480, 198);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "DHCP");
        }
        {
            // btn_wifi_scann
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_wifi_scann = obj;
            lv_obj_set_pos(obj, 913, 75);
            lv_obj_set_size(obj, 100, 42);
            lv_obj_add_event_cb(obj, action_wifi_scann, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Scan");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 224, 385);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Weather Server");
        }
        {
            // btn_set_clock
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_set_clock = obj;
            lv_obj_set_pos(obj, 480, 305);
            lv_obj_set_size(obj, 81, 42);
            lv_obj_add_event_cb(obj, action_btn_real_time_set_clock_cb, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 1, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Set Time");
                }
            }
        }
        {
            // kek_keyboard
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.kek_keyboard = obj;
            lv_obj_set_pos(obj, 793, 220);
            lv_obj_set_size(obj, 201, 120);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj14 = obj;
            lv_obj_set_pos(obj, 20, 133);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00a4ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "NET");
        }
        {
            // btn_save_to_flash
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_save_to_flash = obj;
            lv_obj_set_pos(obj, 910, 134);
            lv_obj_set_size(obj, 103, 42);
            lv_obj_add_event_cb(obj, action_save_to_flash, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 1, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_add_event_cb(obj, action_save_to_flash, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Save To Flash");
                }
            }
        }
        {
            // btn_erase_flash
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_erase_flash = obj;
            lv_obj_set_pos(obj, 911, 191);
            lv_obj_set_size(obj, 102, 42);
            lv_obj_add_event_cb(obj, action_btn_erase_flash, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, -1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Erase Flash");
                }
            }
        }
        {
            // drop_day
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day = obj;
            lv_obj_set_pos(obj, 19, 305);
            lv_obj_set_size(obj, 62, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)1);
        }
        {
            // drop_Month
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_month = obj;
            lv_obj_set_pos(obj, 91, 305);
            lv_obj_set_size(obj, 115, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "January\nFebruary\nMarch\nApril\nMay\nJune\nJuly\nAugust\nSeptember\nOctober\nNovember\nDecember");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)2);
        }
        {
            // drop_Year
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_year = obj;
            lv_obj_set_pos(obj, 213, 305);
            lv_obj_set_size(obj, 81, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "2025\n2026\n2027\n2028\n2029\n2030\n2031\n2032\n2033\n2034\n2035\n2036\n2037\n2038\n2039\n2040");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)3);
        }
        {
            // drop_hour
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_hour = obj;
            lv_obj_set_pos(obj, 309, 307);
            lv_obj_set_size(obj, 66, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)4);
        }
        {
            // drop_minute
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_minute = obj;
            lv_obj_set_pos(obj, 389, 307);
            lv_obj_set_size(obj, 66, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)5);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 277, 464);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "BrokerIP");
        }
        {
            // txt_brokerIP
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_broker_ip = obj;
            lv_obj_set_pos(obj, 378, 450);
            lv_obj_set_size(obj, 136, 42);
            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
            lv_textarea_set_max_length(obj, 16);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_FOCUSED, (void *)0);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 279, 516);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "UserID");
        }
        {
            // txt_mqttuserid
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_mqttuserid = obj;
            lv_obj_set_pos(obj, 375, 499);
            lv_obj_set_size(obj, 253, 42);
            lv_textarea_set_max_length(obj, 16);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_FOCUSED, (void *)4);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_DEFOCUSED, (void *)4);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 264, 559);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Password");
        }
        {
            // txt_mqttpassword
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_mqttpassword = obj;
            lv_obj_set_pos(obj, 376, 549);
            lv_obj_set_size(obj, 253, 42);
            lv_textarea_set_max_length(obj, 16);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, true);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_FOCUSED, (void *)3);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_DEFOCUSED, (void *)3);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 22, 558);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Client ID");
        }
        {
            // txt_mqttclientID
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_mqttclient_id = obj;
            lv_obj_set_pos(obj, 117, 548);
            lv_obj_set_size(obj, 136, 42);
            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,A,B,C,D,E,F");
            lv_textarea_set_max_length(obj, 6);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_FOCUSED, (void *)2);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_DEFOCUSED, (void *)2);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // combo_weather_server
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.combo_weather_server = obj;
            lv_obj_set_pos(obj, 387, 375);
            lv_obj_set_size(obj, 174, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "OpenWeatherMap\nWeatherAPI\nAccuWeather");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_combo_weather_server, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj15 = obj;
            lv_obj_set_pos(obj, 335, 417);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffdf900), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Home Assistant Mqtt Configuration");
        }
        {
            // btn_restart
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_restart = obj;
            lv_obj_set_pos(obj, 911, 252);
            lv_obj_set_size(obj, 102, 42);
            lv_obj_add_event_cb(obj, action_btn_restart, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Restart");
                }
            }
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.obj16 = obj;
            lv_obj_set_pos(obj, 21, 423);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_t line_points[] = {
                { 0, 4 },
                { 280, 4 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.obj17 = obj;
            lv_obj_set_pos(obj, 727, 423);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_t line_points[] = {
                { 0, 4 },
                { 280, 4 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 18, 505);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Port");
        }
        {
            // txt_brokerPort
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_broker_port = obj;
            lv_obj_set_pos(obj, 117, 499);
            lv_obj_set_size(obj, 63, 42);
            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
            lv_textarea_set_max_length(obj, 5);
            lv_textarea_set_placeholder_text(obj, "1886");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_broker_port, LV_EVENT_FOCUSED, (void *)0);
            lv_obj_add_event_cb(obj, action_txt_broker_port, LV_EVENT_DEFOCUSED, (void *)0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // txt_weather_key
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_weather_key = obj;
            lv_obj_set_pos(obj, 747, 375);
            lv_obj_set_size(obj, 253, 42);
            lv_textarea_set_max_length(obj, 16);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_weather_key, LV_EVENT_FOCUSED, (void *)4);
            lv_obj_add_event_cb(obj, action_txt_weather_key, LV_EVENT_DEFOCUSED, (void *)4);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 570, 385);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Weather Api Key");
        }
        {
            // sw_mqtt_status
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.sw_mqtt_status = obj;
            lv_obj_set_pos(obj, 109, 449);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_add_event_cb(obj, action_sw_mqtt_status, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 18, 449);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Disable");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 171, 449);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Enable");
        }
        {
            // txt_mqtttopic
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_mqtttopic = obj;
            lv_obj_set_pos(obj, 752, 450);
            lv_obj_set_size(obj, 253, 42);
            lv_textarea_set_max_length(obj, 16);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_FOCUSED, (void *)5);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_DEFOCUSED, (void *)5);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 677, 460);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Topic");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 638, 527);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Subscribe");
        }
        {
            // txt_mqttsubscribe
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_mqttsubscribe = obj;
            lv_obj_set_pos(obj, 752, 516);
            lv_obj_set_size(obj, 253, 42);
            lv_textarea_set_max_length(obj, 16);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_FOCUSED, (void *)6);
            lv_obj_add_event_cb(obj, action_mqtt_config, LV_EVENT_DEFOCUSED, (void *)6);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // sw_wheater
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.sw_wheater = obj;
            lv_obj_set_pos(obj, 66, 384);
            lv_obj_set_size(obj, 56, 25);
            lv_obj_add_event_cb(obj, action_sw_wheater, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 14, 387);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "OFF");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 134, 387);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "ON");
        }
    }
    
    tick_screen_settings_screen();
}

void tick_screen_settings_screen() {
}

void create_screen_config_termostato() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.config_termostato = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    {
        lv_obj_t *parent_obj = obj;
        {
            // chart_humy
            lv_obj_t *obj = lv_chart_create(parent_obj);
            objects.chart_humy = obj;
            lv_obj_set_pos(obj, 1050, 469);
            lv_obj_set_size(obj, 209, 80);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff010000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff7f0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // tab_configuration
            lv_obj_t *obj = lv_tabview_create(parent_obj, LV_DIR_TOP, 32);
            objects.tab_configuration = obj;
            lv_obj_set_pos(obj, 12, 11);
            lv_obj_set_size(obj, 1003, 486);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // tab_wifi_config
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "WiFi Config");
                    objects.tab_wifi_config = obj;
                    lv_obj_add_event_cb(obj, action_tab_wifi_config, LV_EVENT_SCREEN_LOADED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // txt_tab_wifi_password
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.txt_tab_wifi_password = obj;
                            lv_obj_set_pos(obj, 168, 213);
                            lv_obj_set_size(obj, 343, 42);
                            lv_textarea_set_max_length(obj, 32);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, true);
                            lv_obj_add_event_cb(obj, action_txt_tab_wifi_password, LV_EVENT_FOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_txt_tab_wifi_password, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 2, 225);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "WiFi Password");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 2, 128);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "WiFi  SSID");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj18 = obj;
                            lv_obj_set_pos(obj, 0, 30);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00a4ff), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Hostname");
                        }
                        {
                            // drp_tab_wifi_ssid
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.drp_tab_wifi_ssid = obj;
                            lv_obj_set_pos(obj, 168, 118);
                            lv_obj_set_size(obj, 343, LV_SIZE_CONTENT);
                            lv_dropdown_set_options(obj, "");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_drp_tab_wifi_ssid, LV_EVENT_VALUE_CHANGED, (void *)0);
                        }
                        {
                            // txt_tab_wifi_hostname
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.txt_tab_wifi_hostname = obj;
                            lv_obj_set_pos(obj, 168, 20);
                            lv_obj_set_size(obj, 343, 42);
                            lv_textarea_set_max_length(obj, 16);
                            lv_textarea_set_placeholder_text(obj, "Termostato");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_tab_wifi_txt_hostname_cb, LV_EVENT_FOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_tab_wifi_txt_hostname_cb, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // btn_scann_tab_wifi_config
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.btn_scann_tab_wifi_config = obj;
                            lv_obj_set_pos(obj, 645, 16);
                            lv_obj_set_size(obj, 186, 50);
                            lv_obj_add_event_cb(obj, action_scann_wifi_network, LV_EVENT_CLICKED, (void *)0);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 0, 0);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Scann WiFi Networks");
                                }
                            }
                        }
                    }
                }
                {
                    // tab_network_config
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Network Config");
                    objects.tab_network_config = obj;
                    lv_obj_add_event_cb(obj, action_tab_network_config, LV_EVENT_SCREEN_LOADED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // txt_tab_net_dns
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.txt_tab_net_dns = obj;
                            lv_obj_set_pos(obj, 543, 135);
                            lv_obj_set_size(obj, 207, 42);
                            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
                            lv_textarea_set_max_length(obj, 15);
                            lv_textarea_set_placeholder_text(obj, "192.168.8.1");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)3);
                            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)3);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // txt_tab_net_ipaddress
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.txt_tab_net_ipaddress = obj;
                            lv_obj_set_pos(obj, 165, 65);
                            lv_obj_set_size(obj, 207, 42);
                            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
                            lv_textarea_set_max_length(obj, 15);
                            lv_textarea_set_placeholder_text(obj, "192.168.8.10");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)1);
                            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)1);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // sw_tab_net_staticIP_DynamicIP
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.sw_tab_net_static_ip_dynamic_ip = obj;
                            lv_obj_set_pos(obj, 539, 76);
                            lv_obj_set_size(obj, 56, 25);
                            lv_obj_add_event_cb(obj, action_sw_static_or_dynamic_ip, LV_EVENT_VALUE_CHANGED, (void *)0);
                            lv_obj_add_state(obj, LV_STATE_CHECKED);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 605, 78);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Static IP");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 469, 78);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "DHCP");
                        }
                        {
                            // txt_tab_net_gateway
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.txt_tab_net_gateway = obj;
                            lv_obj_set_pos(obj, 165, 212);
                            lv_obj_set_size(obj, 207, 42);
                            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
                            lv_textarea_set_max_length(obj, 15);
                            lv_textarea_set_placeholder_text(obj, "192.168.8.1");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)3);
                            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)3);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 8, 226);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Gateway");
                        }
                        {
                            // txt_tab_net_netmask
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.txt_tab_net_netmask = obj;
                            lv_obj_set_pos(obj, 165, 135);
                            lv_obj_set_size(obj, 207, 42);
                            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
                            lv_textarea_set_max_length(obj, 15);
                            lv_textarea_set_placeholder_text(obj, "255.255.255.0");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)2);
                            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)2);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 8, 146);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Netmask");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 8, 75);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "IPAddress");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 471, 149);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "DNS");
                        }
                    }
                }
                {
                    // tab_time_config
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Time/Weather Config");
                    objects.tab_time_config = obj;
                    lv_obj_add_event_cb(obj, action_tab_time_config, LV_EVENT_SCREEN_LOADED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.obj19 = obj;
                            lv_obj_set_pos(obj, 658, 275);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_t line_points[] = {
                                { 0, 4 },
                                { 280, 4 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.obj20 = obj;
                            lv_obj_set_pos(obj, 8, 274);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            static lv_point_t line_points[] = {
                                { 0, 4 },
                                { 280, 4 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            lv_obj_set_style_line_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_line_rounded(obj, true, LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.obj21 = obj;
                            lv_obj_set_pos(obj, 345, 269);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffdf900), LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Weather Configuration");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 290, 312);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Enable Weather");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 24, 310);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Disable Weather");
                        }
                        {
                            // tab_time_config_sw_wheater
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.tab_time_config_sw_wheater = obj;
                            lv_obj_set_pos(obj, 216, 310);
                            lv_obj_set_size(obj, 56, 25);
                            lv_obj_add_event_cb(obj, action_sw_wheater, LV_EVENT_VALUE_CHANGED, (void *)0);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 24, 382);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Weather Api Key");
                        }
                        {
                            // tab_time_txt_weather_key
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_time_txt_weather_key = obj;
                            lv_obj_set_pos(obj, 216, 372);
                            lv_obj_set_size(obj, 350, 42);
                            lv_textarea_set_max_length(obj, 16);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_tab_txt_weather_key, LV_EVENT_FOCUSED, (void *)4);
                            lv_obj_add_event_cb(obj, action_txt_weather_key, LV_EVENT_DEFOCUSED, (void *)4);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            // tab_time_combo_weather_server
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.tab_time_combo_weather_server = obj;
                            lv_obj_set_pos(obj, 781, 300);
                            lv_obj_set_size(obj, 174, LV_SIZE_CONTENT);
                            lv_dropdown_set_options(obj, "OpenWeatherMap\nWeatherAPI\nAccuWeather");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_sw_cmb_weather_server_selected, LV_EVENT_VALUE_CHANGED, (void *)0);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 334, 148);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Use Network ime Protocol");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 27, 145);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Use Real Time Clock");
                        }
                        {
                            // sw_tab_time_config_rtc_or_ntp
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.sw_tab_time_config_rtc_or_ntp = obj;
                            lv_obj_set_pos(obj, 253, 144);
                            lv_obj_set_size(obj, 56, 25);
                            lv_obj_add_event_cb(obj, action_sw_manual_rtc_ntp_server, LV_EVENT_VALUE_CHANGED, (void *)0);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 27, 212);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Network Time Protocol Server");
                        }
                        {
                            // tab_time_confiig_txt_ntp_server
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_time_confiig_txt_ntp_server = obj;
                            lv_obj_set_pos(obj, 374, 201);
                            lv_obj_set_size(obj, 308, 42);
                            lv_textarea_set_max_length(obj, 32);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_txt_ntp_server_cb, LV_EVENT_FOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_txt_ntp_server_cb, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 399, 41);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Minute");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 318, 41);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Hour");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 224, 41);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Year");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 101, 41);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Month");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 28, 41);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Day");
                        }
                        {
                            // drop_tab_time_config_minute
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.drop_tab_time_config_minute = obj;
                            lv_obj_set_pos(obj, 397, 70);
                            lv_obj_set_size(obj, 66, LV_SIZE_CONTENT);
                            lv_dropdown_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)5);
                        }
                        {
                            // drop_tab_time_config_hour
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.drop_tab_time_config_hour = obj;
                            lv_obj_set_pos(obj, 317, 70);
                            lv_obj_set_size(obj, 66, LV_SIZE_CONTENT);
                            lv_dropdown_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)4);
                        }
                        {
                            // drop_tab_time_config_year
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.drop_tab_time_config_year = obj;
                            lv_obj_set_pos(obj, 221, 68);
                            lv_obj_set_size(obj, 81, LV_SIZE_CONTENT);
                            lv_dropdown_set_options(obj, "2025\n2026\n2027\n2028\n2029\n2030\n2031\n2032\n2033\n2034\n2035\n2036\n2037\n2038\n2039\n2040");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)3);
                        }
                        {
                            // drop_tab_time_config_month
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.drop_tab_time_config_month = obj;
                            lv_obj_set_pos(obj, 99, 68);
                            lv_obj_set_size(obj, 115, LV_SIZE_CONTENT);
                            lv_dropdown_set_options(obj, "January\nFebruary\nMarch\nApril\nMay\nJune\nJuly\nAugust\nSeptember\nOctober\nNovember\nDecember");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)2);
                        }
                        {
                            // drop_tab_time_config_day
                            lv_obj_t *obj = lv_dropdown_create(parent_obj);
                            objects.drop_tab_time_config_day = obj;
                            lv_obj_set_pos(obj, 27, 68);
                            lv_obj_set_size(obj, 62, LV_SIZE_CONTENT);
                            lv_dropdown_set_options(obj, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31");
                            lv_dropdown_set_selected(obj, 0);
                            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)1);
                        }
                        {
                            // btn_tab_time_config_set_clock
                            lv_obj_t *obj = lv_btn_create(parent_obj);
                            objects.btn_tab_time_config_set_clock = obj;
                            lv_obj_set_pos(obj, 488, 68);
                            lv_obj_set_size(obj, 81, 42);
                            lv_obj_add_event_cb(obj, action_btn_real_time_set_clock_cb, LV_EVENT_CLICKED, (void *)0);
                            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    lv_obj_set_pos(obj, 1, 2);
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_label_set_text(obj, "Set Time");
                                }
                            }
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 601, 312);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Weather Server");
                        }
                    }
                }
                {
                    // tab_ha_config
                    lv_obj_t *obj = lv_tabview_add_tab(parent_obj, "Home Assistant Config");
                    objects.tab_ha_config = obj;
                    lv_obj_add_event_cb(obj, action_tab_ha_config, LV_EVENT_SCREEN_LOADED, (void *)0);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // tab_ha_txt_mqttsubscribe
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_ha_txt_mqttsubscribe = obj;
                            lv_obj_set_pos(obj, 259, 361);
                            lv_obj_set_size(obj, 307, 42);
                            lv_textarea_set_max_length(obj, 16);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_FOCUSED, (void *)6);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_DEFOCUSED, (void *)6);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 11, 371);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Mqtt Subscribe");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 11, 299);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Mqtt Topic");
                        }
                        {
                            // tab_ha_txt_mqtttopic
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_ha_txt_mqtttopic = obj;
                            lv_obj_set_pos(obj, 259, 289);
                            lv_obj_set_size(obj, 307, 42);
                            lv_textarea_set_max_length(obj, 16);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_FOCUSED, (void *)5);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_DEFOCUSED, (void *)5);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 11, 5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Enable Mqtt Protocol");
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 325, 5);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Disable Mqtt Protocol");
                        }
                        {
                            // tab_ha_sw_mqtt_status
                            lv_obj_t *obj = lv_switch_create(parent_obj);
                            objects.tab_ha_sw_mqtt_status = obj;
                            lv_obj_set_pos(obj, 246, 5);
                            lv_obj_set_size(obj, 50, 25);
                            lv_obj_add_event_cb(obj, action_sw_mqtt_protocol_on_off, LV_EVENT_VALUE_CHANGED, (void *)0);
                        }
                        {
                            // tab_ha_txt_broker_port
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_ha_txt_broker_port = obj;
                            lv_obj_set_pos(obj, 731, 78);
                            lv_obj_set_size(obj, 63, 42);
                            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
                            lv_textarea_set_max_length(obj, 5);
                            lv_textarea_set_placeholder_text(obj, "1886");
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_txt_broker_port, LV_EVENT_FOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_txt_broker_port, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 536, 88);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Broker Mqtt Port");
                        }
                        {
                            // tab_ha_txt_mqttclient_id
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_ha_txt_mqttclient_id = obj;
                            lv_obj_set_pos(obj, 257, 215);
                            lv_obj_set_size(obj, 242, 42);
                            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,A,B,C,D,E,F");
                            lv_textarea_set_max_length(obj, 6);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_FOCUSED, (void *)2);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_DEFOCUSED, (void *)2);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 11, 226);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Mqtt Client ID");
                        }
                        {
                            // tab_ha_txt_mqttpassword
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_ha_txt_mqttpassword = obj;
                            lv_obj_set_pos(obj, 668, 152);
                            lv_obj_set_size(obj, 253, 42);
                            lv_textarea_set_max_length(obj, 16);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, true);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_FOCUSED, (void *)3);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_DEFOCUSED, (void *)3);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 538, 160);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Password");
                        }
                        {
                            // tab_ha_txt_mqttuserid
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_ha_txt_mqttuserid = obj;
                            lv_obj_set_pos(obj, 260, 152);
                            lv_obj_set_size(obj, 239, 42);
                            lv_textarea_set_max_length(obj, 16);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_FOCUSED, (void *)4);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_DEFOCUSED, (void *)4);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 11, 159);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Mqtt UserID");
                        }
                        {
                            // tab_ha_txt_broker_ip
                            lv_obj_t *obj = lv_textarea_create(parent_obj);
                            objects.tab_ha_txt_broker_ip = obj;
                            lv_obj_set_pos(obj, 259, 78);
                            lv_obj_set_size(obj, 194, 42);
                            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
                            lv_textarea_set_max_length(obj, 16);
                            lv_textarea_set_one_line(obj, true);
                            lv_textarea_set_password_mode(obj, false);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_FOCUSED, (void *)0);
                            lv_obj_add_event_cb(obj, action_tab_mqtt_config, LV_EVENT_DEFOCUSED, (void *)0);
                            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
                        }
                        {
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 11, 86);
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Mqtt Broker Ip Address");
                        }
                    }
                }
            }
        }
        {
            // btn_save_to_nvs
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_save_to_nvs = obj;
            lv_obj_set_pos(obj, 265, 530);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_save_config_to_nvs, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Save To Flash");
                }
            }
        }
        {
            // btn_erase_nvs
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_erase_nvs = obj;
            lv_obj_set_pos(obj, 498, 530);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_erase_nvs_flash, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Erase Flash");
                }
            }
        }
        {
            // btn_restart_device
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_restart_device = obj;
            lv_obj_set_pos(obj, 730, 530);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_restart_micro, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Restart");
                }
            }
        }
        {
            // btn_config_termostato_return_to_home
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_config_termostato_return_to_home = obj;
            lv_obj_set_pos(obj, 16, 540);
            lv_obj_set_size(obj, 37, 33);
            lv_obj_add_event_cb(obj, action_goto_home, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_img_src(obj, &img_ico_main_home, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // btn_goto_home_3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.btn_goto_home_3 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_img_src(obj, &img_ico_main_home, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // lbl_home_4
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_home_4 = obj;
            lv_obj_set_pos(obj, 54, 546);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffbfbfb), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "HOME");
        }
    }
    
    tick_screen_config_termostato();
}

void tick_screen_config_termostato() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_heating_screen,
    tick_screen_main,
    tick_screen_settings_screen,
    tick_screen_config_termostato,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_heating_screen();
    create_screen_main();
    create_screen_settings_screen();
    create_screen_config_termostato();
}