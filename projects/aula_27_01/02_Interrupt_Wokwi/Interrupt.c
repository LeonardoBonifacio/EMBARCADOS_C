/*
Por: Wilton Lacerda Silva
                    Interrupções.
  Uso de um botão e de um LED RGB da placa BitDogLab para
  demonstração do funcionamento das interrupções. 

  O código a seguir faz com que o LED Blue, seja modificado 
  a cada pressionada do botão.

  O código foi testado na placa Raspberry Pi Pico.

  Para mais informações sobre o código Wokwi acesse:
         https://wokwi.com/projects/

*/

#include "pico/stdlib.h"

// Configurações dos pinos 
const uint led_pin = 12;    //Red=13, Blue=12, Green=11
const uint botao_pin = 5;        // Botão A = 5, Botão B = 6 , BotãoJoy = 22
// Prototipo da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);
int main()
{
    //                      Inicializações
    gpio_init(led_pin);                 // Inicializa o pino do LED
    gpio_set_dir(led_pin, GPIO_OUT);    // Configura o pino como saída
    gpio_init(botao_pin);                    // Inicializa o botão
    gpio_set_dir(botao_pin, GPIO_IN);        // Configura o pino como entrada
    gpio_pull_up(botao_pin);                 // Habilita o pull-up interno
    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(botao_pin, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    // Loop principal
    while (true)
    {
        // Não tem programa no loop.
    }
}
// Função de interrupção "É possivel observar o bounce do botão"
void gpio_irq_handler(uint gpio, uint32_t events)
{
    bool estado_atual = gpio_get(led_pin); // Obtém o estado atual
    gpio_put(led_pin, !estado_atual);      // Alterna o estado
}
