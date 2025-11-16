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
            ESP_LOGI(TAG, "GPIO[%"PRIu32"] intr, val: %d",io_num, gpio_get_level(io_num));
            gptimer_set_raw_count(gptimer,0);
            gptimer_start(gptimer);
           // u64elapsedTime = 0;
            while(gpio_get_level(io_num) == 0)
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

  

    // Initialize the Non-Volatile Storage (NVS) for settings and data persistence
    // This ensures that user data and settings are retained even after power loss.
    //init_nvs();

    static esp_lcd_panel_handle_t panel_handle = NULL; // Handle for the LCD panel
    static esp_lcd_touch_handle_t tp_handle = NULL;    // Handle for the touch panel  

    // Initialize the GT911 touch screen controller
    // This sets up the touch functionality of the screen.
    tp_handle = touch_gt911_init();

    // Initialize the Waveshare ESP32-S3 RGB LCD hardware
    // This prepares the LCD panel for display operations.
    panel_handle = waveshare_esp32_s3_rgb_lcd_init();

    // Turn on the LCD backlight
    // This ensures the display is visible.
    wavesahre_rgb_lcd_bl_on();

    // Initialize the LVGL library, linking it to the LCD and touch panel handles
    // LVGL is a lightweight graphics library used for rendering UI elements.
    ESP_ERROR_CHECK(lvgl_port_init(panel_handle, tp_handle));    

    // Lock the LVGL port to ensure thread safety during API calls
    // This prevents concurrent access issues when using LVGL functions.
    if (lvgl_port_lock(-1))
    {

        // Initialize the UI components with LVGL (e.g., demo screens, sliders)
        // This sets up the user interface elements using the LVGL library.
       // ui_init();
        create_screens();
        loadScreen(SCREEN_ID_BOOT);
      
        // Release the mutex after LVGL operations are complete
        // This allows other tasks to access the LVGL port.
        lvgl_port_unlock();        
    }

    if(bme280_init() < 0) 
    {
        ESP_LOGE(TAG, "BMP280 Init Failed");
    }
    Pcf8523_Init();

   // if (lvgl_port_lock(-1))
   // {
        objects_t objs = objects;
        lv_obj_t *txt_boot_area = objs.txt_boot_area;
        lvgl_port_lock(-1);
        vPrintBootInfo( txt_boot_area );        
        lvgl_port_unlock();
     //   lvgl_port_unlock();
   // }

    if (lvgl_port_lock(-1))
    {
        vTaskDelay(5000); // Delay for a short period to ensure stable initialization
        lvgl_port_unlock();
        loadScreen(SCREEN_ID_MAIN);
        
    }

    vTaskDelay(100); // Delay for a short period to ensure stable initialization

    bme280_run();
    vTaskDelay(100); // Delay for a short period to ensure stable initialization
    Pcf8523_Run();
    
    // Initialize PWM for controlling backlight brightness (if applicable)
    // PWM is used to adjust the brightness of the LCD backlight.
   // pwm_init();


   //aht10_init();

   
    
   
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

    gpio_config_t button_config = {
     .pin_bit_mask = (1ULL << GPIO_NUM_0),
     .mode         = GPIO_MODE_INPUT,
     .pull_up_en   = GPIO_PULLUP_ENABLE,
     .intr_type    = GPIO_INTR_NEGEDGE  /* GPIO interrupt type : falling edge */
    };
    gpio_config(&button_config);
    

    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task   
    xTaskCreate(boot_task, "boot_task", 4096, NULL, 10, &boot_TaskHandle);

    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_0, gpio_isr_handler, (void*) GPIO_NUM_0);
}