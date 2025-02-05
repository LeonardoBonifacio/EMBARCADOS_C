#include <stdio.h> // Biblioteca padrão do C.
#include "pico/stdlib.h" // Biblioteca padrão do Raspberry Pi Pico para controle de GPIO, temporização e comunicação serial.
#include "pico/time.h"   // Biblioteca para gerenciamento de temporizadores e alarmes.

const uint LED_PIN_GREEN = 11;    // Define o pino GPIO 11 para controlar o LED VERDE.
const uint LED_PIN_BLUE = 12;    // Define o pino GPIO 12 para controlar o LED AZUL.
const uint LED_PIN_RED = 13;    // Define o pino GPIO 13 para controlar o LED VERMELHO.
const uint BUTTON_PIN = 5;  // Define o pino GPIO 5 para ler o estado do botão.

bool led_green_active = false;    // Indica se o LED verde está atualmente aceso (para evitar múltiplas ativações).
bool led_blue_active = false;    // Indica se o LED azul está atualmente aceso (para evitar múltiplas ativações).
bool led_red_active = false;    // Indica se o LED vermelho está atualmente aceso (para evitar múltiplas ativações).

// Função de callback para desligar o LED verde após o tempo programado.
int64_t turn_off_callback_green(alarm_id_t id, void *user_data) {

    // Desliga o LED configurando o pino LED_PIN_GREEN para nível baixo.
    gpio_put(LED_PIN_GREEN, false);

    // Atualiza o estado de 'led_green_active' para falso, indicando que o LED está desligado.
    led_green_active = false;

    // Retorna 0 para indicar que o alarme não deve se repetir.
    return 0;
}

// Função de callback para desligar o LED azul após o tempo programado.
int64_t turn_off_callback_blue(alarm_id_t id, void *user_data) {

    // Desliga o LED configurando o pino LED_PIN_BLUE para nível baixo.
    gpio_put(LED_PIN_BLUE, false);

    // Atualiza o estado de 'led_blue_active' para falso, indicando que o LED está desligado.
    led_blue_active = false;

    // Retorna 0 para indicar que o alarme não deve se repetir.
    return 0;
}

// Função de callback para desligar o LED vermelho após o tempo programado.
int64_t turn_off_callback_red(alarm_id_t id, void *user_data) {

    // Desliga o LED configurando o pino LED_PIN_RED para nível baixo.
    gpio_put(LED_PIN_RED, false);

    // Atualiza o estado de 'led_red_active' para falso, indicando que o LED está desligado.
    led_red_active = false;

    // Retorna 0 para indicar que o alarme não deve se repetir.
    return 0;
}

int main() {

    // Configura os pinos dos leds como saída digital.
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);

    // Configura o pino BUTTON_PIN (5) como entrada digital.
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // Habilita o resistor pull-up interno para o pino do botão.
    // Isso garante que o pino seja lido como alto (3,3 V) quando o botão não está pressionado.
    gpio_pull_up(BUTTON_PIN);

    // Loop principal do programa que verifica continuamente o estado do botão.
    while (true) {
        // Verifica se o botão foi pressionado (nível baixo no pino) e se os LEDS não está ativo.
        if (gpio_get(BUTTON_PIN) == 0 && !led_green_active && !led_blue_active && !led_red_active) {
            // Adiciona um pequeno atraso para debounce, evitando leituras errôneas.
            sleep_ms(50);

            // Verifica novamente o estado do botão após o debounce.
            if (gpio_get(BUTTON_PIN) == 0) {
                // Liga os LED configurando os pinos para nível alto.
                gpio_put(LED_PIN_GREEN, true);
                gpio_put(LED_PIN_BLUE, true);
                gpio_put(LED_PIN_RED, true);

                // Define como true para indicar que o LED está aceso.
                led_green_active = true;
                led_blue_active = true;
                led_red_active = true;

                // Agenda um alarme para desligar os leds a cada 3 segundos em sequencia.
                // A função turn_off_callback respectiva a cada led será chamada após esse tempo.
                add_alarm_in_ms(3000, turn_off_callback_green, NULL, false);
                add_alarm_in_ms(6000, turn_off_callback_blue, NULL, false);
                add_alarm_in_ms(9000, turn_off_callback_red, NULL, false);
            }
        }
        sleep_ms(10);
    }

    return 0;
}
