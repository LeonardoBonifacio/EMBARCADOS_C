/*
  Por: Wilton Lacerda Silva

                        Debouncing.
  Tentativa de correção do Efeito Bouncing em botões.

                    Interrupções.
  Uso de um botão e de um LED RGB da placa BitDogLab para
  demonstração do funcionamento das interrupções.

  O código a seguir faz com que o Led azul seja modificado 
  a cada pressionada do botão. Já o LED verde pisca a cada tempo ms.
  Uma variável de verificação é incrementada a cada interrupção.
  Esta variável é exibida no serial monitor para verificar o Bounce.

  O código foi testado na placa Raspberry Pi Pico e no simulador Wokwi.

*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// Configurações dos pinos
const uint ledA_pin = 12; // Blue => GPIO12
const uint ledB_pin = 11; // Green=> GPIO11
const uint button_0 = 5; // Botão A = 5, Botão B = 6 , BotãoJoy = 22
#define tempo 2500

// Variáveis globais
static volatile uint a = 1;
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

// Prototipação da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

int main()
{
    // Inicializações
    stdio_init_all();
    // Inicializa o LED
    gpio_init(ledA_pin);              // Inicializa o pino do LED
    gpio_set_dir(ledA_pin, GPIO_OUT); // Configura o pino como saída
    gpio_init(ledB_pin);              // Inicializa o pino do LED
    gpio_set_dir(ledB_pin, GPIO_OUT); // Configura o pino como saída
                                      // Inicializa o botão
    gpio_init(button_0);
    gpio_set_dir(button_0, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(button_0);          // Habilita o pull-up interno

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

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    printf("A = %d\n", a);
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000) // 200 ms de debouncing
    {
        last_time = current_time; // Atualiza o tempo do último evento
        printf("Mudanca de Estado do Led. A = %d\n", a);
        gpio_put(ledA_pin, !gpio_get(ledA_pin)); // Alterna o estado
        a++;                                     // incrementa a variavel de verificação
    }
}
