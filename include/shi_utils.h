#pragma once

#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <esp_intr_alloc.h>

void IRAM_ATTR intr_endstop_x_a(void *arg);
void IRAM_ATTR intr_endstop_x_b(void *arg);

void IRAM_ATTR intr_endstop_y_a(void *arg);
void IRAM_ATTR intr_endstop_y_b(void *arg);

void IRAM_ATTR intr_sensor_mot1(void *arg);
void IRAM_ATTR intr_sensor_mot2(void *arg);

void count_intr_endstop_x_a(void *arg);
void count_intr_endstop_x_b(void *arg);

void count_intr_endstop_y_a(void *arg);
void count_intr_endstop_y_b(void *arg);

void count_intr_sensor_mot1(void *arg);
void count_intr_sensor_mot2(void *arg);

void config_pin_io();
void config_pin_intr();

bool get_b_endstop_x_a();
bool get_b_endstop_x_b();

uint64_t get_count_x_a();
uint64_t get_count_x_b();

bool get_b_endstop_y_a();
bool get_b_endstop_y_b();

uint64_t get_count_y_a();
uint64_t get_count_y_b();

// class PID
// {
//     PID();
//     ~PID();

// private:
//     double m_prop_var;
//     double m_inte_var;
//     double m_deri_var;

// public:
//     void set_var(double &i_var);
//     void get_var(double &i_var);

//     void set_prop_var(double &i_prop_var);
//     void get_prop_var(double &o_prop_var);

//     void set_inte_var(double &i_inte_var);
//     void get_inte_var(double &o_inte_var);

//     void set_deri_var(double &i_deri_var);
//     void get_deri_var(double &o_deri_var);
// };
