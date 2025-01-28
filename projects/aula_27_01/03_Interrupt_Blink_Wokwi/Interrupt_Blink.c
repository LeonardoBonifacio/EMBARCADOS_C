/*
Por: Wilton Lacerda Silva
                    Interrupções.
  Uso de um botão e de um LED RGB da placa BitDogLab para
  demonstração do funcionamento das interrupções. 

  O código faz o LED verde piscar a cada "tempo" ms.
  Quando o botão A é pressionado, o LED azul alterna de estado.

  O código foi testado na placa Raspberry Pi Pico e no simulador Wokwi.
  
*/

#include "pico/stdlib.h"

// Configurações dos pinos 
const uint ledA_pin = 12;    // Blue => GPIO12
const uint ledB_pin = 11;    // Green => GPIO11
const uint button_0 = 5;     // Botão A = 5, Botão B = 6 , BotãoJoy = 22
#define tempo 2500

// Prototipo da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);
int main()
{
    //                      Inicializações
    gpio_init(ledA_pin);                 // Inicializa o pino do LED
    gpio_set_dir(ledA_pin, GPIO_OUT);    // Configura o pino como saída
    gpio_init(ledB_pin);                 // Inicializa o pino do LED
    gpio_set_dir(ledB_pin, GPIO_OUT);    // Configura o pino como saída

    gpio_init(button_0);                    // Inicializa o botão
    gpio_set_dir(button_0, GPIO_IN);        // Configura o pino como entrada
    gpio_pull_up(button_0);                 // Habilita o pull-up interno
    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    // Loop principal
    while (true)
    {
        gpio_put(ledB_pin, true);
        sleep_ms(tempo);
        gpio_put(ledB_pin, false);
        sleep_ms(tempo);
    }
}
// Função de interrupção "É possivel observar o bounce do botão"
void gpio_irq_handler(uint gpio, uint32_t events)
{
    bool estado_atual = gpio_get(ledA_pin); // Obtém o estado atual
    gpio_put(ledA_pin, !estado_atual);      // Alterna o estado
}
