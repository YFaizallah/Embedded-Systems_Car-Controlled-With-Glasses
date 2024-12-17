#ifndef EYE_H
#define EYE_H

#include <stdint.h>
#include <stdbool.h>

// Define the analog pin for the line track sensor
#define EYE_SENSOR_PIN 27 // You can change this to your desired pin

// Blink detection threshold
#define EYE_BLINK_THRESHOLD 4095

// Initialize the eye sensor
void eye_init();

// Read the analog value from the sensor
uint16_t eye_read_sensor();

// Check if an eye blink is detected
bool eye_detect_blink(uint16_t sensor_value);

#endif // EYE_H
