



#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "gyroscope.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

// LSM6DSL Register Addresses
#define LSM6DSL_ADDR 0x6A
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define WHO_AM_I 0x0F
#define OUTX_L_G 0x22
#define OUTX_H_G 0x23
#define OUTY_L_G 0x24
#define OUTY_H_G 0x25
#define OUTZ_L_G 0x26
#define OUTZ_H_G 0x27

#define EPSILON 0.05f

// LSM6DSL Configuration Structure
// typedef struct {
//     i2c_inst_t* i2c_port;
//     uint8_t device_address;
// } LSM6DSL_Config;

// typedef enum { STATE_CENTER, STATE_LEFT, STATE_RIGHT , STATE_INIT,STATE_ERROR } OrientationState;


// Read a single register
static uint8_t lsm6dsl_read_register(LSM6DSL_Config* config, uint8_t reg) {
    uint8_t data;
    if (i2c_write_blocking(config->i2c_port, config->device_address, &reg, 1, true) != 1) {
        return 0xFF; // Error
    }
    if (i2c_read_blocking(config->i2c_port, config->device_address, &data, 1, false) != 1) {
        return 0xFF; // Error
    }
    return data;
}

// Write to a single register
static bool lsm6dsl_write_register(LSM6DSL_Config* config, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    return i2c_write_blocking(config->i2c_port, config->device_address, data, 2, false) == 2;
}

bool lsm6dsl_init(LSM6DSL_Config* config) {
    // Verify WHO_AM_I register
    printf("Reading WHO_AM_I register...\n");
    uint8_t who_am_i = lsm6dsl_read_register(config, WHO_AM_I);
    printf("WHO_AM_I value: 0x%02X\n", who_am_i);

    if (who_am_i != 0x6C) { // Expected value for LSM6DSL
        printf("Error: LSM6DSL not detected.\n");
        return false;
    }

    printf("WHO_AM_I verified. Configuring sensor...\n");

    // Configure accelerometer
    if (!lsm6dsl_write_register(config, CTRL1_XL, 0x40)) {
        printf("Failed to configure CTRL1_XL.\n");
        return false;
    }

    // Configure gyroscope
    if (!lsm6dsl_write_register(config, CTRL2_G, 0x40)) {
        printf("Failed to configure CTRL2_G.\n");
        return false;
    }

    printf("LSM6DSL initialized successfully.\n");
    return true;
}

// Read raw gyroscope data
bool lsm6dsl_read_gyro(LSM6DSL_Config* config, int16_t* x, int16_t* y, int16_t* z) {
    uint8_t xl = lsm6dsl_read_register(config, OUTX_L_G);
    uint8_t xh = lsm6dsl_read_register(config, OUTX_H_G);
    uint8_t yl = lsm6dsl_read_register(config, OUTY_L_G);
    uint8_t yh = lsm6dsl_read_register(config, OUTY_H_G);
    uint8_t zl = lsm6dsl_read_register(config, OUTZ_L_G);
    uint8_t zh = lsm6dsl_read_register(config, OUTZ_H_G);

    *x = (int16_t)((xh << 8) | xl);
    *y = (int16_t)((yh << 8) | yl);
    *z = (int16_t)((zh << 8) | zl);

    return true;
}

// Convert raw values to degrees per second
void lsm6dsl_convert_gyro_to_dps(int16_t raw, float* dps) {
    // For ±250 dps configuration
    *dps = raw * 0.00875f; // Sensitivity: 8.75 mdps/LSB
}

// Check if the gyroscope is oriented "up"
bool gyroscope_isUp1(float *x, float *y, float *z) {
    if (fabs(*x) < EPSILON && fabs(*y) < EPSILON && *z > 0) {
        return true; // "Up" is when Z is positive (aligned with gravity)
    }
    return false;
}

