#ifndef _LVGL_CUSTOM_
#define _LVGL_CUSTOM_

#include <stdint.h>

void MsgConfigBox(char *strMsg);
void MsgInfoBox(char *strMsg);
void lv_msgbox_factory_reset(void);
void heating_screen(void);
void goto_home_screen(void);
void create_progress_bar(void);
void update_bar(uint32_t value);
void close_pupup_bar(void);
#endif // _LVGL_CUSTOM_
