#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Configuração do pino do buzzer
#define BUZZER_PIN_A 21
#define BUZZER_PIN_B 10

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY_A 10000
#define BUZZER_FREQUENCY_B 100

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin, int buzzer_frequency) {
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (buzzer_frequency * 4096)); // Divisor de clock
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin, 0);
}

// Definição de uma função para emitir um beep com duração especificada
void beep(uint pin_a, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin_a);
    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin_a, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin_a, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

int main() {
    // Configuração do GPIO para o botão como entrada com pull-up
    const uint BUTTON_PIN_A = 5;  // Pino do botão
    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);

    // Configuração do GPIO para o botão como entrada com pull-up
    const uint BUTTON_PIN_B = 6;  // Pino do botão
    gpio_init(BUTTON_PIN_B);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_B);

    // Configuração do GPIO para o buzzer como saída
    gpio_init(BUZZER_PIN_A);
    gpio_set_dir(BUZZER_PIN_A, GPIO_OUT);
    // Inicializar o PWM no pino do buzzer
    pwm_init_buzzer(BUZZER_PIN_A,BUZZER_FREQUENCY_A);

    // Configuração do GPIO para o buzzer como saída
    gpio_init(BUZZER_PIN_B);
    gpio_set_dir(BUZZER_PIN_B, GPIO_OUT);
    // Inicializar o PWM no pino do buzzer
    pwm_init_buzzer(BUZZER_PIN_B,BUZZER_FREQUENCY_B);

    while (true) {
        // Verifica o estado do botão
        if (gpio_get(BUTTON_PIN_A) == 0) {  // Botão pressionado (nível lógico baixo)
            beep(BUZZER_PIN_A, 1000);
        }
        if (gpio_get(BUTTON_PIN_B) == 0) {  // Botão pressionado (nível lógico baixo)
            beep(BUZZER_PIN_B, 1000);
        }
        
    }

    return 0;
}
