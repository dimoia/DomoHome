#ifndef _PCF8563_H_
#define _PCF8563_H_

#include <time.h>
#include <stdbool.h>


void pcf8563_reset();
void pcf8563_init();
int8_t pcf8563_set_time(struct tm *time);
int8_t pcf8563_get_time(struct tm *time);

#endif /* MAIN_PCF8563_H_ */
