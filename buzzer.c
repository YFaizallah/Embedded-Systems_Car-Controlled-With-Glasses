#include "buzzer.h"
#include "hardware/pwm.h"


// PWM slice for buzzer
static uint slice_num_buzzer;

// Initialize the buzzer
void buzzer_init(void) {
    // Set the GPIO pin to function as a PWM output
    gpio_set_function(GPIO_PIN_BUZZER, GPIO_FUNC_PWM);

    // Get the PWM slice number for the buzzer
    slice_num_buzzer = pwm_gpio_to_slice_num(GPIO_PIN_BUZZER);

    // Set the PWM frequency (default frequency, can be changed)
    pwm_set_clkdiv(slice_num_buzzer, 4);  // Set clock divider for desired frequency
    pwm_set_wrap(slice_num_buzzer, 1000); // Set the wrap value for PWM (range of 0 to 1000)
    pwm_set_gpio_level(GPIO_PIN_BUZZER, 0); // Start with the buzzer off

    // Enable PWM for the buzzer
    pwm_set_enabled(slice_num_buzzer, true);
}

// Turn the buzzer on
void buzzer_on(void) {
    pwm_set_gpio_level(GPIO_PIN_BUZZER, 500);  // Set a mid-level value for the buzzer sound
}

// Turn the buzzer off
void buzzer_off(void) {
    pwm_set_gpio_level(GPIO_PIN_BUZZER, 0);  // Set the level to 0 to turn off the buzzer
}

// Make a sound (for passive buzzer, frequency control)
void buzzer_sound(uint32_t frequency, uint32_t duration_ms) {
    // Calculate PWM parameters based on frequency
    uint32_t wrap_value = 125000000 / frequency;
    pwm_set_wrap(slice_num_buzzer, wrap_value);  // Adjust the PWM wrap value based on frequency
    pwm_set_gpio_level(GPIO_PIN_BUZZER, wrap_value / 2); // Set duty cycle to 50%

    // Wait for the duration
    sleep_ms(duration_ms);

    // Turn off the buzzer after the sound duration
    buzzer_off();
}
