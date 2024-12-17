#ifndef WIFI_MODULE_H
#define WIFI_MODULE_H

#include "pico/stdlib.h"
#include <stdbool.h>

// Wi-Fi configuration
#define WIFI_SSID "TP-Link_696E"
#define WIFI_PASSWORD "28923412"

#define UART_ID uart0
#define BAUD_RATE 115200
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define ESP_RST_PIN 16

// Function declarations
void init_uart(void);
void init_esp_reset(void);
void hardware_reset_esp(void);
void flush_uart_input(void);
bool wait_for_response(const char* expected, uint32_t timeout_ms);
void send_at_command(const char* command, const char* expected_response);
bool connect_to_wifi(void);

#endif // WIFI_MODULE_H
