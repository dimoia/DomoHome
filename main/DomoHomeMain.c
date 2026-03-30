/*****************************************************************************
 * | File       :   main.c
 * | Author     :   Waveshare team
 * | Function   :   Main function
 * | Info       :
 * |                UI Design：
 *                          1. User Login and Creation: Users can log in or create new accounts, and the created users are saved to NVS, so data is not lost after power-down.
 *                          2. Wi-Fi: Can connect to Wi-Fi and start an access point (hotspot).
 *                          3. RS485: Can send and receive data, with data displayed on the screen.
 *                          4. PWM: Can modify PWM output in multiple ways to control screen brightness. Additionally, it can display information from a Micro SD card.
 *                          5. CAN: Can send and receive data, with data displayed on the screen.
 *----------------
 * | Version    :   V1.0
 * | Date       :   2025-05-08
 * | Info       :   Basic version
 *
 ******************************************************************************/
#include <time.h>
#include <string.h>
#include <assert.h>
#include "rgb_lcd_port.h" // Header for Waveshare RGB LCD driver
#include "gt911.h"        // Header for touch screen operations (GT911)
#include "lvgl_port.h"    // Header for LVGL port initialization and locking
//#include "wifi.h"         // Header for Wi-Fi functionality
//#include "sd_card.h"      // Header for SD card operations
#include "BME280.h"      // Header for SD card operations
#include "ui.h"           // Header for user interface initialization
#include "pcf8523.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_partition.h"
#include "screens.h"
#include "sys_info.h"
#include "calendar_time.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "config.h"
#include "mqtt_manager.h"
#include "mqtt_client.h"
static const char    *TAG = "DomoHomeMain"; // Tag used for ESP log output
static QueueHandle_t gpio_evt_queue = NULL;
static TaskHandle_t  boot_TaskHandle;
static const uint32_t u32BootPressedForSeconds = 5 * 1000 * 1000; // Time in seconds to consider a long press


static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_current_target(e);
    LV_LOG_USER("Button %s clicked", lv_msgbox_get_active_btn_text(obj));
}

