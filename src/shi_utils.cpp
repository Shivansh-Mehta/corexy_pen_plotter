#include "shi_utils.h"
#include "shi_definitions.h"
#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>

const char *tag_utils = "utilities";

bool b_sensor_mot1 = false;
bool b_sensor_mot2 = false;

bool b_endstop_x_a = false;
bool b_endstop_x_b = false;
uint64_t count_x_a = 0;
uint64_t count_x_b = 0;

bool b_endstop_y_a = false;
bool b_endstop_y_b = false;
uint64_t count_y_a = 0;
uint64_t count_y_b = 0;

void IRAM_ATTR intr_endstop_x_a(void *arg)
{
    b_endstop_x_a = true;
};

void IRAM_ATTR intr_endstop_x_b(void *arg)
{
    b_endstop_x_b = true;
};

void IRAM_ATTR intr_endstop_y_a(void *arg)
{
    b_endstop_y_a = true;
};

void IRAM_ATTR intr_endstop_y_b(void *arg)
{
    b_endstop_y_b = true;
};

void IRAM_ATTR intr_sensor_mot1(void *arg)
{
    b_sensor_mot1 = true;
};

void IRAM_ATTR intr_sensor_mot2(void *arg)
{
    b_sensor_mot2 = true;
};

void config_pin_io()
{
    ESP_LOGI(tag_utils, "running config_pin_io()");

    // global enable
    esp_rom_gpio_pad_select_gpio(global_enable);
    gpio_set_direction(global_enable, GPIO_MODE_OUTPUT);
    gpio_set_level(global_enable, 1);

    // motor pin slp
    esp_rom_gpio_pad_select_gpio(mot1_slp);
    gpio_set_direction(mot1_slp, GPIO_MODE_OUTPUT);
    gpio_set_level(mot1_slp, 0);

    // motor pin slp
    esp_rom_gpio_pad_select_gpio(mot2_slp);
    gpio_set_direction(mot2_slp, GPIO_MODE_OUTPUT);
    gpio_set_level(mot2_slp, 0);

    // motor 1 pin a
    esp_rom_gpio_pad_select_gpio(mot1_pin_a);
    gpio_set_direction(mot1_pin_a, GPIO_MODE_OUTPUT);
    // motor 1 pin b
    esp_rom_gpio_pad_select_gpio(mot1_pin_b);
    gpio_set_direction(mot1_pin_b, GPIO_MODE_OUTPUT);

    // motor 2 pin a
    esp_rom_gpio_pad_select_gpio(mot2_pin_a);
    gpio_set_direction(mot2_pin_a, GPIO_MODE_OUTPUT);
    // motor 2 pin b
    esp_rom_gpio_pad_select_gpio(mot2_pin_b);
    gpio_set_direction(mot2_pin_b, GPIO_MODE_OUTPUT);

    // x axis endstop a
    esp_rom_gpio_pad_select_gpio(endstop_x_a);
    gpio_set_direction(endstop_x_a, GPIO_MODE_INPUT);
    gpio_set_pull_mode(endstop_x_a, GPIO_PULLUP_ONLY);
    // x axis endstop b
    esp_rom_gpio_pad_select_gpio(endstop_x_b);
    gpio_set_direction(endstop_x_b, GPIO_MODE_INPUT);
    gpio_set_pull_mode(endstop_x_b, GPIO_PULLUP_ONLY);

    // y axis endstop a
    esp_rom_gpio_pad_select_gpio(endstop_y_a);
    gpio_set_direction(endstop_y_a, GPIO_MODE_INPUT);
    gpio_set_pull_mode(endstop_y_a, GPIO_PULLUP_ONLY);
    // y axis endstop b
    esp_rom_gpio_pad_select_gpio(endstop_y_b);
    gpio_set_direction(endstop_y_b, GPIO_MODE_INPUT);
    gpio_set_pull_mode(endstop_y_b, GPIO_PULLUP_ONLY);
};

