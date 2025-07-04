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
void kinematic_object_test(void *args);

kinematic *p_kinematic = nullptr;
kinematic_config i_config = kinematic_config::X_AXIS;

extern "C" void app_main()
{
    config_pin_io();

    p_kinematic = new kinematic(1, i_config, mot1_pin_a, mot1_pin_b, mot2_pin_a, mot2_pin_b, mot1_slp, mot2_slp,
                                LEDC_TIMER_0, LEDC_CHANNEL_0, LEDC_CHANNEL_1, LEDC_CHANNEL_2, LEDC_CHANNEL_3, true, false);

    // xTaskCreate(safe_home_routine, "safe_home_routine", 4096, NULL, 1, NULL);

    config_pin_intr();

    xTaskCreate(count_intr_endstop_x_a, "test_switch_x_a", 4096, NULL, 1, NULL);
    xTaskCreate(count_intr_endstop_x_b, "test_switch_x_b", 4096, NULL, 1, NULL);
    xTaskCreate(count_intr_endstop_y_a, "test_switch_y_a", 4096, NULL, 1, NULL);
    xTaskCreate(count_intr_endstop_y_b, "test_switch_y_b", 4096, NULL, 1, NULL);

    xTaskCreate(kinematic_object_test, "kinematic_object_test", 4096, NULL, 2, NULL);

    while (1)
        vTaskDelay(pdMS_TO_TICKS(50));
}

void kinematic_object_test(void *args)
{
    ESP_LOGI(tag_prototype, "test_run_x starting");
    ESP_LOGI(tag_prototype, "kinematic pointer: %p", p_kinematic);

    while (1)
    {
        if (p_kinematic != nullptr)
        {
            p_kinematic->set_pwm(127);

            p_kinematic->set_kinematic_config(kinematic_config::X_AXIS);
            p_kinematic->enable(true);

            vTaskDelay(pdMS_TO_TICKS(5000));
            p_kinematic->enable(false);

            p_kinematic->set_kinematic_config(kinematic_config::Y_AXIS);
            p_kinematic->enable(true);

            vTaskDelay(pdMS_TO_TICKS(5000));
            p_kinematic->enable(false);

            p_kinematic->set_kinematic_config(kinematic_config::QUAD13);
            p_kinematic->enable(true);

            vTaskDelay(pdMS_TO_TICKS(5000));
            p_kinematic->enable(false);

            p_kinematic->set_kinematic_config(kinematic_config::QUAD24);
            p_kinematic->enable(true);

            vTaskDelay(pdMS_TO_TICKS(5000));
            p_kinematic->enable(false);
        }

        // {
        // if (p_kinematic != nullptr)
        //     // if (count_x_a != get_count_x_a())
        //     if (get_b_endstop_x_a())
        //     {
        //         vTaskDelay(pdMS_TO_TICKS(50));
        //         // count_x_a = get_count_x_a();
        //         // if (count_x_a % 2 == 0)
        //         {
        //             p_kinematic->enable(true);
        //             p_kinematic->set_direction(false);
        //         }
        //     }
        //     else
        //         vTaskDelay(pdMS_TO_TICKS(10));

        //     // if (count_x_b != get_count_x_b())
        //     if (get_b_endstop_x_b())
        //     {
        //         vTaskDelay(pdMS_TO_TICKS(50));
        //         // count_x_b = get_count_x_b();
        //         // if (count_x_b % 2 == 0)
        //         {
        //             p_kinematic->enable(true);
        //             p_kinematic->set_direction(true);
        //         }
        //     }
        //     else
        //         vTaskDelay(pdMS_TO_TICKS(10));
        // }
    }
};

// void safe_home_routine(void *args)
// {
//     ESP_LOGI(tag_prototype, "safe_home_routine starting");
//     int64_t cache_time = esp_timer_get_time() / 1000;
//     int64_t temp_time = esp_timer_get_time() / 1000;
//     while (1)
//     {
//         while (temp_time - cache_time >= 5000)
//         {
//             ESP_LOGI(tag_prototype, "safe_home_routine check performed");
//             cache_time = temp_time;
//             if (0 == gpio_get_level(endstop_x_a) && motor_x != nullptr)
//             {
//                 motor_x->enable(false);
//                 motor_x->set_direction(false);
//                 motor_x->set_pwm(127);
//                 motor_x->enable(true);
//             }
//             if (0 == gpio_get_level(endstop_x_b) && motor_x != nullptr)
//             {
//                 motor_x->enable(false);
//                 motor_x->set_direction(true);
//                 motor_x->set_pwm(127);
//                 motor_x->enable(true);
//             }
//         }
//         temp_time = esp_timer_get_time() / 1000;
//     }
// };