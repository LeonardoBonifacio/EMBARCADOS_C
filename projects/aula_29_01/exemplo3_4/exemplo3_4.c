#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"


#define LED_PIN 12
#define BUTTON_PIN 5

bool led_on = false;
absolute_time_t turn_off_time;
bool led_active = false;

bool repeating_timer_callback(struct repeating_timer *t){
    if (led_active && absolute_time_diff_us(get_absolute_time(), turn_off_time) <= 0)
    {
        led_on = false;
        gpio_put(LED_PIN,false);
        led_active = false;
    }
    
    return true;
}


int main()
{
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(LED_PIN,GPIO_OUT);
    gpio_set_dir(BUTTON_PIN,GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    struct repeating_timer timer;

    add_repeating_timer_ms(1000,repeating_timer_callback,NULL,&timer);
    while (true) {
        if (gpio_get(BUTTON_PIN) == 0 && !led_active){
            sleep_ms(50);
            if (gpio_get(BUTTON_PIN) == 0){
                led_on = true;
                gpio_put(LED_PIN,true);
                led_active = true;
                // Definir o tempo para desligar o led apos 2 segundos
                turn_off_time = make_timeout_time_ms(2000);
            }
            
        }
        
    }
}