void lv_example_msgbox_1(void)
{
    static const char * btns[] = { "Reset", "Cancel" ""};

    lv_obj_t * mbox1 = lv_msgbox_create(NULL, "DomoHome", "DomoHome Reset To Factory", btns, true);
    lv_obj_add_event_cb(mbox1, event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_center(mbox1);
}


static void boot_task(void* arg)
{
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = 
    {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT, // Select the default clock source
        .direction = GPTIMER_COUNT_UP,      // Counting direction is up
        .resolution_hz = 1 * 1000 * 1000,   // Resolution is 1 MHz, i.e., 1 tick equals 1 microsecond
        .intr_priority = 1,                // Interrupt priority (0 is highest, but we set it to 1 for a relatively low priority)          
    };
    
    // Create a timer instance
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));
    // Enable the timer
   // ESP_ERROR_CHECK(gptimer_enable(gptimer));
    //static uint8_t u8Count = 0;
    uint32_t io_num;
    //uint64_t u64elapsedTime;
    uint64_t u64TimerCount;
    for (;;) 
    {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) 
        {
            u64TimerCount = 0;
            ESP_ERROR_CHECK(gptimer_enable(gptimer));
            ESP_LOGI(TAG,"GPIO[%" PRIu32 "] intr, val: %d", io_num, gpio_get_level((gpio_num_t)io_num));
            gptimer_set_raw_count(gptimer,0);
            gptimer_start(gptimer);
           // u64elapsedTime = 0;
            while(gpio_get_level((gpio_num_t)io_num) == 0)
            {
                gptimer_get_raw_count(gptimer, &u64TimerCount);
                //u64elapsedTime = u64elapsedTime + u64TimerCount;
                //ESP_LOGI(TAG, "TimerCount %"PRIu64" microseconds", u64TimerCount);
                double time = (double)u64TimerCount / timer_config.resolution_hz;
                ESP_LOGI(TAG, "TimerCount %e seconds", time);
               // ESP_LOGI(TAG, "Elapsed    %"PRIu64" microseconds", u64elapsedTime);
              //  vTaskDelay(pdMS_TO_TICKS(1000)); 
            }
            if(u64TimerCount >= u32BootPressedForSeconds) 
            {
                ESP_LOGI(TAG, "Long Press Detected: Performing Factory Reset");
                // Perform factory reset actions here
                lv_example_msgbox_1();
            }
            else 
            {
                ESP_LOGI(TAG, "Short Press Detected: Performing Normal Boot");
                // Perform normal boot actions here
                // e.g., restre filesystem from nvs                                        
            }                
            gptimer_stop(gptimer);
            ESP_ERROR_CHECK(gptimer_disable(gptimer));
#if 0
            if(gpio_get_level(io_num) == 0) 
            {     
                 ESP_ERROR_CHECK(gptimer_disable(gptimer));
                 ESP_ERROR_CHECK(gptimer_enable(gptimer));

                // Start the timer                
                gptimer_start(gptimer);
                ESP_LOGI(TAG, "Boot Button Pressed => Timer Started");
            }
            else 
            {                                
                // Stop timer and get the elapsed time
                u8Count++;
                if( u8Count <= 2 ) 
                {

                    gptimer_stop(gptimer);
                    ESP_LOGI(TAG, "Boot Button Released => Timer Stopped");

                    uint64_t timer_count;
                    gptimer_get_raw_count(gptimer, &timer_count);

                    ESP_LOGI(TAG, "Button was pressed for %"PRIu64" microseconds", timer_count);
                    
                    if(timer_count >= u32BootPressedForSeconds) 
                    {
                        ESP_LOGI(TAG, "Long Press Detected: Performing Factory Reset");
                        // Perform factory reset actions here
                    }
                    else 
                    {
                        ESP_LOGI(TAG, "Short Press Detected: Performing Normal Boot");
                        // Perform normal boot actions here
                        // e.g., restre filesystem from nvs                                        
                    }                
                }
                else
                {
                    ESP_LOGI(TAG, "Ignoring bounce, u8Count=%d", u8Count);
                    ESP_ERROR_CHECK(gptimer_disable(gptimer));
                    ESP_ERROR_CHECK(gptimer_enable(gptimer));
                }
            }
#endif                
        }
        ESP_LOGI(TAG, "boot_task running");
    }
}

void action_goto_heating_screen(lv_event_t *e) {
    // TODO: Implement action goto_heating_screen here
    lv_event_code_t code = lv_event_get_code(e);
   // lv_obj_t * obj = lv_event_get_current_target(e);

    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
         loadScreen(SCREEN_ID_HEATING_SCREEN);
        //lv_scr_load(obj);       
    }
}

void action_goto_home(lv_event_t *e) 
{
    lv_event_code_t code = lv_event_get_code(e);
      if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
         loadScreen(SCREEN_ID_MAIN );
        
       
    }
}

void action_goto_settings_screen(lv_event_t *e) {
    // TODO: Implement action goto_settings_screen here
    lv_event_code_t code = lv_event_get_code(e);
      if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("Clicked");
         loadScreen(SCREEN_ID_SETTINGS_SCREEN );
               
    }
}


void action_slider_temp1_change(lv_event_t *e) {
    // TODO: Implement action slider_temp1_change here
     LV_LOG_USER("Slider 1 Changeed");
}

static void event_handler_config(lv_event_t * e) 
{
    lv_obj_t * mbox = lv_event_get_current_target(e);
    const char * txt = lv_msgbox_get_active_btn_text(mbox);

    if(strcmp(txt, "Config") == 0) {
        // Logica per il pulsante Config
        loadScreen(SCREEN_ID_SETTINGS_SCREEN );
    } 
    else if(strcmp(txt, "Cancella") == 0) {
        // Logica per il pulsante Cancella
        loadScreen(SCREEN_ID_TEST);
    }

    lv_msgbox_close(mbox);
}


