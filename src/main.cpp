// esp32 generic headers
#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/uart.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <esp_task_wdt.h>

// custom headers
#include "shi_definitions.h"
#include "shi_utils.h"
#include "shi_motor.h"

const char *tag_prototype = "prototype";

void run_x(void *args);
void run_y(void *args);

void test_run_x(void *args);
void test_run_y(void *args);

void test_run_x_a(void *args);
void test_run_x_b(void *args);
void test_run_y_a(void *args);
void test_run_y_b(void *args);

void setup_mot_a(void *args);
void setup_mot_b(void *args);

motor *motor_a = nullptr;
motor *motor_b = nullptr;
virtual_motor *motor_x = nullptr;

bool b_motor_a_set = false;
bool b_motor_b_set = false;
bool b_task_a_set = false;
bool b_task_b_set = false;

extern "C" void app_main()
{
    config_pin_io();
    config_pin_intr();

    // motor motor_a(1, mot1_pin_a, mot1_pin_b, mot_slp, LEDC_TIMER_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3, true, false);
    // motor_a.set_pwm(127);
    // motor motor_b(2, mot2_pin_a, mot2_pin_b, mot_slp, LEDC_TIMER_0, LEDC_CHANNEL_0, LEDC_CHANNEL_1, true, false);
    // motor_b.set_pwm(127);

    // while (1)
    //     vTaskDelay(pdMS_TO_TICKS(50));

    // TaskHandle_t handle_setup_mot_a;
    // TaskHandle_t handle_setup_mot_b;

    // xTaskCreate(setup_mot_a, "setup_mot_a", 4096, NULL, 2, &handle_setup_mot_a);
    // xTaskCreate(setup_mot_b, "setup_mot_a", 4096, NULL, 2, &handle_setup_mot_b);
    // if (b_motor_a_set)
    //     vTaskDelete(handle_setup_mot_a);
    // if (b_motor_b_set)
    //     vTaskDelete(handle_setup_mot_b);

    // motor_a = new motor(1, mot1_pin_a, mot1_pin_b, mot_slp, LEDC_TIMER_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3, true, false);
    // if (nullptr != motor_a)
    // {
    //     motor_a->set_direction(true);
    //     motor_a->set_pwm(127);
    // }
    // motor_b = new motor(2, mot2_pin_a, mot2_pin_b, mot_slp, LEDC_TIMER_0, LEDC_CHANNEL_0, LEDC_CHANNEL_1, true, false);
    // if (nullptr != motor_b)
    // {
    //     motor_b->set_direction(true);
    //     motor_b->set_pwm(127);
    // }

    // TaskHandle_t handle_x;
    // TaskHandle_t handle_y;
    // TaskHandle_t handle_test_run_x;
    // TaskHandle_t handle_test_run_y;
    // xTaskCreate(run_x, "run_x", 4096, motor_a, 1, &handle_x);
    // xTaskCreate(run_y, "run_y", 4096, motor_b, 1, &handle_y);
    // xTaskCreate(test_run_x_a, "test_run_x", 4096, NULL, 2, NULL);
    // xTaskCreate(test_run_x_b, "test_run_x", 4096, NULL, 2, NULL);
    // xTaskCreate(test_run_y_a, "test_run_y", 4096, NULL, 2, NULL);
    // xTaskCreate(test_run_y_b, "test_run_y", 4096, NULL, 2, NULL);

    motor_x = new virtual_motor(1, mot1_pin_a, mot1_pin_b, mot2_pin_a, mot2_pin_b, mot1_slp, mot2_slp, LEDC_TIMER_0, LEDC_CHANNEL_0, LEDC_CHANNEL_1, true, false);
    if (nullptr != motor_x)
    {
        motor_x->enable(true);
        motor_x->set_direction(true);
        motor_x->set_pwm(127);
    }

    xTaskCreate(count_intr_endstop_x_a, "test_switch_x_a", 4096, NULL, 1, NULL);
    xTaskCreate(count_intr_endstop_x_b, "test_switch_x_b", 4096, NULL, 1, NULL);
    xTaskCreate(test_run_x, "test_run_x", 4096, NULL, 2, NULL);
    // xTaskCreate(count_intr_endstop_y_a, "test_switch_y_a", 4096, NULL, 1, NULL);
    // xTaskCreate(count_intr_endstop_y_b, "test_switch_y_b", 4096, NULL, 1, NULL);

    // delete (motor_a);
    // motor_a = nullptr;
    // delete (motor_b);
    // motor_b = nullptr;

    while (1)
        vTaskDelay(pdMS_TO_TICKS(50));
}

void setup_mot_a(void *args)
{
    motor_a = new motor(1, mot1_pin_a, mot1_pin_b, mot1_slp, LEDC_TIMER_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3, true, false);
    if (nullptr != motor_a)
    {
        motor_a->enable(true);
        motor_a->set_direction(true);
        motor_a->set_pwm(127);
        b_motor_a_set = true;
    }
    while (1)
        vTaskDelay(pdMS_TO_TICKS(50));
};

void setup_mot_b(void *args)
{
    motor_b = new motor(2, mot2_pin_a, mot2_pin_b, mot2_slp, LEDC_TIMER_0, LEDC_CHANNEL_0, LEDC_CHANNEL_1, true, false);
    if (nullptr != motor_b)
    {
        motor_b->enable(true);
        motor_b->set_direction(true);
        motor_b->set_pwm(127);
        b_motor_b_set = true;
    }
    while (1)
        vTaskDelay(pdMS_TO_TICKS(50));
};

