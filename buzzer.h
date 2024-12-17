#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"

// GPIO pins for the buzzer (change these as needed)
#define GPIO_PIN_BUZZER 4

// Function to initialize the buzzer
void buzzer_init(void);

// Function to turn the buzzer on
void buzzer_on(void);

// Function to turn the buzzer off
void buzzer_off(void);

// Function to make a sound (for passive buzzers)
void buzzer_sound(uint32_t frequency, uint32_t duration_ms);

#endif // BUZZER_H
