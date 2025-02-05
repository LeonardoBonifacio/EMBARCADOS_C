/*
 * Por: Wilton Lacerda Silva
 * 
 * Utiliza a UART0 para receber dados do usuário e enviar de volta
 * 
 * O código foi adaptado para o RP2040 (Pico) e utiliza a biblioteca
 * Pico SDK.
 * 
 * O teste foi feito utilizando a extensão do Wokwi Simulator:
 *  
 * O funcionamento é simples, o código lê um caractere da UART e envia
 * de volta para o usuário. Além disso, ele acende um LED RGB de acordo
 * com o caractere recebido. Por exemplo, se o caractere 'r' for recebido,
 * o LED vermelho é acionado. Se o caractere 'g' for recebido, o LED verde
 * é acionado. Se o caractere 'b' for recebido, o LED azul é acionado.
 * 
 */

#include <stdio.h>
#include "pico/stdlib.h"

#define UART_ID uart0 // Seleciona a UART0
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 0 // Pino GPIO usado para TX
#define UART_RX_PIN 1 // Pino GPIO usado para RX
#define led_pin_g 11
#define led_pin_b 12
#define led_pin_r 13


int main() {
    // Inicializa a biblioteca padrão
    stdio_init_all();

    // Inicializa a UART
    uart_init(UART_ID, BAUD_RATE);

    // Configura os pinos GPIO para a UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 para TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino 1 para RX

    // Mensagem inicial
    const char *init_message = "UART Demo - RP2\r\n"
                               "Digite algo e veja o eco:\r\n";
    uart_puts(UART_ID, init_message);

    gpio_init(led_pin_g);
    gpio_set_dir(led_pin_g, GPIO_OUT);
    gpio_init(led_pin_b);
    gpio_set_dir(led_pin_b, GPIO_OUT);
    gpio_init(led_pin_r);
    gpio_set_dir(led_pin_r, GPIO_OUT);



    while (1) {
        // Verifica se há dados disponíveis para leitura
        if (uart_is_readable(UART_ID)) {
            // Lê um caractere da UART
            char c = uart_getc(UART_ID);
            if (c == 'r'){
                gpio_put(led_pin_r, !gpio_get(led_pin_r));
            }
            if (c == 'g'){
                gpio_put(led_pin_g, !gpio_get(led_pin_g));
            }
            if (c == 'b'){
                gpio_put(led_pin_b, !gpio_get(led_pin_b));
            }                        
            // Envia de volta o caractere lido (eco)
            uart_putc(UART_ID, c);

            // Envia uma mensagem adicional para cada caractere recebido
            uart_puts(UART_ID, " <- Eco do RP2\r\n");
        }
        sleep_ms(40);
    }

    return 0;
}