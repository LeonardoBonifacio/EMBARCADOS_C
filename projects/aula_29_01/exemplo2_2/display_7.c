  #include <stdio.h>
  #include "pico/stdlib.h"
  #include "hardware/timer.h" // Inclui a biblioteca para gerenciamento de temporizadores de hardware.

// Definição dos pinos dos segmentos do display
const uint8_t segment_pins[] = {0, 1, 2, 3, 4, 5, 6};

// Mapeamento dos dígitos para os segmentos do display
const uint8_t digits[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 0, 0, 1, 1}  // 9
};

// Função para exibir um dígito no display
void display_digit(uint8_t digit) {
    for (int i = 0; i < 7; i++) {
        gpio_put(segment_pins[i], digits[digit][i]);
    }
}

// Função de callback que será chamada repetidamente pelo temporizador
// O tipo bool indica que a função deve retornar verdadeiro ou falso para continuar ou parar o temporizador.
bool repeating_timer_callback(struct repeating_timer *t) {
    
    static uint8_t contador = 0;

    // Imprime uma mensagem na saída serial indicando que 1 segundo se passou.
    printf("%d segundo(0s)\n",contador+1);
    display_digit(contador);  // Exibe o dígito atual no display

    contador++;  // Incrementa o contador
    if (contador > 9) {
        contador = 0;  // Reinicia o contador após 9
    }
    return true;
}

int main() {
    
    stdio_init_all();
    
    // Inicializa e configura os pinos dos segmentos como saída
    for (int i = 0; i < 7; i++) {
        gpio_init(segment_pins[i]);
        gpio_set_dir(segment_pins[i], GPIO_OUT);
    }

    //configuração do timer
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);

    //rotina principal
    while (true) {
        printf("passaram 10 segundos\n");
        sleep_ms(10000);  // Espera 1 segundo
    }
    return 0;
}
