#include "utils.h"

const char *TAG_UTILS = "utils"; 

int dayofweek(int day,int month,int year)
{
    int arr[] = {0,3,2,5,3,5,1,4,6,2,4};
    if(month<3)
        year--;
    return ((year+year/4-year/100+year/400+arr[month-1]+day)%7);
}