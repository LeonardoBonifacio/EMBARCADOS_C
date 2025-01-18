#include "pico/stdlib.h"

// Definições de tempos em milissegundos
#define TEMPO_DOS_PONTOS 200      // Duração do ponto (.) em milissegundos
#define TEMPO_DOS_TRACOS 800     // Duração do traço (-) em milissegundos
#define TEMPO_ENTRE_PONTOS_E_TRACOS 125      // Intervalo entre pontos/traços no mesmo caractere
#define TEMPO_ENTRE_LETRAS 250 // Intervalo entre letras
#define TEMPO_ENTRE_PALAVRAS 3000  // Intervalo entre palavras

// Pino do LED 
#define LED_PIN_GREEN 11
#define LED_PIN_BLUE 12
#define LED_PIN_RED 13

// Função para piscar o LED por um tempo específico
void blink(int duracao, int led) {
    gpio_put(led, 1); // Liga o LED
    sleep_ms(duracao);   // Aguarda o tempo especificado
    gpio_put(led, 0); // Desliga o LED
    sleep_ms(TEMPO_ENTRE_PONTOS_E_TRACOS); // Aguarda o intervalo entre sinais
}

// Função para emitir o sinal SOS em código Morse
void sos() {
    // S (3 pontos)
    for (int i = 0; i < 3; i++) {
        blink(TEMPO_DOS_PONTOS,LED_PIN_RED);
    }
    sleep_ms(TEMPO_ENTRE_LETRAS);

    // O (3 traços)
    for (int i = 0; i < 3; i++) {
        blink(TEMPO_DOS_TRACOS,LED_PIN_GREEN);
    }
    sleep_ms(TEMPO_ENTRE_LETRAS);

    // S (3 pontos)
    for (int i = 0; i < 3; i++) {
        blink(TEMPO_DOS_PONTOS,LED_PIN_BLUE);
    }
    sleep_ms(TEMPO_ENTRE_PALAVRAS);
}

// Função principal
int main() {
    // Inicializa o SDK e configura o pino do LED como saída
    stdio_init_all();
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);

    while (1) {
        sos(); // Chama a função para emitir o padrão SOS
    }

    return 0;
}
