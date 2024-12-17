#include "ir_sensor.h"

// Initialize the IR sensor
void ir_sensor_init(void) {
    gpio_init(IR_SENSOR_PIN);        // Initialize the GPIO pin
    gpio_set_dir(IR_SENSOR_PIN, GPIO_IN); // Set the GPIO pin as input
    gpio_pull_up(IR_SENSOR_PIN);     // Enable pull-up resistor
}

// Check if the IR sensor detects an object
bool ir_sensor_detected(void) {
    // Read the GPIO pin state
    // Return true if the pin reads LOW (object detected, typical for IR sensors)
    return !gpio_get(IR_SENSOR_PIN);
}
