#include "pico/stdlib.h"    // Biblioteca padrão para controle de GPIO, temporização e comunicação serial.
#include "hardware/timer.h" // Biblioteca para gerenciamento de temporizadores de hardware.
#include <stdio.h>          // Biblioteca padrão de entrada e saída para uso de funções como printf.

// Definição dos pinos para o LED e o botão
const uint LED_PIN = 13;   // Define o pino GPIO 13 para controlar o LED.
const uint BUTTON_PIN = 5; // Define o pino GPIO 5 para ler o estado do botão.

// Variáveis globais voláteis para sincronização com interrupções
volatile int button_press_count = 0; // Contador de pressões do botão.
volatile bool led_on = false;        // Armazena o estado atual do LED (ligado/desligado).
volatile bool led_active = false;    // Indica se o LED está ativo para evitar múltiplas ativações.

// Função de callback para desligar o LED após 4 segundos.
// Essa função é chamada pelo temporizador quando o alarme expira.
int64_t turn_off_callback(alarm_id_t id, void *user_data) {
    gpio_put(LED_PIN, false); // Desliga o LED.
    led_active = false;       // Atualiza a variável indicando que o LED não está mais ativo.
    printf("LED desligado\n"); // Exibe mensagem na saída serial indicando que o LED foi desligado.
    return 0; // Retorna 0 para não repetir o alarme.
}

// Função de callback do temporizador repetitivo que é chamada a cada 100 ms.
// Essa função verifica o estado do botão e gerencia o estado do LED.
bool repeating_timer_callback(struct repeating_timer *t) {
    static absolute_time_t last_press_time = 0; // Armazena o tempo da última pressão do botão.
    static bool button_last_state = false;      // Armazena o estado anterior do botão (pressionado ou não).

    // Verifica o estado atual do botão (pressionado = LOW, liberado = HIGH).
    bool button_pressed = !gpio_get(BUTTON_PIN); // O botão pressionado gera um nível baixo (0).

    // Verifica se o botão foi pressionado e realiza o debounce.
    if (button_pressed && !button_last_state && 
        absolute_time_diff_us(last_press_time, get_absolute_time()) > 200000) { // Verifica se 200 ms se passaram (debounce).
        last_press_time = get_absolute_time(); // Atualiza o tempo da última pressão do botão.
        button_last_state = true;              // Atualiza o estado do botão como pressionado.
        button_press_count++;                  // Incrementa o contador de pressões do botão.

        // Verifica se o botão foi pressionado duas vezes.
        if (button_press_count == 2) {
            gpio_put(LED_PIN, true); // Liga o LED.
            led_active = true;       // Atualiza a variável indicando que o LED está ativo.
            printf("LED ligado\n");  // Exibe mensagem na saída serial indicando que o LED foi ligado.

            // Inicia um alarme para desligar o LED após 4 segundos (4000 ms).
            add_alarm_in_ms(4000, turn_off_callback, NULL, false);
            button_press_count = 0; // Reseta o contador de pressões do botão.
        }
    } else if (!button_pressed) {
        button_last_state = false; // Atualiza o estado do botão como liberado quando ele não está pressionado.
    }

    return true; // Retorna true para continuar o temporizador de repetição.
}

int main() {
    stdio_init_all(); // Inicializa a comunicação serial para permitir o uso de printf para depuração.

    // Configura o pino do LED como saída.
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0); // Garante que o LED comece apagado.

    // Configura o pino do botão como entrada com resistor de pull-up interno.
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Habilita o resistor pull-up interno para evitar leituras incorretas.

    // Configura o temporizador repetitivo para verificar o estado do botão a cada 100 ms.
    struct repeating_timer timer;
    add_repeating_timer_ms(100, repeating_timer_callback, NULL, &timer);

    // O loop principal permanece vazio, pois o controle do LED e do botão é gerenciado pelo temporizador.
    while (true) {
        tight_loop_contents(); // Função que otimiza o loop vazio para evitar consumo excessivo de CPU.
    }
}