void config_pin_intr()
{
    ESP_LOGI(tag_utils, "running config_pin_intr()");
    gpio_install_isr_service(0);

    // esp_rom_gpio_pad_select_gpio(sensor_mot1);
    gpio_set_intr_type(sensor_mot1, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(sensor_mot1, intr_sensor_mot1, NULL);

    // esp_rom_gpio_pad_select_gpio(sensor_mot2);
    gpio_set_intr_type(sensor_mot2, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(sensor_mot2, intr_sensor_mot2, NULL);

    // esp_rom_gpio_pad_select_gpio(endstop_x_a);
    gpio_set_intr_type(endstop_x_a, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(endstop_x_a, intr_endstop_x_a, NULL);

    // esp_rom_gpio_pad_select_gpio(endstop_x_b);
    gpio_set_intr_type(endstop_x_b, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(endstop_x_b, intr_endstop_x_b, NULL);

    // esp_rom_gpio_pad_select_gpio(endstop_y_a);
    gpio_set_intr_type(endstop_y_a, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(endstop_y_a, intr_endstop_y_a, NULL);

    // esp_rom_gpio_pad_select_gpio(endstop_y_b);
    gpio_set_intr_type(endstop_y_b, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(endstop_y_b, intr_endstop_y_b, NULL);
};

void count_intr_endstop_x_a(void *arg)
{
    ESP_LOGI(tag_utils, "listening to: endstop_x_a");
    while (1)
    {
        b_endstop_x_a = false;
        vTaskDelay(pdMS_TO_TICKS(50));
        if (b_endstop_x_a == true)
        {
            count_x_a++;
            if (count_x_a % 2 == 0)
                ESP_LOGI(tag_utils, "count_x_a: %d", (int)(count_x_a / 2));
            // ESP_LOGI(tag_utils, "count_x_a: %d", (int)(count_x_a));
        }
    }
};

void count_intr_endstop_x_b(void *arg)
{
    ESP_LOGI(tag_utils, "listening to: endstop_x_b");
    while (1)
    {
        b_endstop_x_b = false;
        vTaskDelay(pdMS_TO_TICKS(50));
        if (b_endstop_x_b == true)
        {
            count_x_b++;
            if (count_x_b % 2 == 0)
                ESP_LOGI(tag_utils, "count_x_b: %d", (int)(count_x_b / 2));
            // ESP_LOGI(tag_utils, "count_x_b: %d", (int)(count_x_b));
        }
    }
};

void count_intr_endstop_y_a(void *arg)
{
    ESP_LOGI(tag_utils, "listening to: endstop_y_a");
    while (1)
    {
        b_endstop_y_a = false;
        vTaskDelay(pdMS_TO_TICKS(50));
        if (b_endstop_y_a == true)
        {
            count_y_a++;
            if (count_y_a % 2 == 0)
                ESP_LOGI(tag_utils, "count_y_a: %d", (int)(count_y_a / 2));
            // ESP_LOGI(tag_utils, "count_y_a: %d", (int)(count_y_a));
        }
    }
};

void count_intr_endstop_y_b(void *arg)
{
    ESP_LOGI(tag_utils, "listening to: endstop_y_b");
    while (1)
    {
        b_endstop_y_b = false;
        vTaskDelay(pdMS_TO_TICKS(50));
        if (b_endstop_y_b == true)
        {
            count_y_b++;
            if (count_y_b % 2 == 0)
                ESP_LOGI(tag_utils, "count_y_b: %d", (int)(count_y_b / 2));
            // ESP_LOGI(tag_utils, "count_y_b: %d", (int)(count_y_b));
        }
    }
};

void count_intr_sensor_mot1(void *arg)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        b_sensor_mot1 = false;
    }
};

void count_intr_sensor_mot2(void *arg)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        b_sensor_mot2 = false;
    }
};

bool get_b_endstop_x_a()
{
    ESP_LOGI(tag_utils, "running get_b_endstop_x_a()");
    return b_endstop_x_a;
};

bool get_b_endstop_x_b()
{
    ESP_LOGI(tag_utils, "running get_b_endstop_x_b()");
    return b_endstop_x_b;
};

uint64_t get_count_x_a()
{
    ESP_LOGI(tag_utils, "running get_count_x_a()");
    return count_x_a;
};

uint64_t get_count_x_b()
{
    ESP_LOGI(tag_utils, "running get_count_x_b()");
    return count_x_b;
};

bool get_b_endstop_y_a()
{
    ESP_LOGI(tag_utils, "running get_b_endstop_y_a()");
    return b_endstop_y_a;
};

bool get_b_endstop_y_b()
{
    ESP_LOGI(tag_utils, "running get_b_endstop_y_b()");
    return b_endstop_y_b;
};

uint64_t get_count_y_a()
{
    ESP_LOGI(tag_utils, "running get_count_y_a()");
    return count_y_a;
};

uint64_t get_count_y_b()
{
    ESP_LOGI(tag_utils, "running get_count_y_b()");
    return count_y_b;
};
