#include "wifi_module.h"
#include "hardware/uart.h"
#include <string.h>
#include <stdio.h>



void init_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

void init_esp_reset() {
    gpio_init(ESP_RST_PIN);
    gpio_set_dir(ESP_RST_PIN, GPIO_OUT);
    gpio_put(ESP_RST_PIN, 1);  // Keep ESP-01S in run mode
}

void hardware_reset_esp() {
    gpio_put(ESP_RST_PIN, 0);
    sleep_ms(100);
    gpio_put(ESP_RST_PIN, 1);
    sleep_ms(5000);  // Wait for stable boot
}

void flush_uart_input() {
    while (uart_is_readable(UART_ID)) {
        uart_getc(UART_ID);
    }
}

bool wait_for_response(const char* expected, uint32_t timeout_ms) {
    char buffer[512] = {0};
    uint32_t start = to_ms_since_boot(get_absolute_time());
    size_t pos = 0;
    
    while (to_ms_since_boot(get_absolute_time()) - start < timeout_ms) {
        if (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            putchar(c);  // Debug output
            
            // Add character to buffer, with overflow protection
            if (pos < sizeof(buffer) - 1) {
                buffer[pos++] = c;
                buffer[pos] = '\0';
            }
            
            // Check for complete response
            if (strstr(buffer, expected) != NULL) {
                printf("\nFound response: %s\n", expected);
                return true;
            }
        }
        sleep_ms(1);
    }
    
    printf("\nTimeout waiting for: %s\n", expected);
    printf("Final buffer: %s\n", buffer);
    return false;
}

void send_at_command(const char* command, const char* expected_response) {
    flush_uart_input();
    printf("\nSending: %s\n", command);
    uart_puts(UART_ID, command);
    uart_puts(UART_ID, "\r\n");  // Proper command termination
    
    if (!wait_for_response(expected_response, 5000)) {
        printf("Failed to get expected response for: %s\n", command);
    }
}

bool connect_to_wifi() {
    printf("Initializing ESP-01S...\n");
    hardware_reset_esp();
    sleep_ms(3000);
    
    // Test AT with retries
    for (int i = 0; i < 3; i++) {
        send_at_command("AT", "OK");
        sleep_ms(500);
    }
    
    // Set station mode
    send_at_command("AT+CWMODE=1", "OK");
    sleep_ms(1000);
    
    // Connect to Wi-Fi
    printf("Attempting WiFi connection...\n");
    char wifi_cmd[100];
    snprintf(wifi_cmd, sizeof(wifi_cmd), "AT+CWJAP=\"%s\",\"%s\"", WIFI_SSID, WIFI_PASSWORD);
    uart_puts(UART_ID, wifi_cmd);
    uart_puts(UART_ID, "\r\n");
    
    // Wait for connection with separate checks
    uint32_t start = to_ms_since_boot(get_absolute_time());
    bool got_ip = false;
    
    while (to_ms_since_boot(get_absolute_time()) - start < 30000) {
        if (wait_for_response("WIFI GOT IP", 1000)) {
            got_ip = true;
            break;
        }
    }
    
    return got_ip;
}
