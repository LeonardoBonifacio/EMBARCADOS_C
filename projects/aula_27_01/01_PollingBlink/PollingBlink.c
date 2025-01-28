/*
Por: Wilton Lacerda Silva

  Uso de um botão e de dois LEDs RGB da placa BitDogLab para
  demonstração do Polling.

  O código faz o LED vermelho piscar a cada "tempo" ms.
  Quando o botão A é pressionado, o LED verde alterna de estado.

  O código foi testado na placa BitDogLab V5.3 e V6.3.

*/

#include "pico/stdlib.h"

// Configurações dos pinos
const uint led_pin = 11;       // Green => GPIO11
const uint led_pin_pisca = 13; // Red => GPIO13
const uint botao_pin = 5;      // Botão A = 5, Botão B = 6 , BotãoJoy = 22
#define tempo 1000

int main()
{
    //                      Inicializações
    gpio_init(led_pin);              // Inicializa o pino do LED
    gpio_set_dir(led_pin, GPIO_OUT); // Configura o pino como saída
    gpio_put(led_pin, 0);            // Garante que o LED inicie apagado

    gpio_init(led_pin_pisca);              // Inicializa o pino do LED para piscar
    gpio_set_dir(led_pin_pisca, GPIO_OUT); // Configura o pino como saída
    gpio_put(led_pin_pisca, 0);            // Garante que o LED inicie apagado

    gpio_init(botao_pin);             // Inicializa o botão
    gpio_set_dir(botao_pin, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(botao_pin);          // Habilita o pull-up interno
                                      // Variáveis para o controle do estado do LED
    bool led_estado = false;          // Estado inicial do LED (apagado)
    bool ultimo_estado_botao = true;  // Último estado do botão (inicialmente solto)

    // Loop principal
    while (true)
    {
        // Lógica para o LED piscar
        gpio_put(led_pin_pisca, 1); // Liga o LED
        sleep_ms(tempo);            // Mantém ligado por "tempo" ms
        gpio_put(led_pin_pisca, 0); // Desliga o LED
        sleep_ms(tempo);             // Mantém desligado por "tempo" ms

        // Lê o estado atual do botão
        bool estado_atual_botao = gpio_get(botao_pin);

        // Detecta uma transição do botão (pressionado e solto)
        if (estado_atual_botao == false && ultimo_estado_botao == true)
        {
            // Transição de solto para pressionado: alterna o estado do LED
            led_estado = !led_estado;
            gpio_put(led_pin, led_estado); // Atualiza o estado do LED
        }

        // Atualiza o estado anterior do botão
        ultimo_estado_botao = estado_atual_botao;
    }
}