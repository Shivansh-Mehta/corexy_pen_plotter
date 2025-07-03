#pragma once

// global definitions
#define time_delay_1 100 / portTICK_PERIOD_MS
#define time_delay_2 200 / portTICK_PERIOD_MS
#define time_delay_3 300 / portTICK_PERIOD_MS
#define global_enable GPIO_NUM_4

// motor 1 drive unit
#define mot1_pin_a GPIO_NUM_22  // op pin
#define mot1_pin_b GPIO_NUM_23  // op pin
#define mot1_slp GPIO_NUM_18    // op pin
#define sensor_mot1 GPIO_NUM_16 // ip pin

// motor 2 drive unit
#define mot2_pin_a GPIO_NUM_26  // op pin
#define mot2_pin_b GPIO_NUM_27  // op pin
#define mot2_slp GPIO_NUM_19    // op pin
#define sensor_mot2 GPIO_NUM_17 // ip pin

// kinematic assembly
#define endstop_x_a GPIO_NUM_33 // ip pin
#define endstop_x_b GPIO_NUM_32 // ip pin
#define endstop_y_a GPIO_NUM_12 // ip pin
#define endstop_y_b GPIO_NUM_13 // ip pin

// pen holder system servo
#define phs_servo GPIO_NUM_9 // op pin
