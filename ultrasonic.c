#include "ultrasonic.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/time.h"

// Initialize the TRIG and ECHO pins
void ultrasonic_init(void) {
    // Initialize TRIG as output
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_put(TRIG_PIN, 0); // Ensure TRIG is LOW initially

    // Initialize ECHO as input
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

// Trigger the ultrasonic sensor to start a measurement
void ultrasonic_trigger(void) {
    // Set TRIG HIGH for 10 microseconds
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);
}

// Measure the distance in centimeters
float ultrasonic_get_distance(void) {
    // Trigger the sensor
    ultrasonic_trigger();

    // Wait for ECHO to go HIGH (start of echo)
    absolute_time_t timeout = make_timeout_time_us(20000); // 20 ms timeout
    while (gpio_get(ECHO_PIN) == 0) {
        if (absolute_time_diff_us(get_absolute_time(), timeout) < 0) {
            return -1; // Timeout waiting for ECHO to start
        }
    }

    // Record the start time
    absolute_time_t start_time = get_absolute_time();

    // Wait for ECHO to go LOW (end of echo)
    timeout = make_timeout_time_us(20000); // 20 ms timeout
    while (gpio_get(ECHO_PIN) == 1) {
        if (absolute_time_diff_us(get_absolute_time(), timeout) < 0) {
            return -1; // Timeout waiting for ECHO to end
        }
    }

    // Record the end time
    absolute_time_t end_time = get_absolute_time();

    // Calculate the duration in microseconds
    int64_t echo_duration = absolute_time_diff_us(start_time, end_time);

    // Calculate the distance in cm
    // Speed of sound is 343 m/s -> 0.0343 cm/µs
    float distance_cm = (echo_duration * 0.0343) / 2;

    // Validate the distance
    if (distance_cm > 400 || distance_cm < 2) {
        return -1; // Out of range
    }

    return distance_cm;
}
