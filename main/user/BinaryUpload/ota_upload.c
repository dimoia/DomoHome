#include <stdio.h>
#include <string.h>
#include "ota_upload.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_http_server.h"
#include <sys/param.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_server.h"
#include "esp_ota_ops.h"


static const char *TAG = "OTA_UPDATE";

esp_err_t update_post_handler(httpd_req_t *req)
{
    char buf[1024];
    esp_ota_handle_t update_handle = 0;
    const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);

    ESP_LOGI(TAG, "Inizio OTA sulla partizione: %s", update_partition->label);
    esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);

    int remaining = req->content_len;
    while (remaining > 0) {
        int recv_len = httpd_req_recv(req, buf, MIN(remaining, sizeof(buf)));
        if (recv_len <= 0) {
            if (recv_len == HTTPD_SOCK_ERR_TIMEOUT) continue;
            esp_ota_end(update_handle);
            return ESP_FAIL;
        }
        esp_ota_write(update_handle, buf, recv_len);
        remaining -= recv_len;
    }

    esp_ota_end(update_handle);
    esp_ota_set_boot_partition(update_partition);

    ESP_LOGI(TAG, "Update completato. Riavvio...");
    httpd_resp_sendstr(req, "<html><body><h1>Update OK! Riavvio...</h1></body></html>");
    
    vTaskDelay(pdMS_TO_TICKS(2000));
    esp_restart();
    return ESP_OK;
}

/* --- PAGINA HTML DI CARICAMENTO --- */
esp_err_t index_get_handler(httpd_req_t *req) 
{
    const char* html = "<html><body>"
                       "<form method='POST' action='/update' enctype='multipart/form-data'>"
                       "Firmware: <input type='file' name='update'><br><br>"
                       "<input type='submit' value='Carica e Aggiorna'>"
                       "</form></body></html>";
    return httpd_resp_send(req, html, strlen(html));
}

/* --- CONFIGURAZIONE SERVER --- */
void start_webserver() 
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 2;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t index_uri = { .uri = "/", .method = HTTP_GET, .handler = index_get_handler };
        httpd_register_uri_handler(server, &index_uri);

        httpd_uri_t update_uri = { .uri = "/update", .method = HTTP_POST, .handler = update_post_handler };
        httpd_register_uri_handler(server, &update_uri);
        
        ESP_LOGI(TAG, "Server avviato.");
    }
}