void MsgConfigBox(void) 
{
    static const char * btns[] = {"Config", ""}; // L'ultimo elemento deve essere stringa vuota

      /* 1. Crea un "velo" (overlay) che copre tutto lo schermo */
    lv_obj_t * obj_bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj_bg, LV_PCT(100), LV_PCT(100)); // Copre tutto
    lv_obj_set_style_bg_color(obj_bg, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(obj_bg, LV_OPA_60, 0);    // Trasparenza al 60%
    lv_obj_set_style_border_width(obj_bg, 0, 0);      // Rimuove bordi del velo
    lv_obj_set_style_radius(obj_bg, 0, 0);            // Angoli retti

    lv_obj_t * mboxConfig = lv_msgbox_create(obj_bg, LV_SYMBOL_WARNING "Warning", "Configuration File Not Found", btns, false);
    
    
    // 2. AGGIUNTA DELL'OMBRA
    lv_obj_set_style_shadow_width(mboxConfig, 20, 0);          // Ampiezza dell'ombra
    lv_obj_set_style_shadow_color(mboxConfig, lv_palette_main(LV_PALETTE_GREY), 0); // Colore ombra
    lv_obj_set_style_shadow_ofs_y(mboxConfig, 5, 0);         // Spostamento verso il basso
    lv_obj_set_style_shadow_spread(mboxConfig, 2, 0);         // Diffusione dell'ombra

    // Opzionale: colora il titolo di giallo/arancio
    lv_obj_t * title = lv_msgbox_get_title(mboxConfig);
    lv_obj_set_style_text_color(title, lv_palette_main(LV_PALETTE_AMBER), 0);

    // Aggiunge un evento per gestire la pressione dei pulsanti
    lv_obj_add_event_cb(mboxConfig, event_handler_config, LV_EVENT_VALUE_CHANGED, NULL);
    
    lv_obj_center(mboxConfig);

    lv_obj_t * btnm = lv_msgbox_get_btns(mboxConfig);
    
    // Impostiamo una larghezza generosa alla matrice per occupare tutta la msgbox
    lv_obj_set_width(btnm, lv_pct(100));
    lv_obj_set_style_pad_column(btnm, 40, 0); 

    //lv_obj_set_style_align(btnm, LV_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(btnm, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_ITEMS);
    
}


/**
 * @brief Main application function.
 *
 * This function initializes the necessary hardware components such as the touch screen
 * and RGB LCD display, sets up the LVGL library for graphics rendering, and runs
 * the LVGL demo UI.
 *
 * - Initializes the GT911 touch screen controller.
 * - Initializes the Waveshare ESP32-S3 RGB LCD display.
 * - Initializes the LVGL library for graphics rendering.
 * - Runs the LVGL demo UI.
 *
 * @return None
 */

