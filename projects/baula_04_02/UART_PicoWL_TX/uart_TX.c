/* Por: Wilton Lacerda Silva
 * Data: 10/08/2021
 *        Estudo do protocolo de comunicação da UART
 *       Universal Asynchronous Receiver/Transmitter
 *
 * Utiliza a UART0 para enviar dados.
 *
 *
 * O funcionamento é dado assim: O programa envia uma mensagem pela UART0
 * quando o botão é pressionado.
 * A mensagem contém o valor atual de um contador.
 *
 * No Wokwi, foi utilizado um Analisador Lógico para 
 * verificar o sinal TX da UART0.
 * Informações sobre o analisador lógico podem ser encontradas em:	
 * https://wokwi.com/arduino/libraries/logic-analyzer
 * 
 * Um possível programa para fazer a análise do sinal TX é o PulseView.
 * Pode ser baixado em: https://sigrok.org/wiki/Downloads
 *
 */

#include <stdio.h>
#include "pico/stdlib.h"

#define UART_ID uart0    // Seleciona a UART0
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 0    // Pino GPIO usado para TX
#define UART_RX_PIN 1    // Pino GPIO usado para RX
#define LED_PIN 13       // Pino GPIO usado para o LED
#define BUTTON_PIN 5    // Pino GPIO usado para o botão

// Variáveis globais
static volatile uint16_t counter = 0;   // Variável inteira para contagem
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile bool flag_var = 0;      // Variável de flag para debouncing

// Prototipação da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

int main()
{
    // Inicializa a biblioteca padrão
    stdio_init_all();
/*     Inicializa a UART com o padrão: ( 115200, 8N1)
*    115200: A taxa de transmissão de dados.
*    8: O número de bits de dados por caractere (neste caso, 8 bits).
*    N: Não há bit de paridade (No parity).
*    1: O número de bits de parada (neste caso, 1 bit).
*    Low bit first: A ordem dos bits é do bit menos significativo para o mais significativo.
*/
    uart_init(UART_ID, BAUD_RATE);

    // Configura os pinos GPIO para a UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 para TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pino 1 para RX

    // Configura o pino do LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Configura o pino do botão com resistor pull-up
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (1)
    {
        // Verifica se a flag está ativa
        if (flag_var)
        {
            // Cria a mensagem com o valor atual do contador
            char message[50];
            sprintf(message, "Contagem: %d\n", counter);

            // Envia a mensagem pela UART
            uart_puts(UART_ID, message);
            printf("Mensagem enviada: %s", message); // Exibe a mensagem no serial monitor

            // Pisca o LED
            gpio_put(LED_PIN, 1);
            sleep_ms(50);
            gpio_put(LED_PIN, 0);
            sleep_ms(50);
            flag_var = 0; // Limpa a flag de interrupção
        }

        // Pequeno atraso para evitar sobrecarga no loop principal
        sleep_ms(10);
    }

    return 0;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000) // 50 ms de debouncing
    {
        last_time = current_time; // Atualiza o tempo do último evento
        counter++;                // Incrementa o contador
        flag_var = 1;
    }
}