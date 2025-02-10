#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para controle do ADC (Conversor Analógico-Digital).

// Definições dos pinos para o joystick, botão e LEDs
#define VRX_PIN 26    // Define o pino GP26 para o eixo X do joystick (Canal ADC0).
#define VRY_PIN 27    // Define o pino GP27 para o eixo Y do joystick (Canal ADC1).
#define SW_PIN 22     // Define o pino GP22 para o botão do joystick (entrada digital).
#define LED1_PIN 11   // Define o pino GP11 para o LED 1.
#define LED2_PIN 13   // Define o pino GP13 para o LED 2.
#define LED3_PIN 12   // Define o pino GP12 para o LED controlado pelo botão (SW_PIN).

int main() {
    // Inicializa a comunicação serial para permitir o uso de printf.
    // Isso permite enviar mensagens para o console via USB, facilitando a depuração.
    stdio_init_all();

    // Inicializa o módulo ADC do Raspberry Pi Pico.
    // Isso prepara o ADC para ler valores dos pinos analógicos.
    adc_init();

    // Configura os pinos GPIO 26 e 27 para leitura analógica do ADC.
    adc_gpio_init(VRX_PIN); // Configura GP26 (ADC0) para o eixo X do joystick.
    adc_gpio_init(VRY_PIN); // Configura GP27 (ADC1) para o eixo Y do joystick.

    // Configura o pino do botão do joystick como entrada digital com pull-up interno.
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN); // Habilita o pull-up interno para garantir leitura estável.

    // Configura os pinos dos LEDs como saída.
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_put(LED1_PIN, false); // Inicialmente, LED1 apagado.

    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_put(LED2_PIN, false); // Inicialmente, LED2 apagado.

    gpio_init(LED3_PIN);
    gpio_set_dir(LED3_PIN, GPIO_OUT);
    gpio_put(LED3_PIN, false); // Inicialmente, LED3 apagado.

    // Loop infinito para ler continuamente os valores do joystick, do botão e controlar os LEDs.
    while (true) {
        // Leitura do valor do ADC para VRX (Eixo X do joystick)
        adc_select_input(0); // Seleciona canal 0 (GP26 - VRX)
        uint16_t vrx_value = adc_read(); // Lê o valor do eixo X, de 0 a 4095.

        // Leitura do valor do ADC para VRY (Eixo Y do joystick)
        adc_select_input(1); // Seleciona canal 1 (GP27 - VRY)
        uint16_t vry_value = adc_read(); // Lê o valor do eixo Y, de 0 a 4095.

        // Leitura do estado do botão do joystick (SW)
        bool sw_value = gpio_get(SW_PIN) == 0; // 0 indica que o botão está pressionado.

        // Controle do LED1 baseado no valor do ADC0 (VRX)
        if (vrx_value > 2100) {
            gpio_put(LED1_PIN, true); // Liga o LED1.
        } else {
            gpio_put(LED1_PIN, false); // Desliga o LED1.
        }

        // Controle do LED2 baseado no valor do ADC1 (VRY)
        if (vry_value > 2100) {
            gpio_put(LED2_PIN, true); // Liga o LED2.
        } else {
            gpio_put(LED2_PIN, false); // Desliga o LED2.
        }

        // Controle do LED3 baseado no estado do botão (SW_PIN)
        if (sw_value) { // Verifica se o botão no pino 22 está pressionado
            gpio_put(LED3_PIN, true); // Liga o LED3.
        } else {
            gpio_put(LED3_PIN, false); // Desliga o LED3.
        }

        // Imprime os valores lidos na comunicação serial.
        printf("VRX: %u, VRY: %u, SW: %d\n", vrx_value, vry_value, sw_value);

        // Introduz um atraso de 500 milissegundos antes de repetir a leitura.
        sleep_ms(500);
    }

    // Retorna 0 indicando que o programa terminou com sucesso.
    return 0;
}
