#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para controle do ADC (Conversor Analógico-Digital).

int main() {
    // Inicializa a comunicação serial para permitir o uso de printf
    // Isso permite enviar mensagens via USB para depuração.
    stdio_init_all();

    // Inicializa o módulo ADC (Conversor Analógico-Digital) do Pico
    // Isso prepara o ADC para começar a ler os valores dos canais analógicos.
    adc_init();

    // Configura o pino GPIO 26 para uso como entrada ADC (Canal 0)
    // O pino 26 está associado ao canal 0 do ADC.
    adc_gpio_init(26);

    // Seleciona o canal 0 do ADC para leitura
    // Essa função configura o ADC para ler o valor do pino 26.
    adc_select_input(0);

    // Loop infinito para leitura contínua do valor analógico
    while (true) {
        // Lê o valor do ADC no canal selecionado (canal 0)
        // O valor lido será um número de 0 a 4095, correspondente ao valor analógico.
        uint16_t adc_value = adc_read();

        // Imprime o valor lido do ADC na comunicação serial
        // Isso envia o valor lido para o console, facilitando a depuração e visualização.
        printf("Valor do ADC: %u\n", adc_value);

        // Atraso de 500 milissegundos para evitar a sobrecarga na comunicação serial
        // Isso evita que a leitura e impressão sejam feitas muito rapidamente.
        sleep_ms(500);
    }

    // Retorna 0 indicando que o programa terminou com sucesso
    // Embora este código nunca chegue a este ponto, pois o loop é infinito.
    return 0;
}
