#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include "hardware/uart.h"




#define UART0_TX_PIN 0
#define UART0_RX_PIN 1
#define UART1_TX_PIN 8
#define UART1_RX_PIN 9


char palavra[50];


int main()
{
    stdio_init_all();



    // Configurações do UART0
    uart_init(uart0,115200);// Inicializa a uart0 com baud rate de 115200
    gpio_set_function(UART0_TX_PIN,GPIO_FUNC_UART); //  Configura o pino 0 como Tx
    gpio_set_function(UART0_RX_PIN,GPIO_FUNC_UART); //  Configura o pino 1 como Rx


    // Configurações da UART1
    uart_init(uart1,115200);// Inicializa a uart0 com baud rate de 115200
    gpio_set_function(UART1_TX_PIN,GPIO_FUNC_UART); //  Configura o pino 8 como Tx
    gpio_set_function(UART1_RX_PIN,GPIO_FUNC_UART); //  Configura o pino 9 como Rx

    uart_set_fifo_enabled(uart0,true);
    uart_set_fifo_enabled(uart1,true);

    // Recebe a mensagem via console
    printf("Digite mensagem para enviar uart0: ");
    scanf("%s", &palavra);


    // Envia mensagem pela uart1
    uart_puts(uart0,palavra);

    // Aguarda e verifica se há dados disponíveis na UART1
    sleep_ms(100); // Dá um tempo  para a transmissão
    if (uart_is_readable(uart1)){
        char recebido[50];
        int i = 0;


        // Lê a mensagem recebida da UART1
        while (uart_is_readable(uart1) && i < sizeof(recebido) - 1){
            recebido[i++] = uart_getc(uart1);
        }
        recebido[i] = '\0'; // Finaliza a string recebida
        sleep_ms(50);
        // Imprime a mensagem recebida no console
        printf("\nMensagem recebida da Uart1: %s\n", palavra,recebido);
        
    }
    else{
        printf("Nenhuma mensagem recebida da UART1.\n");
    }
    

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
