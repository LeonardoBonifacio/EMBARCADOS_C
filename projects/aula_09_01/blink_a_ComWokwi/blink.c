/*
Por: Wilton Lacerda Silva

  Uso do LED RGB da placa BitDogLab. 
  Também será utilizado o Wokwi para simular o código.
    O LED tricolor (RGB) é composto por 3 LEDs no mesmo encapsulamento:
                Vermelho(R), Verde(G) e o Azul(B).
    O código a seguir faz com que o um dos LEDs RGB, selecionado pela definição
    led_pin_red, pisque por um período dado na variável inteira "tempo".

    O código foi testado na placa Raspberry Pi Pico.
    Para mais informações sobre a placa BitDogLab acesse: 
        https://github.com/Fruett/BitDogLab
    Para mais informações sobre a placa Raspberry Pi Pico acesse: 
        https://www.raspberrypi.org/products/raspberry-pi-pico/
    Para mais informações sobre o código Wokwi acesse:
           https://wokwi.com/projects/418071361765683201

*/


#include "pico/stdlib.h"

#define led_pin_red 13
int tempo = 1000;

int main() {
    gpio_init(led_pin_red);
    gpio_set_dir(led_pin_red, GPIO_OUT);
 
     while (true) {
        gpio_put(led_pin_red, true);
        sleep_ms(tempo);
        gpio_put(led_pin_red, false);
        sleep_ms(tempo);
    }
}
