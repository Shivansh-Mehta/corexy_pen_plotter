#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <driver/ledc.h>

class motor
{
private:
    uint8_t m_id;
    gpio_num_t m_pin_a;
    gpio_num_t m_pin_b;
    gpio_num_t m_pin_en;
    ledc_timer_t m_timer;
    ledc_channel_t m_channel_a;
    ledc_channel_t m_channel_b;
    bool m_dir;
    bool m_enable;
    uint8_t m_pwm;

public:
    motor(
        uint8_t i_id,
        gpio_num_t i_pin_a,
        gpio_num_t i_pin_b,
        gpio_num_t i_pin_en,
        ledc_timer_t i_timer,
        ledc_channel_t i_channel_a,
        ledc_channel_t i_channel_b,
        bool i_dir = true,
        bool i_en = true);
    void set_pwm(uint8_t i_pwm);
    uint8_t get_pwm();

    void set_direction(bool i_direction);
    bool get_direction();

    void enable(bool i_enable);
    bool is_enabled();
    void run(bool b_pin_a);
    void run(bool b_pin_a, uint8_t i_pwm);
};

class virtual_motor
{
private:
    uint8_t m_id;
    gpio_num_t m_pin_a_a;
    gpio_num_t m_pin_a_b;
    gpio_num_t m_pin_b_a;
    gpio_num_t m_pin_b_b;
    gpio_num_t m_pin_en_a;
    gpio_num_t m_pin_en_b;
    ledc_timer_t m_timer;
    ledc_channel_t m_channel_a;
    ledc_channel_t m_channel_b;
    bool m_dir;
    bool m_enable;
    uint8_t m_pwm;

public:
    virtual_motor(
        uint8_t i_id,
        gpio_num_t i_pin_a_a,
        gpio_num_t i_pin_a_b,
        gpio_num_t i_pin_b_a,
        gpio_num_t i_pin_b_b,
        gpio_num_t i_pin_en_a,
        gpio_num_t i_pin_en_b,
        ledc_timer_t i_timer,
        ledc_channel_t i_channel_a,
        ledc_channel_t i_channel_b,
        bool i_dir = true,
        bool i_en = true);
    void set_pwm(uint8_t i_pwm);
    uint8_t get_pwm();

    void set_direction(bool i_direction);
    bool get_direction();

    void enable(bool i_enable);
    bool is_enabled();
    void run();
};