
#include <stdio.h>
#include "pico/stdlib.h"

const uint led_pin_red = 13;
#define tempo 50

int main() {
    uint a = 100;

    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);
    stdio_init_all();
 
     while (true) {
        if (a % 2)  
          printf("Blinking!\r\n");

        gpio_put(led_pin_red, true);
        printf("Led Ligado..  ");
        sleep_ms(tempo);
        gpio_put(led_pin_red, false);
        printf("Led desligado..\r\n");
        sleep_ms(tempo);
    }
}
