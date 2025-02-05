#include <stdio.h>
#include "pico/stdlib.h"

#define LED_RED_PIN 11
#define LED_YELLOW_PIN 12
#define LED_GREEN_PIN 13


volatile int estado = 0;  // Variável global para controlar o estado do semáforo

// Callback chamado pelo temporizador repetitivo
bool repeating_timer_callback(struct repeating_timer *t) {
    // Desliga todos os LEDs antes de mudar de estado
    gpio_put(LED_RED_PIN, false);
    gpio_put(LED_YELLOW_PIN, false);
    gpio_put(LED_GREEN_PIN, false);

    // Alterna entre os estados do semáforo
    switch (estado) {
        case 0:  // Vermelho
            gpio_put(LED_RED_PIN, true);
            printf("Sinal fechado\n");
            break;
        case 1:  // Amarelo
            gpio_put(LED_YELLOW_PIN, true);
            printf("Sinal abrindo ou  vai freiando\n");
            break;
        case 2:  // Verde
            gpio_put(LED_GREEN_PIN, true);
            printf("Sinal Aberto\n");
            break;
    }

    // Atualiza o estado para o próximo ciclo
    estado = (estado + 1) % 3;

    return true;  // Mantém o temporizador rodando
}

int main() {
    stdio_init_all();

    // Inicializa os pinos dos LEDs como saída
    gpio_init(LED_RED_PIN);
    gpio_init(LED_YELLOW_PIN);
    gpio_init(LED_GREEN_PIN);

    gpio_set_dir(LED_RED_PIN, GPIO_OUT);
    gpio_set_dir(LED_YELLOW_PIN, GPIO_OUT);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);

    struct repeating_timer timer;

    // Configura o temporizador para chamar a função a cada 3 segundos (3000 ms)
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);

    while (true) {
        sleep_ms(1000);
        printf("Sinaleira funcionando\n");

    }
}
