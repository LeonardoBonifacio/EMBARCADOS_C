#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
// By default the stdout UART is `uart0`
#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 0 and 1 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1



int main()
{
    stdio_init_all();

    // Set up our UART
    uart_init(UART_ID, BAUD_RATE); //  Inicializa a UART com baud rate de 125200
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART); // Configura o pino 0 como TX
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART); // Configura o pina 1 como RX
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default UART
    
    
    
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart

    while (true) {
        sleep_ms(5000);
        uart_puts(UART_ID, " Hello, UART!\n");// Envia uma mensagem via UART
        char c = uart_getc(UART_ID);  // Espera até que um caractere seja recebido
        printf("Caractere recebido: '%c'\n", c);
    }

    return 0;
}

 