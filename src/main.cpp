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

void safe_home_routine(void *args);
void kin_obj_free_run_test(void *args);
void kin_obj_limit_test_x(void *args);
void kin_obj_limit_test_y(void *args);
void kin_obj_limit_test_q13(void *args);
void kin_obj_limit_test_q24(void *args);

kinematic *p_kinematic = nullptr;
kinematic_config i_config = kinematic_config::X_AXIS;

extern "C" void app_main()
{
    config_pin_io();
    config_pin_intr();

    // xTaskCreate(safe_home_routine, "safe_home_routine", 4096, NULL, 1, NULL);
    p_kinematic = new kinematic(1, i_config, mot1_pin_a, mot1_pin_b, mot2_pin_a, mot2_pin_b, mot1_slp, mot2_slp,
                                LEDC_TIMER_0, LEDC_CHANNEL_0, LEDC_CHANNEL_1, true, true);
    // xTaskCreate(kin_obj_free_run_test, "kin_obj_free_run_test", 4096, NULL, 2, NULL);
    if (p_kinematic != nullptr)
    {
        p_kinematic->enable(false);
        // p_kinematic->set_pwm(95);
        p_kinematic->set_pwm(127);
        p_kinematic->set_kinematic_config(kinematic_config::X_AXIS);
        p_kinematic->enable(true);
        p_kinematic->run();
    }

    xTaskCreate(kin_obj_limit_test_x, "kin_obj_limit_test_x", 4096, NULL, 2, NULL);
    // xTaskCreate(kin_obj_limit_test_y, "kin_obj_limit_test_y", 4096, NULL, 2, NULL);
    // xTaskCreate(kin_obj_limit_test_y, "kin_obj_limit_test_q13", 4096, NULL, 2, NULL);
    // xTaskCreate(kin_obj_limit_test_y, "kin_obj_limit_test_q24", 4096, NULL, 2, NULL);

    xTaskCreate(count_intr_endstop_x_a, "test_switch_x_a", 4096, NULL, 1, NULL);
    xTaskCreate(count_intr_endstop_x_b, "test_switch_x_b", 4096, NULL, 1, NULL);
    xTaskCreate(count_intr_endstop_y_a, "test_switch_y_a", 4096, NULL, 1, NULL);
    xTaskCreate(count_intr_endstop_y_b, "test_switch_y_b", 4096, NULL, 1, NULL);

    while (1)
        vTaskDelay(pdMS_TO_TICKS(50));
}

void kin_obj_free_run_test(void *args)
{
    ESP_LOGI(tag_prototype, "kin_obj_free_run_test starting");
    ESP_LOGI(tag_prototype, "kinematic pointer: %p", p_kinematic);

    while (1)
    {
        if (p_kinematic != nullptr)
        {
            p_kinematic->enable(false);
            p_kinematic->set_pwm(95);

            vTaskDelay(pdMS_TO_TICKS(5000));
            p_kinematic->enable(false);

            p_kinematic->set_kinematic_config(kinematic_config::X_AXIS);
            p_kinematic->enable(true);
            p_kinematic->run();

            vTaskDelay(pdMS_TO_TICKS(5000));
            p_kinematic->enable(false);

            p_kinematic->set_kinematic_config(kinematic_config::Y_AXIS);
            p_kinematic->enable(true);
            p_kinematic->run();

            vTaskDelay(pdMS_TO_TICKS(5000));
            p_kinematic->enable(false);

            p_kinematic->set_kinematic_config(kinematic_config::QUAD13);
            p_kinematic->enable(true);
            p_kinematic->run();

            vTaskDelay(pdMS_TO_TICKS(5000));
            p_kinematic->enable(false);

            p_kinematic->set_kinematic_config(kinematic_config::QUAD24);
            p_kinematic->enable(true);
            p_kinematic->run();
        }
    }
};

