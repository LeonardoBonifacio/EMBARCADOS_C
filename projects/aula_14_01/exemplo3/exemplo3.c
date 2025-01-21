#include <stdio.h>
#include "pico/stdlib.h"

const uint led_pin_red = 12;
const uint button_0 = 5;
static volatile uint a = 1;

//rotina da interrupção
static void gpio_irq_handler(uint gpio, uint32_t events){
    a++;
    printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
    if(a%2 == 0)
        gpio_put(led_pin_red, true);
    else
        gpio_put(led_pin_red, false);
}

int main(){

    stdio_init_all();

    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);

    gpio_init(button_0);
    gpio_set_dir(button_0, GPIO_IN);
    gpio_pull_up(button_0);

    //interrupção da gpio habilitada
    gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, 1, & gpio_irq_handler);

    while (true)
    {
        printf("Teste de interrupção\r\n");
        sleep_ms(10000);
    }
}