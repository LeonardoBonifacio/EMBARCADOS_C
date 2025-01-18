/*
Por: Wilton Lacerda Silva

  Uso do LED tricolor da placa BitDogLab. 
  Também será utilizado o Wokwi para simular o código.
  O LED tricolor (RGB) é composto por 3 LEDs no mesmo encapsulamento:
                Vermelho(R), Verde(G) e o Azul(B).
                
    O código a seguir faz com que o LED RGB pisque de forma sequencial:
        primeiro o verde, depois o azul e por último o vermelho.
   
    O tempo de cada piscada é de 900ms para o verde, 600ms para o azul e
    300ms para o vermelho.

    O código foi testado na placa Raspberry Pi Pico.
    Para mais informações sobre a placa BitDogLab acesse: 
        https://github.com/Fruett/BitDogLab
    Para mais informações sobre a placa Raspberry Pi Pico acesse: 
        https://www.raspberrypi.org/products/raspberry-pi-pico/
    Para mais informações sobre o código Wokwi acesse:
           https://wokwi.com/projects/418071766652353537

*/

#include <stdio.h>
#include "pico/stdlib.h"


const uint led_pin_green = 11;
const uint led_pin_blue  = 12;
const uint led_pin_red   = 13;


int main()
{
    gpio_init(led_pin_green);
    gpio_set_dir(led_pin_green, GPIO_OUT);
    gpio_init(led_pin_blue);
    gpio_set_dir(led_pin_blue, GPIO_OUT);
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);

    while (true) {
        gpio_put(led_pin_green, true);
        sleep_ms(900);
        gpio_put(led_pin_green, false);
        sleep_ms(900);
        gpio_put(led_pin_blue, true);
        sleep_ms(600);
        gpio_put(led_pin_blue, false);
        sleep_ms(600);
        gpio_put(led_pin_red, true);
        sleep_ms(300);
        gpio_put(led_pin_red, false);
        sleep_ms(300);
    }
}
