#include "motor_driver.h"

// PWM configuration
void setup_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM); // Configure pin as PWM
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, 255);           // Set PWM resolution (0-255)
    pwm_set_clkdiv(slice_num, 4.0f);        // Set clock divider
    pwm_set_enabled(slice_num, true);      // Enable PWM output
}

// Set PWM duty cycle for motor enable pins
void set_motor_speed(uint gpio, uint16_t speed) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_gpio_level(gpio, speed); // Set duty cycle (0-255)
}

// Initialize GPIO pins
void motor_init() {
    // Set motor control pins as output
    gpio_init(MOTOR1_IN1);
    gpio_init(MOTOR1_IN2);
    gpio_init(MOTOR2_IN3);
    gpio_init(MOTOR2_IN4);
    gpio_set_dir(MOTOR1_IN1, GPIO_OUT);
    gpio_set_dir(MOTOR1_IN2, GPIO_OUT);
    gpio_set_dir(MOTOR2_IN3, GPIO_OUT);
    gpio_set_dir(MOTOR2_IN4, GPIO_OUT);

    // Setup PWM for enable pins
    setup_pwm(MOTOR1_EN);
    setup_pwm(MOTOR2_EN);
}

// Motor control functions
void move_backward(uint16_t speed) {
    gpio_put(MOTOR1_IN1, 1);
    gpio_put(MOTOR1_IN2, 0);
    gpio_put(MOTOR2_IN3, 0);
    gpio_put(MOTOR2_IN4, 1);
    set_motor_speed(MOTOR1_EN, (speed/2)+15);
    set_motor_speed(MOTOR2_EN, speed/2);
}

void move_backward_left(uint16_t speed) {
    gpio_put(MOTOR1_IN1, 1);
    gpio_put(MOTOR1_IN2, 0);
    gpio_put(MOTOR2_IN3, 0);
    gpio_put(MOTOR2_IN4, 1);
    set_motor_speed(MOTOR1_EN, speed/2);
    set_motor_speed(MOTOR2_EN, speed/2);
}

void move_backward_right(uint16_t speed) {
    gpio_put(MOTOR1_IN1, 1);
    gpio_put(MOTOR1_IN2, 0);
    gpio_put(MOTOR2_IN3, 0);
    gpio_put(MOTOR2_IN4, 1);
    set_motor_speed(MOTOR1_EN, speed/2);
    set_motor_speed(MOTOR2_EN, speed/2);
}

void move_forward(uint16_t speed) {
    gpio_put(MOTOR1_IN1, 0);
    gpio_put(MOTOR1_IN2, 1);
    gpio_put(MOTOR2_IN3, 1);
    gpio_put(MOTOR2_IN4, 0);
    set_motor_speed(MOTOR1_EN, (speed/2)+18);
    set_motor_speed(MOTOR2_EN, speed/2);
}

void move_left(uint16_t speed) {
    gpio_put(MOTOR1_IN1, 0);
    gpio_put(MOTOR1_IN2, 1);
    gpio_put(MOTOR2_IN3, 1);
    gpio_put(MOTOR2_IN4, 0);
    set_motor_speed(MOTOR1_EN, (speed/2)+24 );
    set_motor_speed(MOTOR2_EN, speed/2);
}

void move_right(uint16_t speed) {
     gpio_put(MOTOR1_IN1, 0);
    gpio_put(MOTOR1_IN2, 1);
    gpio_put(MOTOR2_IN3, 1);
    gpio_put(MOTOR2_IN4, 0);
    set_motor_speed(MOTOR1_EN, (speed/2)+18);
    set_motor_speed(MOTOR2_EN, (speed/2)+6);
}

void stop_car() {
    gpio_put(MOTOR1_IN1, 0);
    gpio_put(MOTOR1_IN2, 0);
    gpio_put(MOTOR2_IN3, 0);
    gpio_put(MOTOR2_IN4, 0);
    set_motor_speed(MOTOR1_EN, 0);
    set_motor_speed(MOTOR2_EN, 0);
}