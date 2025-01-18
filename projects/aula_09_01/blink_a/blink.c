#include "pico/stdlib.h"

#define led_pin_red 13

int main() {
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);
 
     while (true) {
        gpio_put(led_pin_red, true);
        sleep_ms(1000);
        gpio_put(led_pin_red, false);
        sleep_ms(1000);
    }
}
