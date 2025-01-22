#include <stdio.h>
#include "pico/stdlib.h"


// Definição dos pinos dos segmentos do display
                            //  a  b  c  d  e  f  g
const uint8_t segment_pins[] = {0, 1, 2, 3, 4, 5, 6};


// Mapeamento dos digitos para os segmentos do display
const uint8_t digits [10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 0, 0, 1, 1}, // 9
};


// Função para exibir um dígito no display
void display_digit(uint8_t digit){
    for (int i = 0; i < 7; i++)
    {
        gpio_put(segment_pins[i], digits[digit][i]);
    }
    
}

int main()
{
    stdio_init_all();

    // Inicializa e configura os pinos dos segmentos como sáida
    for (int i = 0; i < 7; i++)
    {
        gpio_init(segment_pins[i]);
        gpio_set_dir(segment_pins[i], GPIO_OUT);
    }

    uint8_t contador = 0;
    while (1)
    {
        display_digit(contador); // Exibe o dígito atual no display
        sleep_ms(1000); // Espera 1 segundo
        contador++; // Incrementa o contador
        if (contador > 9) {
            contador = 0; // Reinicia o contador após 9
        }
        
    }
    
    
}
