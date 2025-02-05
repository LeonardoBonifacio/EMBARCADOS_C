#include <stdio.h>
#include "pico/stdlib.h" // Biblioteca padrão para funcionalidades básicas como GPIO, temporização e E/S.
#include "pico/time.h"   // Biblioteca para gerenciamento de tempo, como manipulação de temporizadores e atrasos.

#define LED_PIN_RED 12 //Definição da GPIO de saída
bool led_on = false;

int main() {

    stdio_init_all(); // Inicializa a comunicação serial

    // Inicializar o pino GPIO12
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED,GPIO_OUT);

    // Define um intervalo de tempo em milissegundos
    uint32_t interval = 1000;

    // Calcula o próximo tempo absoluto em que a ação deve ocorrer.
    // get_absolute_time() retorna o tempo atual do sistema.
    // delayed_by_us() calcula um tempo futuro adicionando o intervalo em microsegundos ao tempo atual.
    absolute_time_t next_wake_time = delayed_by_us(get_absolute_time(), interval * 1000); // 1.000.000 MICROSSEGUNDOS = 1 SEGUNDO

    // Loop infinito que mantém o programa em execução contínua.
    while (true) {
        // Verifica se o tempo atual atingiu ou ultrapassou o tempo definido em next_wake_time.
        if (time_reached(next_wake_time)) {

            // Imprime uma mensagem na saída serial indicando que 1 segundo se passou.
            printf("1 segundo passou\n");
            
            //Liga ou desliga o led.
            led_on = !led_on;
            gpio_put(LED_PIN_RED,led_on);

            // Calcula o próximo tempo de despertar, adicionando o intervalo de 1 segundo ao tempo atual.
            // Isso garante que a próxima execução aconteça exatamente 1 segundo após a última.
            next_wake_time = delayed_by_us(next_wake_time, interval * 1000);
        }

        // Introduz uma pequena pausa de 1 milissegundo para reduzir o uso da CPU.
        // Isso evita que o loop rode desnecessariamente rápido, economizando recursos.
        sleep_ms(1);
    }
}