#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <driver/ledc.h>

enum kinematic_config
{
    X_AXIS = 0,
    Y_AXIS = 1,
    QUAD13 = 2,
    QUAD24 = 3
};

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

class kinematic
{
private:
    uint8_t m_id;
    kinematic_config m_config;
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
    bool m_enable_a;
    bool m_enable_b;
    uint8_t m_pwm;

public:
    kinematic(
        uint8_t i_id,
        kinematic_config i_config,
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
        bool i_en_a = true,
        bool i_en_b = true);
    void set_pwm(uint8_t i_pwm);
    uint8_t get_pwm();

    void set_kinematic_config(kinematic_config i_kinematic_config);
    kinematic_config get_kinematic_config();

    void set_direction(bool i_direction);
    bool get_direction();

    void enable_a(bool i_enable_a);
    bool is_a_enabled();
    void enable_b(bool i_enable_b);
    bool is_b_enabled();
    void run();

    void config_X_AXIS();
    void config_Y_AXIS();
    void config_QUAD13();
    void config_QUAD24();
};