void kin_obj_limit_test_x(void *args)
{
    ESP_LOGI(tag_prototype, "kin_obj_limit_test_x starting");
    while (1)
    {
        if (p_kinematic != nullptr)
        {
            if (get_b_endstop_x_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                p_kinematic->enable(false);
                p_kinematic->set_direction(true);
                p_kinematic->enable(true);
                p_kinematic->run();
            }

            if (get_b_endstop_x_b())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                p_kinematic->enable(false);
                p_kinematic->set_direction(false);
                p_kinematic->enable(true);
                p_kinematic->run();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
};

void kin_obj_limit_test_y(void *args)
{
    ESP_LOGI(tag_prototype, "kin_obj_limit_test_y starting");
    while (1)
    {
        if (p_kinematic != nullptr)
        {
            if (get_b_endstop_y_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                p_kinematic->enable(false);
                p_kinematic->set_direction(true);
                p_kinematic->enable(true);
                p_kinematic->run();
            }

            if (get_b_endstop_y_b())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                p_kinematic->enable(false);
                p_kinematic->set_direction(false);
                p_kinematic->enable(true);
                p_kinematic->run();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
};

void kin_obj_limit_test_q13(void *args)
{
    ESP_LOGI(tag_prototype, "kin_obj_limit_test_q13 starting");
    while (1)
    {
        if (p_kinematic != nullptr)
        {
            if (get_b_endstop_y_a() || get_b_endstop_x_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                p_kinematic->enable(false);
                p_kinematic->set_direction(false);
                p_kinematic->enable(true);
                p_kinematic->run();
            }

            if (get_b_endstop_y_b() || get_b_endstop_x_b())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                p_kinematic->enable(false);
                p_kinematic->set_direction(true);
                p_kinematic->enable(true);
                p_kinematic->run();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
};

void kin_obj_limit_test_q24(void *args)
{
    ESP_LOGI(tag_prototype, "kin_obj_limit_test_q24 starting");
    while (1)
    {
        if (p_kinematic != nullptr)
        {
            if (get_b_endstop_y_a() || get_b_endstop_x_b())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                p_kinematic->enable(false);
                p_kinematic->set_direction(true);
                p_kinematic->enable(true);
                p_kinematic->run();
            }

            if (get_b_endstop_y_b() || get_b_endstop_x_a())
            {
                vTaskDelay(pdMS_TO_TICKS(50));
                p_kinematic->enable(false);
                p_kinematic->set_direction(false);
                p_kinematic->enable(true);
                p_kinematic->run();
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
};

void safe_home_routine(void *args)
{
    ESP_LOGI(tag_prototype, "safe_home_routine starting");
    int32_t inspection_counter = 0;
    while (1)
    {
        ESP_LOGI(tag_prototype, "safe_home_routine inspection %d", (int)inspection_counter++);
        if (get_b_endstop_x_a() && p_kinematic != nullptr)
        {
            p_kinematic->enable(false);
            p_kinematic->set_direction(true);
            p_kinematic->set_pwm(95);
            p_kinematic->set_kinematic_config(kinematic_config::X_AXIS);
            p_kinematic->enable(true);
            p_kinematic->run();
        }
        if (get_b_endstop_x_b() && p_kinematic != nullptr)
        {
            p_kinematic->enable(false);
            p_kinematic->set_direction(false);
            p_kinematic->set_pwm(95);
            p_kinematic->set_kinematic_config(kinematic_config::X_AXIS);
            p_kinematic->enable(true);
            p_kinematic->run();
        }
        if (get_b_endstop_y_a() && p_kinematic != nullptr)
        {
            p_kinematic->enable(false);
            p_kinematic->set_direction(false);
            p_kinematic->set_pwm(95);
            p_kinematic->set_kinematic_config(kinematic_config::Y_AXIS);
            p_kinematic->enable(true);
            p_kinematic->run();
        }
        if (get_b_endstop_y_b() && p_kinematic != nullptr)
        {
            p_kinematic->enable(false);
            p_kinematic->set_direction(true);
            p_kinematic->set_pwm(95);
            p_kinematic->set_kinematic_config(kinematic_config::Y_AXIS);
            p_kinematic->enable(true);
            p_kinematic->run();
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
};