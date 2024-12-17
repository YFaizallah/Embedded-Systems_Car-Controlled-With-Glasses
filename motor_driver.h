#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"

// GPIO pin definitions for motor control
#define MOTOR1_IN1 0
#define MOTOR1_IN2 1
#define MOTOR1_EN 2

#define MOTOR2_IN3 12
#define MOTOR2_IN4 18
#define MOTOR2_EN  27

// PWM configuration
void setup_pwm(uint gpio);
void set_motor_speed(uint gpio, uint16_t speed);

// Motor initialization
void motor_init(void);

// Motor control functions
void move_forward(uint16_t speed);
void move_backward(uint16_t speed);
void move_backward_left(uint16_t speed);
void move_backward_right(uint16_t speed);
void move_left(uint16_t speed);
void move_right(uint16_t speed);
void stop_car(void);

#endif // MOTOR_DRIVER_H
