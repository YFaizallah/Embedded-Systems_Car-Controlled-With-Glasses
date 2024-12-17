#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>

// Pin definitions
#define TRIG_PIN 15
#define ECHO_PIN 14

// Initialize the ultrasonic sensor (TRIG and ECHO pins)
void ultrasonic_init(void);

// Trigger the ultrasonic sensor to start a measurement
void ultrasonic_trigger(void);

// Measure the distance in centimeters
float ultrasonic_get_distance(void);

#endif // ULTRASONIC_H
