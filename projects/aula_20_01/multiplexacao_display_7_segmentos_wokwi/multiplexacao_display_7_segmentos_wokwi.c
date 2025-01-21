#include <stdio.h>
#include "pico/stdlib.h"

// Definição dos pinos dos segmentos do display e dos controles do display
                            
const uint8_t segment_pins[] = {5, 6, 4, 2, 3, 0, 1}; // Pinos dos segmetos a,b,c,d,e,f,g
const uint8_t display_pins[] = {11, 12}; // Pinos de controle dos displays (Dig1 e Dig2) 
uint8_t dezenas  = 0;
uint8_t unidades  = 0;


// Mapeamento dos digitos para os segmentos do display (0 = Desligado , 1 = ligado)
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


// Função para configurar todos os pinos como saída
void setup(){
    for (int i = 0; i < 7; i++)
    {
        gpio_init(segment_pins[i]);
        gpio_set_dir(segment_pins[i], GPIO_OUT);
    }
    for (int i = 0; i < 2; i++)
    {
        gpio_init(display_pins[i]);
        gpio_set_dir(display_pins[i], GPIO_OUT);
        gpio_put(display_pins[i], 1); // Desativa inicialmente os displays
    }
    

    
}

// Função para exibir um número em um display específico
void show_digit(uint8_t display, uint8_t digit){
    // Desativa ambos displays(Catodo comum é desativado com nivel alto)
    gpio_put(display_pins[0],1);
    gpio_put(display_pins[1],1);


    // Ativa os segmentos correspondentes ao dígito
    for (int  i = 0; i < 7; i++)
    {
        gpio_put(segment_pins[i], digits[digit][i]);
    }

    // Ativa o display desejado
    gpio_put(display_pins[display],0);
    
    
}

int main()
{
    stdio_init_all();
    setup(); // Configura todos os pinos como sáida

    while (true) {
        for (uint8_t numero = 0; numero < 100; numero++)// Loop para contagem de 0 a 99
        {
            dezenas = numero / 10; //  Calcula o dígito das dezenas
            unidades = numero % 10; // Calcula o dígito das unidades

            // Multiplexa rapidamente entre os displays
            for (int j = 0; j < 25; j++) {// Ajusta o valor de j para controlar a velocidade de atualização 
                show_digit(0,dezenas); // Exibe o dígito das dezenas no primeiro display
                sleep_ms(5); // Tempo de permanência no primeiro display
                show_digit(1, unidades); // Exibe o dígito das unidades no segundo display
                sleep_ms(5); // Tempo de permanência no segundo display
            }
        sleep_ms(500); // Pausa de meio segundo entre incrementos
        }
    }
    return 0;
}
