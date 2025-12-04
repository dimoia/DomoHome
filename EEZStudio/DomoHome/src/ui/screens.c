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
static lv_meter_indicator_t * indicator3;
static lv_meter_indicator_t * indicator4;
static lv_meter_indicator_t * indicator5;
static lv_meter_indicator_t * indicator6;
static lv_meter_indicator_t * indicator7;
static lv_meter_indicator_t * indicator8;
static lv_meter_indicator_t * indicator9;
static lv_meter_scale_t * scale10;
static lv_meter_indicator_t * indicator11;
static lv_meter_indicator_t * indicator12;
static lv_meter_indicator_t * indicator13;
static lv_meter_scale_t * scale14;
static lv_meter_indicator_t * indicator15;
static lv_meter_scale_t * scale16;
static lv_meter_indicator_t * indicator17;
static lv_meter_scale_t * scale18;
static lv_meter_indicator_t * indicator19;
static lv_meter_indicator_t * indicator20;
static lv_meter_indicator_t * indicator21;
static lv_meter_indicator_t * indicator22;
static lv_meter_indicator_t * indicator23;
static lv_meter_indicator_t * indicator24;
static lv_meter_indicator_t * indicator25;
static lv_meter_indicator_t * indicator26;
static lv_meter_indicator_t * indicator27;
static lv_meter_indicator_t * indicator28;
static lv_meter_scale_t * scale29;
static lv_meter_indicator_t * indicator30;
static lv_meter_indicator_t * indicator31;
static lv_meter_indicator_t * indicator32;
static lv_meter_indicator_t * indicator33;
static lv_meter_scale_t * scale34;
static lv_meter_indicator_t * indicator35;
static lv_meter_indicator_t * indicator36;

