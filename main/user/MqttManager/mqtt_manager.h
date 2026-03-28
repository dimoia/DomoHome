#ifndef _MQTT_MANAGER_H_
#define _MQTT_MANAGER_H_

#include "mqtt_client.h"

int8_t mqtt_manager_init(void);
int8_t mqtt_manager_publish(const char *topic, const char *payload, int qos, bool retain);
int8_t mqtt_manager_subscribe(const char *topic, int qos);
int8_t mqtt_manager_unsubscribe(const char *topic);   
int8_t mqtt_manager_cleanup(void);
#endif //_MQTT_MANAGER_H_