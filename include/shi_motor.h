#pragma once

#include <freertos/FreeRTOS.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <iot_servo.h>

enum kinematic_config
{
    X_AXIS = 0,
    Y_AXIS = 1,
    QUAD13 = 2,
    QUAD24 = 3
};

class servo_motor
{
private:
    uint8_t m_id;
    gpio_num_t m_pin;
    ledc_timer_t m_timer;
    ledc_channel_t m_channel;

private:
    bool m_enable = false;
    bool m_dir = false;
    double m_min_microsec = 500;
    double m_max_microsec = 2500;
    double m_max_degree = 180;
    uint32_t m_freq = 50;

public:
    servo_motor(
        uint8_t i_id,
        gpio_num_t i_pin_a,
        ledc_timer_t i_timer,
        ledc_channel_t i_channel);

    uint32_t angle_to_duty_us(int angle);
    void run(double i_degrees);
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
    gpio_num_t m_pin_a_p;
    gpio_num_t m_pin_a_n;
    gpio_num_t m_pin_b_p;
    gpio_num_t m_pin_b_n;
    gpio_num_t m_pin_a_en;
    gpio_num_t m_pin_b_en;
    ledc_timer_t m_timer;
    ledc_channel_t m_channel_a;
    ledc_channel_t m_channel_b;
    bool m_dir;
    bool m_enable;
    uint8_t m_pwm;

public:
    kinematic(
        uint8_t i_id,
        kinematic_config i_config,
        gpio_num_t i_pin_a_p,
        gpio_num_t i_pin_a_n,
        gpio_num_t i_pin_b_p,
        gpio_num_t i_pin_b_n,
        gpio_num_t i_pin_a_en,
        gpio_num_t i_pin_b_en,
        ledc_timer_t i_timer,
        ledc_channel_t i_channel_a,
        ledc_channel_t i_channel_b,
        bool i_dir = true,
        bool i_en = true);
    void set_pwm(uint8_t i_pwm);
    uint8_t get_pwm();

    void set_kinematic_config(kinematic_config i_kinematic_config);
    kinematic_config get_kinematic_config();

    void set_direction(bool i_direction);
    bool get_direction();

    void enable(bool i_enable);
    bool is_enabled();
    void run();

    void rotate_once_ch_a();
    void rotate_once_ch_b();
    void rotate_ch_a(uint64_t i_num_of_rotations);
    void rotate_ch_b(uint64_t i_num_of_rotations);
};