#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"


// Função de callback que será chamada quando a interrupção ocorrer
void on_uart_rx(){
    // Enquanto houver dados para ler na UART
    while (uart_is_readable(uart0)){
        // Lê o caractere recebido 
        char c = uart_getc(uart0);
        // Envia o caractere de volta (echo) para verificação
        printf("%c", c);
    }
    
}


int main()
{
    uart_init(uart0,115200); // Inicializa a uart0 com baud rate de 115200
    gpio_set_function(0, GPIO_FUNC_UART); //  Configura o pina 0 como TX
    gpio_set_function(1, GPIO_FUNC_UART); //  Configura o pina 1 como RX

    // Habilita o FIFO para evitar sobrecarga  de buffer
    uart_set_fifo_enabled(uart0,true);

    // Configura a interrupção para a UART0
    // Define a função de callback para a interrupção  de recepção
    irq_set_exclusive_handler(UART0_IRQ,on_uart_rx);
    // Habilita a interrupção na UART0
    irq_set_enabled(UART0_IRQ,true);
    // Habilita  a interrupção  de recepção  de dados (RX) na uart
    uart_set_irq_enables(uart0,true,false);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
    
}
