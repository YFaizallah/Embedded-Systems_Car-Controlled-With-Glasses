#include "led.h"

// PWM slices for each color channel
static uint slice_num_red;
static uint slice_num_green;
static uint slice_num_blue;

// Initialize the RGB LED
void rgb_led_init(void) {
    // Initialize the GPIO pins for RGB as output
    gpio_set_function(GPIO_PIN_RED, GPIO_FUNC_PWM);
    gpio_set_function(GPIO_PIN_GREEN, GPIO_FUNC_PWM);
    gpio_set_function(GPIO_PIN_BLUE, GPIO_FUNC_PWM);

    // Get the PWM slice number for each color channel
    slice_num_red = pwm_gpio_to_slice_num(GPIO_PIN_RED);
    slice_num_green = pwm_gpio_to_slice_num(GPIO_PIN_GREEN);
    slice_num_blue = pwm_gpio_to_slice_num(GPIO_PIN_BLUE);

    // Set the PWM frequency (for simplicity, using a fixed clock divider)
    pwm_set_clkdiv(slice_num_red, 4);   // Change if necessary for desired frequency
    pwm_set_clkdiv(slice_num_green, 4);
    pwm_set_clkdiv(slice_num_blue, 4);

    pwm_set_wrap(slice_num_red, 255);
    pwm_set_wrap(slice_num_green, 255);
    pwm_set_wrap(slice_num_blue, 255);


    // Set initial PWM duty cycle to 0 (LED off)
    pwm_set_gpio_level(GPIO_PIN_RED, 0);
    pwm_set_gpio_level(GPIO_PIN_GREEN, 0);
    pwm_set_gpio_level(GPIO_PIN_BLUE, 0);

    // Enable PWM for each channel
    pwm_set_enabled(slice_num_red, true);
    pwm_set_enabled(slice_num_green, true);
    pwm_set_enabled(slice_num_blue, true);
}

// Set the RGB color by adjusting the PWM duty cycle
void set_rgb_color(uint8_t red, uint8_t green, uint8_t blue) {
    // Ensure values are within range
    red = (red > 255) ? 255 : red;
    green = (green > 255) ? 255 : green;
    blue = (blue > 255) ? 255 : blue;

    // Set the PWM duty cycle for each color channel
    pwm_set_gpio_level(GPIO_PIN_RED, red);
    pwm_set_gpio_level(GPIO_PIN_GREEN, green);
    pwm_set_gpio_level(GPIO_PIN_BLUE, blue);
}

// Test the RGB LED by cycling through various colors
void test_rgb_led(void) {
    // Red
    set_rgb_color(255, 0, 0);
    sleep_ms(1000);

    // Green
    set_rgb_color(0, 255, 0);
    sleep_ms(1000);

    // Blue
    set_rgb_color(0, 0, 255);
    sleep_ms(1000);

    // Yellow (Red + Green)
    set_rgb_color(255, 255, 0);
    sleep_ms(1000);

    // Purple (Red + Blue)
    set_rgb_color(255, 0, 255);
    sleep_ms(1000);

    // Cyan (Green + Blue)
    set_rgb_color(0, 255, 255);
    sleep_ms(1000);

    // White (Red + Green + Blue)
    set_rgb_color(255, 255, 255);
    sleep_ms(1000);
}