void app_main()
{
    static esp_lcd_panel_handle_t panel_handle = NULL; // Handle for the LCD panel
    static esp_lcd_touch_handle_t tp_handle = NULL;    // Handle for the touch panel  

    ESP_LOGI(TAG, "DomoHome Main Application Starting...");

    setConfigStatus(NOT_CONFIG);

    // Initialize the GT911 touch screen controller
    // This sets up the touch functionality of the screen.
    tp_handle = touch_gt911_init();
    if (tp_handle == NULL) 
    {
        ESP_LOGE(TAG, "Failed to initialize GT911 touch controller");
        return; // Exit if touch controller initialization fails
    }

    // Initialize the Waveshare ESP32-S3 RGB LCD hardware
    // This prepares the LCD panel for display operations.
    panel_handle = waveshare_esp32_s3_rgb_lcd_init();
    if (panel_handle == NULL) 
    {
        ESP_LOGE(TAG, "Failed to initialize RGB LCD panel");
        return; // Exit if LCD panel initialization fails
    }

    // Turn on the LCD backlight This ensures the display is visible.
    wavesahre_rgb_lcd_bl_on();

    // Initialize the LVGL library, linking it to the LCD and touch panel handles
    // LVGL is a lightweight graphics library used for rendering UI elements.

    if(lvgl_port_init(panel_handle, tp_handle) != ESP_OK) 
    {
        ESP_LOGI(TAG, "Failed to initialize LVGL");
        return; // Exit if LVGL initialization fails}   
    }
    if(bme280_init() >= 0) 
    {
        ESP_LOGI(TAG, "BMP280 Init Success");

        uint8_t u8DevId;
        int8_t u8Pbit = bme280_Pbit(&u8DevId);
        if(u8Pbit == 0)
        {
            ESP_LOGI(TAG, "BME280 Device ID: 0x%02X", u8DevId);
        }
        else
        {
            ESP_LOGE(TAG, "Failed to read BME280 Device ID ");
        }
    }
    
    
    if(Pcf8523_Init() >= 0)
    {
        ESP_LOGI(TAG, "PCF8523 initialized successfully");  
        if(Pcf8523_PBit_Battery_Status() >= 0)
        {
            ESP_LOGI(TAG, "PCF8523 Status: OK");
        }
        else
        {
            ESP_LOGE(TAG, "PCF8523 Status: LOW");
        }
    }
    else
    {
        ESP_LOGE(TAG, "Failed to initialize PCF8523");
    }

    // Initialize Wi-Fi settings (connect to the specified Wi-Fi network)
   // wifi_init(); 


    // Initialize the Non-Volatile Storage (NVS) for settings and data persistence
    // This ensures that user data and settings are retained even after power loss.
    // Initialize the Non-Volatile Storage (NVS) for Wi-Fi settings
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // If NVS has no free pages or a new version is found, erase and reinitialize NVS
        ESP_LOGE(TAG, "NVS Flash Init Error, Erasing NVS...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
        if(err != ESP_OK)
        {
            ESP_LOGE(TAG, "NVS Flash Re-Init Failed: %d", err);
            //return; // Exit if NVS initialization fails
        }
    }

    if(err != ESP_OK)
    {
        // NVS not critical for main application, so we log the error but continue execution
        ESP_LOGE(TAG, "NVS Flash Init Failed: %d", err);

        // Use Termostato as minimal configuration if NVS fails to initialize
        uint8_t u8DevId;
        int8_t i8Bme280Status = bme280_Pbit(&u8DevId);
        int32_t iRtcStatus   = Pcf8523_PBit_Battery_Status();

        if((i8Bme280Status == 0) && (iRtcStatus >= 0))
        {
            // screen with temperature, humidity, pressure, date and time
            // loadScreen(SCREEN_ID_MAIN_TEMP_HUMI_PRESS_DATE_TIME);
        }
        else if ((i8Bme280Status == 0) && (iRtcStatus < 0))
        {
            // screen with temperature, humidity, pressure, No time and date information available
            // loadScreen(SCREEN_ID_MAIN_TEMP_HUMI_PRESS);
        }
        else if ((i8Bme280Status < 0) && (iRtcStatus >= 0))
        {
            // screen with No temperature, humidity, pressure information available, date and time information available
            // loadScreen(SCREEN_ID_MAIN_MANUAL_DATE_TIME);
        }
        else
        {
            // Call screen with error message about sensor initialization failure
            //loadScreen(SCREEN_ID_MAIN_MANUAL);
        }
    }
    else
    {
        // Lock the LVGL port to ensure thread safety during API calls
        // This prevents concurrent access issues when using LVGL functions.
        if (lvgl_port_lock(-1))
        {

            // Initialize the UI components with LVGL (e.g., demo screens, sliders)
            // This sets up the user interface elements using the LVGL library.
            //ui_init();
            create_screens();

            if(iDownloadConfigFileFromNVS("config.txt") == -1)
            {
                ESP_LOGI(TAG, "Configuration file not found");
                MsgConfigBox();
                //loadScreen(SCREEN_ID_SETTINGS_SCREEN );
            }
            else
            {
                setConfigStatus(CONFIG);
                loadScreen(SCREEN_ID_MAIN);
                if( wifi_sta_init((uint8_t*)"dlinkAP", (uint8_t*)"DMINGL6Intrepido123.", WIFI_AUTH_WPA2_PSK) == ESP_OK)
                {
                    ESP_LOGE(TAG, "Success to connect to Wi-Fi network");
                }
                else
                {
                    ESP_LOGE(TAG, "Failed to connect to Wi-Fi network");
                }
            }            

            // Release the mutex after LVGL operations are complete
            // This allows other tasks to access the LVGL port.
            lvgl_port_unlock();        
        }
        
        
        if (getConfigStatus() == CONFIG)
        {

            lvgl_port_lock(-1);
            objects_t objs_goto_heating_screen = objects;
            lv_obj_t *btn_goto_heating_screen  = objs_goto_heating_screen.btn_heating_screen;
            lv_obj_add_event_cb(btn_goto_heating_screen, action_goto_heating_screen, LV_EVENT_CLICKED, NULL);  
            
            objects_t objs_btn_goto_home = objects;
            lv_obj_t *btn_goto_home      =  objs_btn_goto_home.btn_goto_home_from_heater_screen;         
            lv_obj_t *btn_goto_home_1    =  objs_btn_goto_home.btn_goto_home_from_heater_screen_1;
            lv_obj_add_event_cb(btn_goto_home_1, action_goto_home, LV_EVENT_CLICKED, NULL);  
            lv_obj_add_event_cb(btn_goto_home,   action_goto_home, LV_EVENT_CLICKED, NULL); 

            objects_t objs_goto_settings_screen = objects;
            lv_obj_t *btn_goto_settings_screen  = objs_goto_heating_screen.btn_settings_screen;
            lv_obj_add_event_cb(btn_goto_settings_screen, action_goto_settings_screen, LV_EVENT_CLICKED, NULL);

            /* Slider Temperature 1*/
            objects_t objs_slider_temperature_1 = objects;
            lv_obj_t *slider_temp_1  = objs_slider_temperature_1.slider_temp_1;
            lv_obj_add_event_cb(slider_temp_1, action_slider_temp1_change, LV_EVENT_VALUE_CHANGED, NULL);       

            initialize_data_and_time();

            lvgl_port_unlock();
            /*  
                lv_obj_t  * calendar = lv_calendar_create(lv_scr_act());
            lv_obj_set_size(calendar, 185, 185);
            lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 27);
            lv_obj_add_event_cb(calendar, event_handler, LV_EVENT_ALL, NULL);

            lv_calendar_set_today_date(calendar, 2021, 02, 23);
        lv_calendar_set_showed_date(calendar, 2021, 02);
        */
                
            mqtt_manager_init();
                
            
            // Delay to ensure all initializations are stable
            vTaskDelay(100); // Delay for a short period to ensure stable initialization

            bme280_run();
        //    vTaskDelay(100); // Delay for a short period to ensure stable initialization
            Pcf8523_Run();
            
            // Initialize PWM for controlling backlight brightness (if applicable)
            // PWM is used to adjust the brightness of the LCD backlight.
        // pwm_init();


        
            
        
            /*
            struct tm time = {
                .tm_year = 2025, // Year since 1900
                .tm_mon  = 10,  // 0-based
                .tm_mday = 18,
                .tm_hour = 12,
                .tm_min  = 11,
                .tm_sec  = 00,
                .tm_wday = 0  // days since Sunday - [0, 6]
            };

            pcf8563_reset();
            Pcf8523_Set_Time(&time);
            */


            // Initialize SD card operations
            // This sets up the Micro SD card for data storage and retrieval.
        // sd_init();

            // Start the WIFI task to handle Wi-Fi functionality
            // This task manages Wi-Fi connections and hotspot creation.
        // xTaskCreate(wifi_task, "wifi_task", 6 * 1024, NULL, 9, &wifi_TaskHandle);
            
            // Initialize SoftAP (Wi-Fi Access Point) with SSID, password, and channel
        // wifi_sta_init((uint8_t *)"HUAWEI-B535-13F7", (uint8_t *)"DMINGL6Intrepido123.", WIFI_AUTH_WPA2_PSK);

            gpio_config_t button_config = {
            .pin_bit_mask = (1ULL << GPIO_NUM_0),
            .mode         = GPIO_MODE_INPUT,
            .pull_up_en   = GPIO_PULLUP_ENABLE,
            .intr_type    = GPIO_INTR_NEGEDGE,  /* GPIO interrupt type : falling edge */
            };
            gpio_config(&button_config);
            

            gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
            //start gpio task   
            xTaskCreate(boot_task, "boot_task", 4096, NULL, 10, &boot_TaskHandle);

            gpio_install_isr_service(0);
            //hook isr handler for specific gpio pin
            gpio_isr_handler_add(GPIO_NUM_0, gpio_isr_handler, (void*) GPIO_NUM_0);
        }
        ESP_LOGI(TAG, "DomoHome Main Application Finished Initialization.");
    }
}