void run_x(void *args)
{
    ESP_LOGI(tag_prototype, "run_x starting");
    if (nullptr != motor_a)
    {
        motor_a->set_direction(true);
        motor_a->set_pwm(127);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    while (1)
        vTaskDelay(pdMS_TO_TICKS(50));
};

void run_y(void *args)
{
    ESP_LOGI(tag_prototype, "run_y starting");
    if (nullptr != motor_b)
    {
        motor_b->set_direction(true);
        motor_b->set_pwm(127);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    while (1)
        vTaskDelay(pdMS_TO_TICKS(50));
};

void test_run_x_a(void *args)
{
    ESP_LOGI(tag_prototype, "test_run_x starting");
    ESP_LOGI(tag_prototype, "motor pointer: %p", motor_a);
    uint64_t count_x_a = get_count_x_a();
    uint64_t count_x_b = get_count_x_b();

    while (1)
    {
        if (motor_a != nullptr && motor_b != nullptr)
        {
            if (count_x_a != get_count_x_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                count_x_a = get_count_x_a();
                if (count_x_a % 2 == 0)
                {
                    motor_a->enable(true);
                    motor_a->set_direction(false);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
            if (count_x_b != get_count_x_b())
            {

                vTaskDelay(pdMS_TO_TICKS(50));
                count_x_b = get_count_x_b();
                if (count_x_b % 2 == 0)
                {
                    motor_a->enable(true);
                    motor_a->set_direction(true);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void test_run_x_b(void *args)
{
    ESP_LOGI(tag_prototype, "test_run_x starting");
    ESP_LOGI(tag_prototype, "motor pointer: %p", motor_a);
    uint64_t count_x_a = get_count_x_a();
    uint64_t count_x_b = get_count_x_b();

    while (1)
    {
        if (motor_a != nullptr && motor_b != nullptr)
        {
            if (count_x_a != get_count_x_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                count_x_a = get_count_x_a();
                if (count_x_a % 2 == 0)
                {
                    motor_b->enable(true);
                    motor_b->set_direction(true);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
            if (count_x_b != get_count_x_b())
            {

                vTaskDelay(pdMS_TO_TICKS(50));
                count_x_b = get_count_x_b();
                if (count_x_b % 2 == 0)
                {
                    motor_b->enable(true);
                    motor_b->set_direction(false);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void test_run_y_a(void *args)
{
    ESP_LOGI(tag_prototype, "test_run_y starting");
    ESP_LOGI(tag_prototype, "motor pointer: %p", motor_b);
    uint64_t count_y_a = get_count_y_a();
    uint64_t count_y_b = get_count_y_b();

    bool local_dir_y = motor_b->get_direction();
    while (1)
    {
        if (motor_a != nullptr && motor_b != nullptr)
        {
            if (count_y_a != get_count_y_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                count_y_a = get_count_y_a();
                if (count_y_a % 2 == 0)
                {
                    motor_a->enable(true);
                    motor_a->set_direction(!local_dir_y);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
            if (count_y_b != get_count_y_b())
            {

                vTaskDelay(pdMS_TO_TICKS(50));
                count_y_b = get_count_y_b();
                if (count_y_b % 2 == 0)
                {
                    motor_a->enable(true);
                    motor_a->set_direction(local_dir_y);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void test_run_y_b(void *args)
{
    ESP_LOGI(tag_prototype, "test_run_y starting");
    ESP_LOGI(tag_prototype, "motor pointer: %p", motor_b);
    uint64_t count_y_a = get_count_y_a();
    uint64_t count_y_b = get_count_y_b();

    bool local_dir_y = motor_b->get_direction();
    while (1)
    {
        if (motor_a != nullptr && motor_b != nullptr)
        {
            if (count_y_a != get_count_y_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                count_y_a = get_count_y_a();
                if (count_y_a % 2 == 0)
                {
                    motor_b->enable(true);
                    motor_b->set_direction(local_dir_y);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
            if (count_y_b != get_count_y_b())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                count_y_b = get_count_y_b();
                if (count_y_b % 2 == 0)
                {
                    motor_b->enable(true);
                    motor_b->set_direction(!local_dir_y);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void test_run_x(void *args)
{
    ESP_LOGI(tag_prototype, "test_run_x starting");
    ESP_LOGI(tag_prototype, "motor pointer: %p", motor_x);
    uint64_t count_x_a = get_count_x_a();
    uint64_t count_x_b = get_count_x_b();
    while (1)
    {
        if (motor_x != nullptr)
        {
            if (count_x_a != get_count_y_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                count_x_a = get_count_y_a();
                if (count_x_a % 2 == 0)
                {
                    motor_x->enable(true);
                    motor_x->set_direction(true);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
            if (count_x_b != get_count_x_b())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                count_x_b = get_count_x_b();
                if (count_x_b % 2 == 0)
                {
                    motor_x->enable(true);
                    motor_x->set_direction(false);
                }
            }
            else
                vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void test_run_y(void *args)
{
    ESP_LOGI(tag_prototype, "test_run_y starting");
    ESP_LOGI(tag_prototype, "motor pointer: %p", motor_b);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
};
