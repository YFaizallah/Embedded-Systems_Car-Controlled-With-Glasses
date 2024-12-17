// #include "pico/stdlib.h"
// #include "wifi_module.h"
// #include "hardware/uart.h"
// #include "ir_sensor.h"
// #include "gyroscope.h"
// #include "eye.h"
// #include  "hardware/i2c.h"

// #include <stdio.h>



// #define I2C_SDA 12
// #define I2C_SCL 13
// #define I2C_BAUD 400000

// void setup_i2c() {
//     // Initialize I2C
//     i2c_init(i2c0, I2C_BAUD);
    
//     // Set up GPIO for I2C
//     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
//     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
//     gpio_pull_up(I2C_SDA);
//     gpio_pull_up(I2C_SCL);
// }



// void send_http_request(const char* path) {
//     send_at_command("AT+CIPCLOSE", "OK");
//     sleep_ms(1000);

//     printf("\nAttempting TCP connection...\n");
//     char connect_cmd[100];
//     snprintf(connect_cmd, sizeof(connect_cmd), "AT+CIPSTART=\"TCP\",\"%s\",80", "192.168.0.122");
//     uart_puts(UART_ID, connect_cmd);
//     uart_puts(UART_ID, "\r\n");

//     if (!wait_for_response("CONNECT", 5000)) {
//         printf("Failed to establish TCP connection\n");
//         return;
//     }

//     char http_request[256];
//     int len = snprintf(http_request, sizeof(http_request),
//         "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
//         path, "192.168.0.122");

//     char send_cmd[32];
//     snprintf(send_cmd, sizeof(send_cmd), "AT+CIPSEND=%d", len);
//     send_at_command(send_cmd, ">");
//     sleep_ms(100);

//     printf("Sending HTTP request...\n%s\n", http_request);
//     uart_puts(UART_ID, http_request);

//     if (!wait_for_response("SEND OK", 5000)) {
//         printf("Failed to send HTTP request\n");
//     }

//     wait_for_response("CLOSED", 5000);
// }

// int main() {
//     stdio_init_all();
//     sleep_ms(5000);

//         // Initialize I2C for gyroscope
//     setup_i2c();
//     sleep_ms(500);
    
//     // Initialize gyroscope
//     LSM6DSL_Config gyro_config = {
//         .i2c_port = i2c0,
//         .device_address = LSM6DSL_ADDR
//     };
    
//     if (!lsm6dsl_init(&gyro_config)) {
//         printf("Failed to initialize gyroscope\n");
//         return -1;
//     }
    
//     calibrateGyroscope(&gyro_config);
//     printf("Gyroscope initialized\n");

//     init_uart();
//     eye_init();
//     ir_sensor_init();
//     init_esp_reset();

//     printf("Starting WiFi connection...\n");
//     int retry_count = 0;
//     while (!connect_to_wifi() && retry_count < 3) {
//         printf("Retrying WiFi connection (%d/3)...\n", retry_count + 1);
//         retry_count++;
//         sleep_ms(5000);
//     }

//     if (retry_count >= 3) {
//         printf("Failed to connect to WiFi\n");
//         return -1;
//     }
//     // bool isBlink = false;
//     bool isIR = false;
//     while(1){
//         u_int16_t sensor_value = eye_read_sensor();
//         printf("WiFi connected. Sending HTTP request to set LED color to red.\n");
//         OrientationState state = processGyroscopeData(&gyro_config);
//         if(eye_detect_blink(sensor_value)){
//             printf("in first if");
//             send_http_request("/led.cgi?motor=stop");
//             // isBlink = true;
//         }
//         else{   
//             if(ir_sensor_detected() && !isIR){
//                 printf("in first else");
//                  if(ir_sensor_detected()) isIR = true;
//                 switch(state) {
//                 case STATE_LEFT:
//                     printf("Left turn detected\n");
//                     send_http_request("/led.cgi?motor=left");
//                     break;
                    
//                 case STATE_RIGHT:
//                     printf("Right turn detected\n");
//                     send_http_request("/led.cgi?motor=right");
//                     break;
                    
//                 case STATE_CENTER:
//                     printf("Center position\n");
//                     send_http_request("/led.cgi?motor=forward");
//                     break;
                    
//                 case STATE_ERROR:
//                     printf("Gyroscope error\n");
//                     send_http_request("/led.cgi?motor=stop");
//                     break;
//             }
//                if(ir_sensor_detected()) isIR = true;
//             }
//             else{
//                 send_http_request("/led.cgi?motor=backward");
//                 if(ir_sensor_detected()) isIR = false;
//             }
            
//         //    if(eye_detect_blink(sensor_value)) isBlink = false;
//         }
//         // if(ir_sensor_detected()){
//         //     send_http_request("/led.cgi?rgbled=green");
//         // }
//         // send_http_request("/led.cgi?rgbled=blue");
//         // sleep_ms(1000);
//         // // send_http_request("/led.cgi?rgbled=red");
//         // // sleep_ms(1000);
//         // // send_http_request("/led.cgi?buzzer=on");
//         // // sleep_ms(1000);
//         // // send_http_request("/led.cgi?buzzer=off");
//         // // sleep_ms(1000);
//         // send_http_request("/led.cgi?motor=forward");
//         // sleep_ms(1000);

//     }

//     return 0;
// }


#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include <stdio.h>
#include "motor_driver.h"  // Add this with other includes
#include "led.h"
#include "buzzer.h"
#include "ultrasonic.h"

// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "Youssef Home ET";
const char WIFI_PASSWORD[] = "Qatar2024";

int main() {
    stdio_init_all();
    sleep_ms(10000);
    printf("Standard I/O initialized.\n");
    if (cyw43_arch_init()) {
        printf("Failed to initialize Wi-Fi module.\n");
        return -1;
    }
    printf("Wi-Fi module initialized.\n");
    cyw43_arch_enable_sta_mode();
    printf("Wi-Fi station mode enabled.\n");

    // Connect to the WiFi network - loop until connected
    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("Failed to connect to Wi-Fi. Retrying...\n");
        sleep_ms(1000);
    }
    printf("Connected to Wi-Fi.\n");

    // Get and print the IP address
    const ip4_addr_t *ip = netif_ip4_addr(netif_list);
    printf("IP Address: %s\n", ip4addr_ntoa(ip));

    // Initialise web server
    httpd_init();
    printf("Http server initialized\n");

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialized\n");
    cgi_init();
    printf("CGI Handler initialized\n");
    
    // Main loop
    while (true) {
        float distance = ultrasonic_get_distance();
        
        if(is_moving_forward) {
            if(distance < 10) {
                // Only stop if obstacle detected while moving forward
                stop_car();
                set_rgb_color(0, 0, 255);
                buzzer_sound(25000, 500);
                is_moving_forward = false;
            } else if(is_moving_left) {
                // Keep moving left if no obstacle
                move_left(250);
                set_rgb_color(0, 255, 0);
            } else if(is_moving_right) {
                // Keep moving right if no obstacle
                move_right(250);
                set_rgb_color(0, 255, 0);
            } else {
                // Keep moving forward if no obstacle
                move_forward(250);
                set_rgb_color(0, 255, 0);
            }
        }
        sleep_ms(100);
    }
    return 0;
}
