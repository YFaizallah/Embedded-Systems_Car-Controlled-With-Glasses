#ifndef RGB_LED_H
#define RGB_LED_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"


// GPIO pins for RGB LED (change these if needed)
#define GPIO_PIN_RED   6
#define GPIO_PIN_GREEN 7
#define GPIO_PIN_BLUE  8

// Function to initialize the RGB LED
void rgb_led_init(void);

// Function to set the color of the RGB LED by adjusting the PWM duty cycle
void set_rgb_color(uint8_t red, uint8_t green, uint8_t blue);

// Function to cycle through some test colors
void test_rgb_led(void);

#endif // RGB_LED_H
