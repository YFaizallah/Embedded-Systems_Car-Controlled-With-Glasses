// cgi.h
#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"
#include "motor_driver.h"
#include "buzzer.h"
#include "led.h"

#include "ultrasonic.h"

static bool is_moving_forward = false;
static bool is_moving_left = false;
static bool is_moving_right = false;
static bool is_moving_backward = false;


// CGI handler for LED and motor control
const char * cgi_led_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    for(int i = 0; i < iNumParams; i++) {
        if (strcmp(pcParam[i], "led") == 0) {
            // LED control
            if(strcmp(pcValue[i], "0") == 0)
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
            else if(strcmp(pcValue[i], "1") == 0)
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        }
        else if (strcmp(pcParam[i], "motor") == 0) {
            uint16_t speed = 250;
            is_moving_forward = true;
            if(strcmp(pcValue[i], "forward") == 0) {
                if(ultrasonic_get_distance() < 10) {
                    // Obstacle detected - don't move
                    is_moving_forward = false;
                    is_moving_left = false;
                    is_moving_right = false;
                    is_moving_backward = false;
                    stop_car();
                    set_rgb_color(0, 0, 255);  // Red warning
                    buzzer_sound(25000, 500);
                } else {
                    is_moving_forward = true;
                    is_moving_left = false;
                    is_moving_right = false;
                    is_moving_backward = false;
                    move_forward(speed);
                    set_rgb_color(0, 255, 0);
                    buzzer_off();
                }
            }
            else if(strcmp(pcValue[i], "backward") == 0) {
                is_moving_forward = false;
                is_moving_left = false;
                is_moving_right = false;
                is_moving_backward = true;
                set_rgb_color(255, 0, 0);
                move_backward(speed);
                buzzer_sound(10000, 1000);
                

            }
            else if(strcmp(pcValue[i], "left") == 0) {
                is_moving_forward = true;
                is_moving_left = true;
                is_moving_right = false;
                is_moving_backward = false;
                set_rgb_color(0, 255, 0);
                move_left(speed);
                buzzer_off();
                
            }
            else if(strcmp(pcValue[i], "right") == 0) {
                is_moving_forward = true;
                is_moving_right = true;
                is_moving_left = false;
                is_moving_backward = false;
                move_right(speed);
                buzzer_off();
                set_rgb_color(0, 255, 0);
            }
            else if(strcmp(pcValue[i], "stop") == 0) {
                is_moving_forward = false;
                is_moving_left = false;
                is_moving_right = false;
                is_moving_backward = false;
                set_rgb_color(0, 0, 255);
                stop_car();
                
            }
        }
        else if (strcmp(pcParam[i], "buzzer") == 0) {
            if(strcmp(pcValue[i], "on") == 0) {
                buzzer_on();
            }
            else if(strcmp(pcValue[i], "off") == 0) {
                buzzer_off();
            }
        }
        else if (strcmp(pcParam[i], "rgbled") == 0) {
            if(strcmp(pcValue[i], "red") == 0) {
                set_rgb_color(0, 0, 255);
            }
            else if(strcmp(pcValue[i], "green") == 0) {
                set_rgb_color(0, 255, 0);
            }
            else if(strcmp(pcValue[i], "blue") == 0) {
                set_rgb_color(255, 0, 0);
            }
            else if(strcmp(pcValue[i], "off") == 0) {
                set_rgb_color(0, 0, 0);
            }
        }
    }
    return "/index.shtml";
}

// tCGI Struct
static const tCGI cgi_handlers[] = {
    {"/led.cgi", cgi_led_handler},
};


void cgi_init(void)
{
    motor_init();
    buzzer_init();
    rgb_led_init();
    ultrasonic_init();
    http_set_cgi_handlers(cgi_handlers, 1);
}