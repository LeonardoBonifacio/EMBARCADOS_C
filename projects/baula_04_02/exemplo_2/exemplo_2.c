#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"



int main()
{
    uart_init(uart0,115200); // Inicializa a uart0 com baud rate de 115200
    gpio_set_function(0, GPIO_FUNC_UART); //  Configura o pina 0 como TX
    gpio_set_function(1, GPIO_FUNC_UART); //  Configura o pina 1 como RX

    stdio_init_all();

    // Habilita o FIFO para evitar sobrecarga  de buffer
    uart_set_fifo_enabled(uart0,true);

    // // Configurações adicionais(opcional)
    // uart_set_hw_flow(uart0,false,false); // Desabilita controle de fluxo
    // uart_set_format(uart0,8,1,UART_PARITY_NONE); // Configura formato 8N1

    while (true) {
        // Verifica se há dados disponíveis na UART
        if (uart_is_readable(uart0)){
            // Lê um caractere da UART
            char c = uart_getc(uart0);
            // Envia o caractere  de volta (echo) para verificação
            uart_putc(uart0,c);
        }
        
    }
}