// Check if the gyroscope is oriented "down"
bool gyroscope_isDown1(float *x, float *y, float *z) {
    if (fabs(*x) < EPSILON && fabs(*y) < EPSILON && *z < 0) {
        return true; // "Down" is when Z is negative (opposite direction of gravity)
    }
    return false;
}

// Check if the gyroscope is oriented "left"
bool gyroscope_isLeft1(float *x, float *y, float *z) {
    if (fabs(*z) < EPSILON && *x < 0 && fabs(*y) < EPSILON) {
        return true; // "Left" is when X is negative and Z and Y are near zero
    }
    return false;
}

// Check if the gyroscope is oriented "right"
bool gyroscope_isRight1(float *x, float *y, float *z) {
    if (fabs(*z) < EPSILON && *x > 0 && fabs(*y) < EPSILON) {
        return true; // "Right" is when X is positive and Z and Y are near zero
    }
    return false;
}

// Global offset variable for Z-axis
static float gyroZ_offset = 0;

// Calibration function
void calibrateGyroscope(LSM6DSL_Config* config) {
    printf("Calibrating gyroscope...\n");
    float sumZ = 0;
    int16_t rawX, rawY, rawZ;

    int samples = 100; // Number of samples for calibration
    for (int i = 0; i < samples; i++) {
        if (lsm6dsl_read_gyro(config, &rawX, &rawY, &rawZ)) {
            float dpsZ;
            lsm6dsl_convert_gyro_to_dps(rawZ, &dpsZ);
            sumZ += dpsZ;
        }
        sleep_ms(10); // Delay between readings
    }

    gyroZ_offset = sumZ / samples; // Compute average offset
    printf("Calibration complete. Gyro Z-offset: %.2f dps\n", gyroZ_offset);
}

// Process gyroscope data and apply calibration and dead zone
OrientationState processGyroscopeData(LSM6DSL_Config* config) {
    int16_t rawX, rawY, rawZ;
    float dpsX, dpsY, dpsZ;
    static OrientationState currentState = STATE_CENTER; // Persistent state across function calls

    // Read raw data
    if (lsm6dsl_read_gyro(config, &rawX, &rawY, &rawZ)) {
        lsm6dsl_convert_gyro_to_dps(rawX, &dpsX); // up and down i think
        lsm6dsl_convert_gyro_to_dps(rawY, &dpsY); //  if the nano is on the left side of the glasses
        lsm6dsl_convert_gyro_to_dps(rawZ, &dpsZ); // if the nano is on the front of the glasses

        // Apply calibration
        dpsX -= gyroZ_offset;

        // Apply dead zone
        const float DEAD_ZONE = 0.1; // Threshold for small movements make is 0.1 for y rotations
        if (fabs(dpsX) < DEAD_ZONE) dpsX = 0;

        // Define thresholds
        const float TURN_THRESHOLD = 15.0f; // Threshold for entering turn state
        // const float CENTER_THRESHOLD = 5.0f;  // Threshold for returning to center

        // State transition logic
        switch (currentState) {
            case STATE_CENTER:
                if (dpsX < -TURN_THRESHOLD) {
                    currentState = STATE_RIGHT;
                    printf("Action: Go Right\n");
                } else if (dpsX > TURN_THRESHOLD) {
                    currentState = STATE_LEFT;
                    printf("Action: Go Left\n");
                }
                break;

            case STATE_RIGHT:
                if (dpsX > TURN_THRESHOLD) {
                    currentState = STATE_CENTER;
                    printf("Action: Center\n");
                }
                break;

            case STATE_LEFT:
                if (dpsX < -TURN_THRESHOLD) {
                    currentState = STATE_CENTER;
                    printf("Action: Center\n");
                }
                break;

            default:
                printf("Unexpected state. Resetting to center.\n");
                currentState = STATE_CENTER;
                break;
        }

    } else {
        currentState = STATE_ERROR;
        printf("Failed to read gyroscope data.\n");
    }

    printf("Returned State: %d\n", currentState);
    return currentState;
}