void create_screen_settings_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    lv_obj_add_event_cb(obj, action_settings_screen_cb, LV_EVENT_SCREEN_LOADED, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // btn_goto_home_from_heater_screen_1
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_goto_home_from_heater_screen_1 = obj;
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
            // lbl_time_2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_time_2 = obj;
            lv_obj_set_pos(obj, 920, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_AUTO, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<Time>");
        }
        {
            // lbl_strDate_2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_str_date_2 = obj;
            lv_obj_set_pos(obj, 18, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<used to set date>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 15, 300);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00a4ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Clock Setting");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 331);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Day");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 88, 331);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Month");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 211, 331);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Year");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 305, 331);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Hour");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 386, 331);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Minute");
        }
        {
            // txt_ntp_server
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_ntp_server = obj;
            lv_obj_set_pos(obj, 467, 355);
            lv_obj_set_size(obj, 253, 42);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_ntp_server_cb, LV_EVENT_FOCUSED, (void *)0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 471, 331);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Ntp Server");
        }
        {
            // sw_manual_automatic_time
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.sw_manual_automatic_time = obj;
            lv_obj_set_pos(obj, 810, 364);
            lv_obj_set_size(obj, 56, 25);
            lv_obj_add_event_cb(obj, action_sw_manual_rtc_ntp_server, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // sw_ManualRTC_NtpServer
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.sw_manual_rtc_ntp_server = obj;
            lv_obj_set_pos(obj, 723, 366);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Manual");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 876, 366);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Ntp");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 399, 101);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "WiFi  SSID");
        }
        {
            // drp_wifi_ssid
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drp_wifi_ssid = obj;
            lv_obj_set_pos(obj, 516, 91);
            lv_obj_set_size(obj, 343, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_ssid_select, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // txt_wifiPsw
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_wifi_psw = obj;
            lv_obj_set_pos(obj, 516, 149);
            lv_obj_set_size(obj, 343, 42);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, true);
            lv_obj_add_event_cb(obj, action_wifi_txt_psw, LV_EVENT_FOCUSED, (void *)0);
            lv_obj_add_event_cb(obj, action_wifi_txt_psw, LV_EVENT_DEFOCUSED, (void *)0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 356, 159);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "WiFi Password");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 18, 64);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00a4ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Hostname");
        }
        {
            // txt_hostname
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_hostname = obj;
            lv_obj_set_pos(obj, 18, 91);
            lv_obj_set_size(obj, 224, 42);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_hostname_cb, LV_EVENT_FOCUSED, (void *)0);
            lv_obj_add_event_cb(obj, action_txt_hostname_cb, LV_EVENT_DEFOCUSED, (void *)0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 15, 212);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "IPAddress");
        }
        {
            // txt_ipaddress
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_ipaddress = obj;
            lv_obj_set_pos(obj, 15, 237);
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
            lv_obj_set_pos(obj, 158, 212);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Netmask");
        }
        {
            // txt_netmask
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_netmask = obj;
            lv_obj_set_pos(obj, 159, 237);
            lv_obj_set_size(obj, 130, 42);
            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
            lv_textarea_set_max_length(obj, 15);
            lv_textarea_set_placeholder_text(obj, "255.255.255.0");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)1);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)1);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 305, 212);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Gateway");
        }
        {
            // txt_gateway
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_gateway = obj;
            lv_obj_set_pos(obj, 306, 237);
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
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 457, 212);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "DNS");
        }
        {
            // txt_dns
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.txt_dns = obj;
            lv_obj_set_pos(obj, 459, 237);
            lv_obj_set_size(obj, 130, 42);
            lv_textarea_set_accepted_chars(obj, "0,1,2,3,4,5,6,7,8,9,0,.");
            lv_textarea_set_max_length(obj, 15);
            lv_textarea_set_placeholder_text(obj, "8.8.8.8");
            lv_textarea_set_one_line(obj, true);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_FOCUSED, (void *)4);
            lv_obj_add_event_cb(obj, action_txt_net_cb, LV_EVENT_DEFOCUSED, (void *)4);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // sw_staticIP_DynamicIP
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.sw_static_ip_dynamic_ip = obj;
            lv_obj_set_pos(obj, 692, 246);
            lv_obj_set_size(obj, 56, 25);
            lv_obj_add_event_cb(obj, action_sw_static_dynamic_ip, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 606, 247);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Static IP");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 751, 247);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Dynamic IP");
        }
        {
            // btn_wifi_scann
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_wifi_scann = obj;
            lv_obj_set_pos(obj, 895, 91);
            lv_obj_set_size(obj, 100, 42);
            lv_obj_add_event_cb(obj, action_wifi_scann, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Scan");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 17, 416);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "TimeZone");
        }
        {
            // btn_set_clock
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_set_clock = obj;
            lv_obj_set_pos(obj, 932, 355);
            lv_obj_set_size(obj, 81, 42);
            lv_obj_add_event_cb(obj, action_btn_real_time_set_clock_cb, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Set Clock");
                }
            }
        }
        {
            // btn_wifi_connect
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_wifi_connect = obj;
            lv_obj_set_pos(obj, 896, 149);
            lv_obj_set_size(obj, 100, 42);
            lv_obj_add_event_cb(obj, action_wifi_connect, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // btn_connect
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.btn_connect = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Try Connect");
                }
            }
        }
        {
            // kek_keyboard
            lv_obj_t *obj = lv_keyboard_create(parent_obj);
            objects.kek_keyboard = obj;
            lv_obj_set_pos(obj, 705, 464);
            lv_obj_set_size(obj, 190, 120);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 16, 182);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00a4ff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "NET");
        }
        {
            // btn_apply
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_apply = obj;
            lv_obj_set_pos(obj, 928, 542);
            lv_obj_set_size(obj, 81, 42);
            lv_obj_add_event_cb(obj, action_btn_apply, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Apply");
                }
            }
        }
        {
            // btn_cancell
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.btn_cancell = obj;
            lv_obj_set_pos(obj, 925, 482);
            lv_obj_set_size(obj, 81, 42);
            lv_obj_add_event_cb(obj, action_btn_cancell, LV_EVENT_CLICKED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Cancell");
                }
            }
        }
        {
            // drop_day
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day = obj;
            lv_obj_set_pos(obj, 15, 353);
            lv_obj_set_size(obj, 62, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)1);
        }
        {
            // drop_Month
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_month = obj;
            lv_obj_set_pos(obj, 87, 353);
            lv_obj_set_size(obj, 115, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "January\nFebruary\nMarch\nApril\nMay\nJune\nJuly\nAugust\nSeptember\nOctober\nNovember\nDecember");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)2);
        }
        {
            // drop_Year
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_year = obj;
            lv_obj_set_pos(obj, 209, 353);
            lv_obj_set_size(obj, 81, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "2025\n2026\n2027\n2028\n2029\n2030\n2031\n2032\n2033\n2034\n2035\n2036\n2037\n2038\n2039\n2040");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)3);
        }
        {
            // drop_hour
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_hour = obj;
            lv_obj_set_pos(obj, 305, 355);
            lv_obj_set_size(obj, 66, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)4);
        }
        {
            // drop_minute
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_minute = obj;
            lv_obj_set_pos(obj, 385, 355);
            lv_obj_set_size(obj, 66, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_add_event_cb(obj, action_drop_date_time, LV_EVENT_VALUE_CHANGED, (void *)5);
        }
    }
    
    tick_screen_settings_screen();
}

