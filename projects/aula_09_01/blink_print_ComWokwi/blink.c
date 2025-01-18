#include <stdio.h>
#include "pico/stdlib.h"

const uint led_pin_red = 13;
#define tempo 500
uint a = 0;

int main()
{

  gpio_init(led_pin_red);
  gpio_set_dir(led_pin_red, GPIO_OUT);
  stdio_init_all();

  while (true)
  {
    if (a % 2)
      printf("Blinking! %d\r\n", a);
    a++;
    gpio_put(led_pin_red, true);
    printf("Led Ligado..  ");
    sleep_ms(tempo);
    gpio_put(led_pin_red, false);
    printf("Led desligado..\r\n");
    sleep_ms(tempo);
  }
}
