#ifndef _AHT10_H_
#define _AHT10_H_

int8_t aht10_init();
int8_t aht10_getCurrentTempAndHumidity(float *out_ptrTemperature, float *out_ptrHumidity);


#endif //_AHT10_H_