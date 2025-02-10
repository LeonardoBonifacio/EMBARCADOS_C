#include <stdio.h>             // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"       // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h"      // Biblioteca para controle do ADC (Conversor Analógico-Digital).
#include "hardware/pwm.h"      // Biblioteca para controle de PWM.

// Definições dos pinos para o joystick e LED
#define VRX_PIN 26    // Define o pino GP26 para o eixo X do joystick (Canal ADC0)
#define LED_PIN 12    // Define o pino GP12 para o LED controlado via PWM

// Função para inicializar o PWM
uint pwm_init_gpio(uint gpio, uint wrap) {
    // Define o pino GPIO como saída de PWM
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    
    // Obtém o número do "slice" de PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    
    // Define o "wrap" do PWM, que define o valor máximo para o contador
    // Isso afeta a resolução do PWM (quanto maior, maior a precisão do duty cycle)
    pwm_set_wrap(slice_num, wrap);
    
    // Habilita o PWM no slice
    pwm_set_enabled(slice_num, true);  
    return slice_num;  // Retorna o número do slice para uso posterior
}

int main() {
    // Inicializa a comunicação serial para permitir o uso de printf
    // Isso permite enviar dados via USB para depuração no console
    stdio_init_all();

    // Inicializa o módulo ADC do Raspberry Pi Pico
    adc_init(); // Habilita o hardware ADC para uso

    // Configura o pino GP26 (VRX_PIN) como entrada analógica para o ADC
    adc_gpio_init(VRX_PIN); // Pino VRX_PIN será usado para ler o valor analógico do eixo X do joystick

    // Inicializa o PWM para o LED no pino GP12
    uint pwm_wrap = 4096;  // Define o valor máximo para o contador do PWM (influencia o duty cycle)
    uint pwm_slice = pwm_init_gpio(LED_PIN, pwm_wrap);  // Inicializa o PWM no pino LED_PIN e retorna o número do slice
    
    uint32_t last_print_time = 0; // Variável para armazenar o tempo da última impressão na serial

    // Loop principal do programa
    while (true) {
        // Leitura do valor do ADC para VRX (Eixo X do joystick)
        adc_select_input(0);  // Seleciona o canal 0 do ADC, correspondente ao pino GP26 (VRX)
        uint16_t vrx_value = adc_read();  // Lê o valor analógico do eixo X, retornando um valor entre 0 e 4095

        // Controle do LED com PWM baseado no valor do ADC0 (VRX)
        pwm_set_gpio_level(LED_PIN, vrx_value);  // Ajusta o duty cycle do LED proporcional ao valor lido de VRX

        // Calcula o duty cycle em porcentagem para impressão
        float duty_cycle = (vrx_value / 4095.0) * 100;  // Converte o valor lido do ADC em uma porcentagem do duty cycle

        // Imprime os valores lidos e o duty cycle proporcional na comunicação serial a cada 1 segundo
        uint32_t current_time = to_ms_since_boot(get_absolute_time());  // Obtém o tempo atual desde o boot do sistema
        if (current_time - last_print_time >= 1000) {  // Verifica se passou 1 segundo desde a última impressão
            printf("VRX: %u\n", vrx_value);  // Imprime o valor lido de VRX no console serial
            printf("Duty Cycle LED: %.2f%%\n", duty_cycle);  // Imprime o duty cycle calculado em porcentagem
            last_print_time = current_time;  // Atualiza o tempo da última impressão
        }

        // Introduz um atraso de 100 milissegundos antes de repetir a leitura
        sleep_ms(100);  // Pausa o programa por 100ms para evitar leituras e impressões muito rápidas
    }

    return 0;  // Retorna 0 ao final do programa (nunca será alcançado neste caso, pois o loop é infinito)
}
