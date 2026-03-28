#include "mqtt_manager.h"
#include "esp_log.h"

static const char    *TAG = "MqttManager"; // Tag used for ESP log output
static esp_mqtt_client_handle_t client = NULL;
static esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://192.168.8.250:1883",
        .credentials.username = "mqttuser",
        .credentials.authentication.password = "DMINGL6Intrepido123.",
        //.credentials.username = "mqttuser",
        //.credentials.authentication_method = "DMINGL6Intrepido123.",
        .credentials.client_id = "Termostato",
        };
static bool mqtt_connected = false;


// Gestore degli eventi MQTT
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) 
{
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            esp_mqtt_client_subscribe(client, "/Termostato/Configure", 0);
            //esp_mqtt_client_publish(client, "/topic/test", "Ciao da ESP32", 0, 1, 0);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            ESP_LOGI(TAG, "Received MQTT message on topic: %.*s", event->topic_len, event->topic);
            //printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            //printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        default:
            ESP_LOGI(TAG, "Unhandled MQTT event");
            break;
    }
}

int8_t mqtt_manager_init(void)
{
    int8_t iRet = 0;
    if (mqtt_connected) 
    {
        ESP_LOGI(TAG, "MQTT client already connected");
        iRet = -1; // Already connected, no need to initialize again
    }
    else
    {
        ESP_LOGI(TAG, "mqtt_manager_init Start");
        client = esp_mqtt_client_init(&mqtt_cfg);
        if (client == NULL) {
            ESP_LOGE(TAG, "Failed to initialize MQTT client");
            iRet = -1;
        } 
        else 
        {
            esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
            if (esp_mqtt_client_start(client) != ESP_OK) 
            {
                ESP_LOGE(TAG, "Failed to start MQTT client");
                iRet = -1;
            }
            else
            {
                mqtt_connected = true;
                ESP_LOGI(TAG, "MQTT client started successfully");
            }
        }
    }
    return iRet;
}

int8_t mqtt_manager_publish(const char *topic, const char *payload, int qos, bool retain)
{
    int8_t iRet = 0;
    if(!mqtt_connected) 
    {
        ESP_LOGE(TAG, "MQTT client not connected. Cannot publish message.");
        iRet = -1; // Not connected, cannot publish
    }
    else
    {
        ESP_LOGI(TAG, "mqtt_manager_publish Start");
        esp_mqtt_client_publish(client, topic, payload, 0, qos, retain);
    }
    return iRet;

}
int8_t mqtt_manager_subscribe(const char *topic, int qos)
{
    int8_t iRet = 0;
    if(!mqtt_connected) 
    {
        ESP_LOGE(TAG, "MQTT client not connected. Cannot subscribe to topic.");
        iRet = -1; // Not connected, cannot subscribe
    }
    else
    {
        ESP_LOGI(TAG, "mqtt_manager_subscribe Start");
        esp_mqtt_client_subscribe(client, topic, qos);
    }
    return iRet;
}
int8_t mqtt_manager_unsubscribe(const char *topic)   
{
    int8_t iRet = 0;
    if(!mqtt_connected) 
    {
        ESP_LOGE(TAG, "MQTT client not connected. Cannot unsubscribe from topic.");
        iRet = -1; // Not connected, cannot unsubscribe
    }
    else
    {
        ESP_LOGI(TAG, "mqtt_manager_unsubscribe Start");
        esp_mqtt_client_unsubscribe(client, topic);
    }
    return iRet;
}
int8_t mqtt_manager_cleanup(void)
{
    int8_t iRet = 0;
    if(!mqtt_connected) 
    {
        ESP_LOGE(TAG, "MQTT client not connected. Cannot clean up.");
        iRet = -1; // Not connected, cannot clean up
    }
    else
    {
        ESP_LOGI(TAG, "mqtt_manager_cleanup Start");
    //esp_mqtt_client_cleanup(client);
    }
    return iRet;
}   