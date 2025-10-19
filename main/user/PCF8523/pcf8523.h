#ifndef _PCF8523_H_
#define _PCF8523_H_

#include <time.h>
#include <stdbool.h>


int8_t Pcf8523_Reset();
int8_t Pcf8523_Init();
int8_t Pcf8523_Set_Time(struct tm *time);
int8_t Pcf8523_Get_Time(struct tm *out_stTime);

#endif /* MAIN_PCF8563_H_ */
