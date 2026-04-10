#ifndef _HEATING_CONFIG_H_
#define _HEATING_CONFIG_H_

#include <stdio.h>
#include <string.h>

typedef enum _CONFIG_TYPE_
{
    OFF = 0,
    TEMPERATURE = 1,
    TIMER = 2
} CONFIG_TYPE;

typedef struct _TEMPERATURE_RANGE_
{
    uint8_t u8TempMin;
    uint8_t u8TempMax;
} TEMPERATURE_RANGE;

typedef struct _TIMER_RANGE_
{
    uint8_t u8ClockMin;
    uint8_t u8ClockMax;
} TIMER_RANGE;
typedef struct _HEATING_CONFIG_
{
    CONFIG_TYPE       enConfigType;
    TEMPERATURE_RANGE stTempRange;
    TIMER_RANGE       stTimerRange;
} HEATING_CONFIG;

int8_t InitHeatingConfig(void);
int8_t getHeatingConfig (HEATING_CONFIG *out_ptrToHeatingConfig);
int8_t setHeatingConfig (const HEATING_CONFIG *const in_ptrToHeatingConfig);

#endif //_HEATING_CONFIG_H_