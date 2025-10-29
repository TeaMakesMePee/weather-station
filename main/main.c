#include <stdio.h>
#include "bme280.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "freertos/task.h"
#include "driver/gpio.h"
#include "ssd1306.h"

#define I2C_MASTER_SCL_IO           GPIO_NUM_22           /*!< gpio number for I2C master clock IO2*/
#define I2C_MASTER_SDA_IO           GPIO_NUM_21           /*!< gpio number for I2C master data  IO1*/
#define I2C_MASTER_NUM              I2C_NUM_0            /*!< I2C port number for master bme280 */
#define I2C_MASTER_TX_BUF_DISABLE   0                    /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                    /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ          100000 

static i2c_bus_handle_t i2c_bus = NULL;
static bme280_handle_t bme280 = NULL;
static ssd1306_handle_t ssd1306 = NULL;

static const char *TAG = "bme280_main";

esp_err_t i2c_bus_init()
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_bus = i2c_bus_create(I2C_NUM_0, &conf);
    if (i2c_bus == NULL) {
        ESP_LOGE(TAG, "Failed to create I2C bus handle");
        return ESP_FAIL;
    }

    bme280 = bme280_create(i2c_bus, BME280_I2C_ADDRESS_DEFAULT);
    if (bme280 == NULL) {
        ESP_LOGE(TAG, "Failed to create BME280 handle");
        return ESP_FAIL;
    }

    if (bme280_default_init(bme280) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize BME280 sensor");
        return ESP_FAIL;
    }

    i2c_master_bus_handle_t low_level_bus_handle = i2c_bus_get_internal_bus_handle(i2c_bus); //
    if (low_level_bus_handle == NULL) {
        ESP_LOGE(TAG, "Failed to get internal bus handle");
        return ESP_FAIL;
    }

    ssd1306_config_t ssd1306_cfg = {
        .i2c_address = I2C_SSD1306_DEV_ADDR,
        .i2c_clock_speed = I2C_MASTER_FREQ_HZ,
        .panel_size = SSD1306_PANEL_128x64,
        .offset_x = 0,
        .flip_enabled = false
    };
    
    // Pass the low-level handle here
    if (ssd1306_init(low_level_bus_handle, &ssd1306_cfg, &ssd1306) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SSD1306");
        return ESP_FAIL;
    }

    // --- 5. Now use the SSD1306 handle to draw ---
    ssd1306_clear_display(ssd1306, false);
    ssd1306_display_text(ssd1306, 0, "Initializing...", false);
    vTaskDelay(pdMS_TO_TICKS(1000));

    return ESP_OK;
}

void bme280_deinit()
{
    bme280_delete(&bme280);
    i2c_bus_delete(&i2c_bus);
    ssd1306_delete(ssd1306);
}

void bme280_read()
{
    while (1) 
    {
        float temperature = 0.0, humidity = 0.0, pressure = 0.0;
        bme280_read_temperature(bme280, &temperature);
        bme280_read_humidity(bme280, &humidity);
        bme280_read_pressure(bme280, &pressure);
        
        ESP_LOGI(TAG, "Temp: %.2f C, Hum: %.2f %%, Pres: %.2f hPa",
                temperature, humidity, pressure);

        char temp_line[20];
        char hum_line[20];
        char pres_line[20];
        snprintf(temp_line, sizeof(temp_line), "Temp: %.1f C", temperature);
        snprintf(hum_line, sizeof(hum_line), "Hum: %.1f %%", humidity);
        snprintf(pres_line, sizeof(pres_line), "Pres: %.0f hPa", pressure);

        ssd1306_clear_display(ssd1306, false);
        ssd1306_display_text(ssd1306, 0, temp_line, false);  // Page 0
        ssd1306_display_text(ssd1306, 2, hum_line, false);   // Page 2
        ssd1306_display_text(ssd1306, 4, pres_line, false); // Page 4

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    if (i2c_bus_init() != ESP_OK) {
        ESP_LOGE(TAG, "I2C init failed. Halting.");
        return;
    }

    ESP_LOGI(TAG, "I2C init success. Starting measurements.");

    xTaskCreate(
        bme280_read,    
        "Sensor Read Task",  
        2048,                
        NULL,                
        5,                   
        NULL                 
    );
}