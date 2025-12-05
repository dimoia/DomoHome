#ifndef _UTILS_
#define _UTILS_

#include <string.h>
#include <time.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Calculate Day of Week from Date
/// @param day   Day of the month (1-31)
/// @param month Month (1-12)
/// @param year  Year (e.g., 2024)
/// @return Day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
/////////////////////////////////////////////////////////////////////////////////////////////////////////
int dayofweek(int day,int month,int year);

#endif //_UTILS_
