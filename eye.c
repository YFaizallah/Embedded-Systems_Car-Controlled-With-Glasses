#include "eye.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Initialize the sensor (configure ADC)
void eye_init() {
    adc_init();                         // Initialize the ADC
    adc_gpio_init(EYE_SENSOR_PIN);      // Set up the GPIO pin for analog input
    adc_select_input(1);                // Use ADC channel 0 (corresponds to A0)
}

// Read the analog value from the sensor
uint16_t eye_read_sensor() {
    return adc_read(); // Read and return the analog value (0–4095 for 12-bit ADC)
}

// Check if an eye blink is detected
bool eye_detect_blink(uint16_t sensor_value) {
    if (sensor_value < EYE_BLINK_THRESHOLD) {
        return true; // Eye blink detected
    }
    return false; // No blink detected
}
