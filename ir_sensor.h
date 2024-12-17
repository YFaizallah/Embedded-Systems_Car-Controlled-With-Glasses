#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include "pico/stdlib.h"

// Define the GPIO pin for the IR sensor
#define IR_SENSOR_PIN 15

// Function prototypes
void ir_sensor_init(void);           // Initialize the IR sensor GPIO pin
bool ir_sensor_detected(void);       // Check if the IR sensor detects an object

#endif // IR_SENSOR_H