void tick_screen_settings_screen() {
}

void create_screen_main() {
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
                    objects.obj3 = obj;
                    lv_obj_set_pos(obj, 0, 50);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff00fff4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_SUBTRACTIVE, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_anim_speed(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
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
                scale0 = scale;
                lv_meter_set_scale_ticks(obj, scale, 40, 15, 9, lv_color_hex(0xfff84444));
                lv_meter_set_scale_major_ticks(obj, scale, 4, 9, 9, lv_color_hex(0xfffff174), 20);
                lv_meter_set_scale_range(obj, scale, 0, 40, 180, 180);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_img(obj, scale, &img_img_ball_temp_indicator, -167, 5);
                    lv_meter_set_indicator_value(obj, indicator, 0);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffd4fcfe), lv_color_hex(0xffb3e3f4), false, 0);
                    indicator1 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 25);
                    lv_meter_set_indicator_end_value(obj, indicator, -4);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffb3e3f4), lv_color_hex(0xff38def7), true, 0);
                    indicator2 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, -3);
                    lv_meter_set_indicator_end_value(obj, indicator, 2);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff38def7), lv_color_hex(0xff00b1e5), true, 0);
                    indicator3 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 3);
                    lv_meter_set_indicator_end_value(obj, indicator, 8);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff00b1e5), lv_color_hex(0xfffad475), true, 0);
                    indicator4 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 9);
                    lv_meter_set_indicator_end_value(obj, indicator, 14);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xfffad475), lv_color_hex(0xffffa932), false, 0);
                    indicator5 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 15);
                    lv_meter_set_indicator_end_value(obj, indicator, 20);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffffa932), lv_color_hex(0xffd17000), true, 0);
                    indicator6 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 21);
                    lv_meter_set_indicator_end_value(obj, indicator, 25);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffd17000), lv_color_hex(0xffff4820), true, 0);
                    indicator7 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 26);
                    lv_meter_set_indicator_end_value(obj, indicator, 32);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffff4820), lv_color_hex(0xffac0000), true, 0);
                    indicator8 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 33);
                    lv_meter_set_indicator_end_value(obj, indicator, 38);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffac0000), lv_color_hex(0xff620000), true, 0);
                    indicator9 = indicator;
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
                    objects.obj4 = obj;
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
                    objects.obj5 = obj;
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
                scale10 = scale;
                lv_meter_set_scale_ticks(obj, scale, 50, 10, 9, lv_color_hex(0xff050505));
                lv_meter_set_scale_major_ticks(obj, scale, 5, 10, 8, lv_color_hex(0xff000000), 20);
                lv_meter_set_scale_range(obj, scale, 0, 100, 180, 180);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_img(obj, scale, &img_img_ball_humidity_16_16, -167, 6);
                    lv_meter_set_indicator_value(obj, indicator, 0);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xfff86034), lv_color_hex(0xfffaa992), false, 0);
                    indicator11 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 0);
                    lv_meter_set_indicator_end_value(obj, indicator, 35);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff6ff789), lv_color_hex(0xff00de49), true, 0);
                    indicator12 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 36);
                    lv_meter_set_indicator_end_value(obj, indicator, 65);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff3dcbfe), lv_color_hex(0xff0089ba), true, 0);
                    indicator13 = indicator;
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
                    objects.obj6 = obj;
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
                scale14 = scale;
                lv_meter_set_scale_ticks(obj, scale, 41, 1, 5, lv_color_hex(0xffffffff));
                lv_meter_set_scale_major_ticks(obj, scale, 8, 3, 10, lv_color_hex(0xff55f800), 15);
                lv_meter_set_scale_range(obj, scale, 870, 1095, 180, 120);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 3, lv_color_hex(0xffa6a6a6), -20);
                    indicator15 = indicator;
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
        {
            // meter_pressure_1
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.meter_pressure_1 = obj;
            lv_obj_set_pos(obj, 503, 48);
            lv_obj_set_size(obj, 160, 160);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                scale16 = scale;
                lv_meter_set_scale_ticks(obj, scale, 41, 1, 5, lv_color_hex(0xffffffff));
                lv_meter_set_scale_major_ticks(obj, scale, 8, 3, 10, lv_color_hex(0xff55f800), 15);
                lv_meter_set_scale_range(obj, scale, 0, 100, 180, 120);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 3, lv_color_hex(0xffa6a6a6), -20);
                    indicator17 = indicator;
                    lv_meter_set_indicator_value(obj, indicator, 870);
                }
            }
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_width(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xfff9f9f9), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // lbl_press_hpa_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_press_hpa_2 = obj;
                    lv_obj_set_pos(obj, 48, 96);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Kw/h");
                }
                {
                    // Lbl_pressure_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_pressure_2 = obj;
                    lv_obj_set_pos(obj, 70, 45);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "PWR");
                }
                {
                    // Lbl_press_value_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_press_value_2 = obj;
                    lv_obj_set_pos(obj, 48, 71);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "0000");
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_heating_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.heating_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
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
            lv_obj_set_pos(obj, 920, 9);
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
            objects.obj7 = obj;
            lv_obj_set_pos(obj, 19, 85);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Monday");
        }
        {
            // dropDay_1
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_1 = obj;
            lv_obj_set_pos(obj, 150, 75);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // lblFromToTemp_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_temp_1 = obj;
            lv_obj_set_pos(obj, 462, 53);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            // sliderTemp_1
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_1 = obj;
            lv_obj_set_pos(obj, 407, 90);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_slider_temp1_change, LV_EVENT_SCROLL, (void *)0);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // liderTimer_1
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.lider_timer_1 = obj;
            lv_obj_set_pos(obj, 718, 90);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lblFromToTimer_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_timer_1 = obj;
            lv_obj_set_pos(obj, 775, 53);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj8 = obj;
            lv_obj_set_pos(obj, 691, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj9 = obj;
            lv_obj_set_pos(obj, 957, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj10 = obj;
            lv_obj_set_pos(obj, 377, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj11 = obj;
            lv_obj_set_pos(obj, 639, 84);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj12 = obj;
            lv_obj_set_pos(obj, 8, 42);
            lv_obj_set_size(obj, 1004, 541);
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
            objects.obj13 = obj;
            lv_obj_set_pos(obj, 17, 159);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Tuesday");
        }
        {
            // dropDay_2
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_2 = obj;
            lv_obj_set_pos(obj, 146, 149);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // lblFromToTemp_2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_temp_2 = obj;
            lv_obj_set_pos(obj, 460, 127);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            // sliderTemp_2
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_2 = obj;
            lv_obj_set_pos(obj, 403, 164);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_slider_temp1_change, LV_EVENT_SCROLL, (void *)0);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // liderTimer_2
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.lider_timer_2 = obj;
            lv_obj_set_pos(obj, 714, 164);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lblFromToTimer_2
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_timer_2 = obj;
            lv_obj_set_pos(obj, 773, 127);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj14 = obj;
            lv_obj_set_pos(obj, 687, 158);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj15 = obj;
            lv_obj_set_pos(obj, 953, 158);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj16 = obj;
            lv_obj_set_pos(obj, 373, 158);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj17 = obj;
            lv_obj_set_pos(obj, 635, 158);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj18 = obj;
            lv_obj_set_pos(obj, 19, 232);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Wednesday");
        }
        {
            // dropDay_3
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_3 = obj;
            lv_obj_set_pos(obj, 150, 222);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // lblFromToTemp_3
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_temp_3 = obj;
            lv_obj_set_pos(obj, 465, 200);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            // sliderTemp_3
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_3 = obj;
            lv_obj_set_pos(obj, 407, 237);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_slider_temp1_change, LV_EVENT_SCROLL, (void *)0);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // liderTimer_3
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.lider_timer_3 = obj;
            lv_obj_set_pos(obj, 718, 237);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lblFromToTimer_3
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_timer_3 = obj;
            lv_obj_set_pos(obj, 775, 200);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj19 = obj;
            lv_obj_set_pos(obj, 691, 231);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj20 = obj;
            lv_obj_set_pos(obj, 957, 231);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj21 = obj;
            lv_obj_set_pos(obj, 377, 231);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj22 = obj;
            lv_obj_set_pos(obj, 639, 231);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj23 = obj;
            lv_obj_set_pos(obj, 17, 309);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Thursday");
        }
        {
            // dropDay_4
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_4 = obj;
            lv_obj_set_pos(obj, 148, 299);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // lblFromToTemp_4
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_temp_4 = obj;
            lv_obj_set_pos(obj, 464, 277);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            // sliderTemp_4
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_4 = obj;
            lv_obj_set_pos(obj, 405, 314);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_slider_temp1_change, LV_EVENT_SCROLL, (void *)0);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // liderTimer_4
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.lider_timer_4 = obj;
            lv_obj_set_pos(obj, 716, 314);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lblFromToTimer_4
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_timer_4 = obj;
            lv_obj_set_pos(obj, 780, 277);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj24 = obj;
            lv_obj_set_pos(obj, 689, 308);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj25 = obj;
            lv_obj_set_pos(obj, 955, 308);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj26 = obj;
            lv_obj_set_pos(obj, 375, 308);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj27 = obj;
            lv_obj_set_pos(obj, 637, 308);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj28 = obj;
            lv_obj_set_pos(obj, 18, 384);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Friday");
        }
        {
            // dropDay_5
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_5 = obj;
            lv_obj_set_pos(obj, 149, 374);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // lblFromToTemp_5
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_temp_5 = obj;
            lv_obj_set_pos(obj, 465, 352);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            // sliderTemp_5
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_5 = obj;
            lv_obj_set_pos(obj, 406, 389);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_slider_temp1_change, LV_EVENT_SCROLL, (void *)0);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // liderTimer_5
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.lider_timer_5 = obj;
            lv_obj_set_pos(obj, 717, 389);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lblFromToTimer_5
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_timer_5 = obj;
            lv_obj_set_pos(obj, 774, 352);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj29 = obj;
            lv_obj_set_pos(obj, 690, 383);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj30 = obj;
            lv_obj_set_pos(obj, 956, 383);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj31 = obj;
            lv_obj_set_pos(obj, 376, 383);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj32 = obj;
            lv_obj_set_pos(obj, 638, 383);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj33 = obj;
            lv_obj_set_pos(obj, 19, 463);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Saturday");
        }
        {
            // dropDay_6
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_6 = obj;
            lv_obj_set_pos(obj, 150, 453);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // lblFromToTemp_6
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_temp_6 = obj;
            lv_obj_set_pos(obj, 468, 431);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            // sliderTemp_6
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_6 = obj;
            lv_obj_set_pos(obj, 407, 468);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_slider_temp1_change, LV_EVENT_SCROLL, (void *)0);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // liderTimer_6
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.lider_timer_6 = obj;
            lv_obj_set_pos(obj, 718, 468);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lblFromToTimer_6
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_timer_6 = obj;
            lv_obj_set_pos(obj, 777, 431);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj34 = obj;
            lv_obj_set_pos(obj, 691, 462);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj35 = obj;
            lv_obj_set_pos(obj, 957, 462);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj36 = obj;
            lv_obj_set_pos(obj, 377, 462);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj37 = obj;
            lv_obj_set_pos(obj, 639, 462);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj38 = obj;
            lv_obj_set_pos(obj, 17, 540);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Sunday");
        }
        {
            // dropDay_7
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.drop_day_7 = obj;
            lv_obj_set_pos(obj, 148, 530);
            lv_obj_set_size(obj, 194, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Off\nTemperature Select\nTimer Select");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // lblFromToTemp_7
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_temp_7 = obj;
            lv_obj_set_pos(obj, 466, 508);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            // sliderTemp_7
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.slider_temp_7 = obj;
            lv_obj_set_pos(obj, 405, 545);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 50);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 20, LV_ANIM_ON);
            lv_slider_set_left_value(obj, 0, LV_ANIM_ON);
            lv_obj_add_event_cb(obj, action_slider_temp1_change, LV_EVENT_SCROLL, (void *)0);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // liderTimer_7
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.lider_timer_7 = obj;
            lv_obj_set_pos(obj, 716, 545);
            lv_obj_set_size(obj, 216, 10);
            lv_slider_set_range(obj, 0, 23);
            lv_slider_set_mode(obj, LV_SLIDER_MODE_RANGE);
            lv_slider_set_value(obj, 23, LV_ANIM_OFF);
            lv_slider_set_left_value(obj, 0, LV_ANIM_OFF);
            lv_obj_set_style_outline_width(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_color(obj, lv_color_hex(0xffd1b14f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_outline_pad(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // lblFromToTimer_7
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_from_to_timer_7 = obj;
            lv_obj_set_pos(obj, 775, 508);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "<FromTo>");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj39 = obj;
            lv_obj_set_pos(obj, 689, 539);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj40 = obj;
            lv_obj_set_pos(obj, 955, 539);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "24");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj41 = obj;
            lv_obj_set_pos(obj, 375, 539);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff00ecf7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj42 = obj;
            lv_obj_set_pos(obj, 637, 539);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
    }
    
    tick_screen_heating_screen();
}

