// #ifndef GYROSCOPE_H
// #define GYROSCOPE_H
// #include "pico/stdlib.h"




// #include <stdint.h>
// #include <math.h>
// #include <stdbool.h>

// // Default I2C address of the gyroscope
// #define GYROSCOPE_ADDR 0x6C

// #ifndef SDA_PIN 
// #define SDA_PIN 8
// #endif

// #ifndef SCL_PIN
// #define SCL_PIN 9
// #endif


// // Register addresses
// #define WHO_AM_I_REG 0x0F
// #define CTRL1_XL     0x10
// #define CTRL2_G      0x11
// #define OUTX_L_G     0x22

// // Sensitivity constants for gyroscope (mdps/LSB)
// #define GYRO_SENSITIVITY_2000 70.0

// // Function prototypes
// void gyroscope_i2c_init(uint8_t sda_pin, uint8_t scl_pin, uint32_t baudrate);
// bool gyroscope_init();
// bool gyroscope_who_am_i(uint8_t *id);
// void gyroscope_configure(uint8_t settings);
// bool gyroscope_read_data(float *x, float *y, float *z);
// bool gyroscope_isUp(float *x, float *y, float *z);
// bool gyroscope_isLeft(float *x, float *y, float *z);
// bool gyroscope_isDown(float *x, float *y, float *z);
// bool gyroscope_isRight(float *x, float *y, float *z);


// #endif


#ifndef LSM6DSL_H
#define LSM6DSL_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdbool.h>
#include <math.h>

// Device Address
#define LSM6DSL_ADDR 0x6A

// Register Addresses
#define WHO_AM_I        0x0F    // Device identification register
#define CTRL1_XL        0x10    // Accelerometer control register
#define CTRL2_G         0x11    // Gyroscope control register

// Gyroscope Output Registers
#define OUTX_L_G        0x22    // Gyroscope X-axis low byte
#define OUTX_H_G        0x23    // Gyroscope X-axis high byte
#define OUTY_L_G        0x24    // Gyroscope Y-axis low byte
#define OUTY_H_G        0x25    // Gyroscope Y-axis high byte
#define OUTZ_L_G        0x26    // Gyroscope Z-axis low byte
#define OUTZ_H_G        0x27    // Gyroscope Z-axis high byte

// Configuration Options
#define LSM6DSL_XL_ODR_104HZ    0x40    // Accelerometer 104 Hz
#define LSM6DSL_G_ODR_104HZ     0x40    // Gyroscope 104 Hz
#define LSM6DSL_G_FS_250DPS     0x00    // Gyroscope ±250 dps full scale

// LSM6DSL Configuration Structure
typedef struct {
    i2c_inst_t* i2c_port;       // I2C port (typically i2c0 or i2c1)
    uint8_t device_address;      // I2C device address
} LSM6DSL_Config;

typedef enum { STATE_CENTER, STATE_LEFT, STATE_RIGHT , STATE_INIT,STATE_ERROR } OrientationState;

// Function Prototypes
bool lsm6dsl_init(LSM6DSL_Config* config);
bool lsm6dsl_read_gyro(LSM6DSL_Config* config, int16_t* x, int16_t* y, int16_t* z);
void lsm6dsl_convert_gyro_to_dps(int16_t raw, float* dps);

// Additional optional configuration functions (declarations)
bool lsm6dsl_set_gyro_full_scale(LSM6DSL_Config* config, uint8_t full_scale);
bool lsm6dsl_set_gyro_output_rate(LSM6DSL_Config* config, uint8_t output_rate);

bool gyroscope_isUp1(float *x, float *y, float *z);
bool gyroscope_isLeft1(float *x, float *y, float *z);
bool gyroscope_isDown1(float *x, float *y, float *z);
bool gyroscope_isRight1(float *x, float *y, float *z);

// Function Prototypes
void calibrateGyroscope(LSM6DSL_Config* config);
OrientationState processGyroscopeData(LSM6DSL_Config* config);


#endif // LSM6DSL_H