void tick_screen_heating_screen() {
}

void create_screen_test() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.test = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 1024, 600);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.obj43 = obj;
            lv_obj_set_pos(obj, 367, 0);
            lv_obj_set_size(obj, 309, 309);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                scale18 = scale;
                lv_meter_set_scale_ticks(obj, scale, 50, 10, 8, lv_color_hex(0xfff84444));
                lv_meter_set_scale_major_ticks(obj, scale, 5, 3, 10, lv_color_hex(0xfffff174), 15);
                lv_meter_set_scale_range(obj, scale, -10, 50, 180, 180);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 5, lv_color_hex(0xfff5d833), 0);
                    indicator19 = indicator;
                    lv_meter_set_indicator_value(obj, indicator, 25);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffd4fcfe), lv_color_hex(0xffb3e3f4), false, 0);
                    indicator20 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, -10);
                    lv_meter_set_indicator_end_value(obj, indicator, -4);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffb3e3f4), lv_color_hex(0xff38def7), true, 0);
                    indicator21 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, -3);
                    lv_meter_set_indicator_end_value(obj, indicator, 2);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff38def7), lv_color_hex(0xff00b1e5), true, 0);
                    indicator22 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 3);
                    lv_meter_set_indicator_end_value(obj, indicator, 8);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff00b1e5), lv_color_hex(0xfffad475), true, 0);
                    indicator23 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 9);
                    lv_meter_set_indicator_end_value(obj, indicator, 14);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xfffad475), lv_color_hex(0xffffa932), false, 0);
                    indicator24 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 15);
                    lv_meter_set_indicator_end_value(obj, indicator, 20);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffffa932), lv_color_hex(0xffd17000), true, 0);
                    indicator25 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 21);
                    lv_meter_set_indicator_end_value(obj, indicator, 25);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffd17000), lv_color_hex(0xffff4820), true, 0);
                    indicator26 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 26);
                    lv_meter_set_indicator_end_value(obj, indicator, 32);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffff4820), lv_color_hex(0xffac0000), true, 0);
                    indicator27 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 33);
                    lv_meter_set_indicator_end_value(obj, indicator, 38);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xffac0000), lv_color_hex(0xff620000), true, 0);
                    indicator28 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 39);
                    lv_meter_set_indicator_end_value(obj, indicator, 50);
                }
            }
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_src(obj, &img_ico_temp, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfff5d2d2), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_TICKS | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.obj44 = obj;
            lv_obj_set_pos(obj, 685, 12);
            lv_obj_set_size(obj, 309, 309);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                scale29 = scale;
                lv_meter_set_scale_ticks(obj, scale, 50, 10, 8, lv_color_hex(0xff050505));
                lv_meter_set_scale_major_ticks(obj, scale, 5, 10, 8, lv_color_hex(0xff000000), 20);
                lv_meter_set_scale_range(obj, scale, 0, 100, 180, 180);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 5, lv_color_hex(0xfff5d833), 0);
                    indicator30 = indicator;
                    lv_meter_set_indicator_value(obj, indicator, 25);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xfff86034), lv_color_hex(0xfffaa992), false, 0);
                    indicator31 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 0);
                    lv_meter_set_indicator_end_value(obj, indicator, 35);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff6ff789), lv_color_hex(0xff00de49), true, 0);
                    indicator32 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 36);
                    lv_meter_set_indicator_end_value(obj, indicator, 65);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_scale_lines(obj, scale, lv_color_hex(0xff3dcbfe), lv_color_hex(0xff0089ba), true, 0);
                    indicator33 = indicator;
                    lv_meter_set_indicator_start_value(obj, indicator, 66);
                    lv_meter_set_indicator_end_value(obj, indicator, 100);
                }
            }
            lv_obj_set_style_bg_img_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_img_src(obj, &img_ico_humidity, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff7d7d7d), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 150, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_TICKS | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_TICKS | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_img_create(parent_obj);
                    lv_obj_set_pos(obj, 206, 119);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_img_set_zoom(obj, 250);
                }
                {
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    lv_obj_set_pos(obj, 57, 188);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    static lv_point_t line_points[] = {
                        { 0, 0 },
                        { 150, 0 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                }
                {
                    // lbl_c_4
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_c_4 = obj;
                    lv_obj_set_pos(obj, 141, 194);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, " [%]");
                }
                {
                    // Lbl_temp_value_4
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temp_value_4 = obj;
                    lv_obj_set_pos(obj, 95, 191);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "00.0");
                }
                {
                    // Lbl_temperature_2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.lbl_temperature_2 = obj;
                    lv_obj_set_pos(obj, 101, 170);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Humidity");
                }
            }
        }
        {
            // clkMeter
            lv_obj_t *obj = lv_meter_create(parent_obj);
            objects.clk_meter = obj;
            lv_obj_set_pos(obj, 61, 321);
            lv_obj_set_size(obj, 220, 220);
            {
                lv_meter_scale_t *scale = lv_meter_add_scale(obj);
                scale34 = scale;
                lv_meter_set_scale_ticks(obj, scale, 61, 2, 10, lv_color_hex(0xfffffefe));
                lv_meter_set_scale_major_ticks(obj, scale, 5, 4, 10, lv_color_hex(0xfffd0000), 10);
                lv_meter_set_scale_range(obj, scale, 1, 13, 360, 300);
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 4, lv_color_hex(0xff9e1c00), -10);
                    indicator35 = indicator;
                    lv_meter_set_indicator_value(obj, indicator, 12);
                }
                {
                    lv_meter_indicator_t *indicator = lv_meter_add_needle_line(obj, scale, 3, lv_color_hex(0xff9e1c00), -1);
                    indicator36 = indicator;
                    lv_meter_set_indicator_value(obj, indicator, 6);
                }
            }
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffd0000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xffff1010), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_ITEMS | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xfffbca02), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffffffff), LV_PART_TICKS | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 292, 278);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_ico_weater);
        }
        {
            // calendar_data
            lv_obj_t *obj = lv_calendar_create(parent_obj);
            objects.calendar_data = obj;
            lv_obj_set_pos(obj, 440, 342);
            lv_obj_set_size(obj, 184, 191);
            lv_calendar_header_arrow_create(obj);
            lv_calendar_set_today_date(obj, 2022, 11, 1);
            lv_calendar_set_showed_date(obj, 2022, 11);
            lv_obj_add_event_cb(obj, action_data_change, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // roller_hour
            lv_obj_t *obj = lv_roller_create(parent_obj);
            objects.roller_hour = obj;
            lv_obj_set_pos(obj, 209, 423);
            lv_obj_set_size(obj, 83, 101);
            lv_roller_set_options(obj, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23", LV_ROLLER_MODE_INFINITE);
            lv_obj_add_event_cb(obj, action_hour_change, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            // roller_minute
            lv_obj_t *obj = lv_roller_create(parent_obj);
            objects.roller_minute = obj;
            lv_obj_set_pos(obj, 316, 388);
            lv_obj_set_size(obj, 80, 100);
            lv_roller_set_options(obj, "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n\n\n", LV_ROLLER_MODE_INFINITE);
            lv_obj_add_event_cb(obj, action_minute_change, LV_EVENT_VALUE_CHANGED, (void *)0);
        }
        {
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            lv_obj_set_pos(obj, 549, 535);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "Monday\nTuesday\nWednesday\nThursday\nFriday\nSaturday\nSunday");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            lv_obj_set_pos(obj, 269, 527);
            lv_obj_set_size(obj, 150, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "January\nFebruary\nMarch\nApril\nMay\nJune\nJuly\nAugust\nSeptember\nOctober\nNovember\nDecember");
            lv_dropdown_set_selected(obj, 0);
        }
        {
            // lbl_press_hpa_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_press_hpa_1 = obj;
            lv_obj_set_pos(obj, 845, 495);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, " [hPa]");
        }
        {
            // img_temperature
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.img_temperature = obj;
            lv_obj_set_pos(obj, 647, 362);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_ico_temp);
        }
        {
            // img_humidity
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.img_humidity = obj;
            lv_obj_set_pos(obj, 839, 362);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_ico_humidity);
        }
        {
            // Lbl_pressure_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_pressure_1 = obj;
            lv_obj_set_pos(obj, 791, 463);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Pressure");
        }
        {
            // Lbl_press_value_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_press_value_1 = obj;
            lv_obj_set_pos(obj, 792, 491);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "0000");
        }
        {
            // Lbl_temperature_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_temperature_1 = obj;
            lv_obj_set_pos(obj, 719, 362);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Temperature");
        }
        {
            // Lbl_temp_value_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_temp_value_1 = obj;
            lv_obj_set_pos(obj, 719, 391);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff03bce4), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "00.0");
        }
        {
            // Lbl_Humidity_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_humidity_1 = obj;
            lv_obj_set_pos(obj, 903, 368);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Humidity");
        }
        {
            // Lbl_humidity_value_1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_humidity_value_1 = obj;
            lv_obj_set_pos(obj, 903, 391);
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
            lv_obj_set_pos(obj, 766, 394);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, " [°C]");
        }
        {
            // lbl_percent
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.lbl_percent = obj;
            lv_obj_set_pos(obj, 952, 394);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "[%] ");
        }
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
            lv_obj_t *obj = lv_img_create(parent_obj);
            lv_obj_set_pos(obj, 727, 463);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_ico_pressure);
        }
        {
            // tempHumi_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.temp_humi_panel = obj;
            lv_obj_set_pos(obj, 29, 81);
            lv_obj_set_size(obj, 224, 219);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_align(obj, LV_ALIGN_DEFAULT, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // chart_temp_humy
                    lv_obj_t *obj = lv_chart_create(parent_obj);
                    objects.chart_temp_humy = obj;
                    lv_obj_set_pos(obj, 526, 367);
                    lv_obj_set_size(obj, 209, 76);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0xff010000), LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_color(obj, lv_color_hex(0xff7f0000), LV_PART_MAIN | LV_STATE_DEFAULT);
                }
                {
                    // img_humi
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.img_humi = obj;
                    lv_obj_set_pos(obj, 408, -10);
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
                    lv_label_set_text(obj, "Temperature/Humidity                      ");
                }
            }
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 49, 520);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_update_data_time, LV_EVENT_CLICKED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 11, 3);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Update");
                }
            }
        }
    }
    
    tick_screen_test();
}

void tick_screen_test() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_settings_screen,
    tick_screen_main,
    tick_screen_heating_screen,
    tick_screen_test,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_settings_screen();
    create_screen_main();
    create_screen_heating_screen();
    create_screen_test();
}
