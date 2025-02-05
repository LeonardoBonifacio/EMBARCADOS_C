#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/irq.h"

// Buffer para armazenar dados recebidos
#define BUFFER_SIZE 10
uint8_t rx_buffer[BUFFER_SIZE];
volatile int rx_index = 0;


// Função de callback para tratar interrupções de SPI
void on_spi_rx(){
    while (spi_is_readable(spi0)){
        // Lê os dados da SPI e armazena  no buffer
        uint8_t received = spi_get_hw(spi0)->dr;
        if (rx_index < BUFFER_SIZE){
            rx_buffer[rx_index++] = received;
        }
        
    }
    
}



int main()
{
    stdio_init_all();
    spi_init(spi0, 500 * 1000); // Inicializa SPI com velocidade de 500khz(CPOL = 0, CPHA = 0)
    gpio_set_function(2,GPIO_FUNC_SPI); // Configura pino 2 como SCLK
    gpio_set_function(3,GPIO_FUNC_SPI); // Configura pino 3 como MOSI
    gpio_set_function(4,GPIO_FUNC_SPI); // Configura pino 4 como MISO
    gpio_set_function(5,GPIO_FUNC_SPI); // Configura pino 5 como CS

    // Habilita interrupções na SPIO
    irq_set_exclusive_handler(SPI0_IRQ,on_spi_rx);
    irq_set_enabled(SPI0_IRQ,true);
    spi_get_hw(spi0)->imsc = SPI_SSPIMSC_RXIM_BITS;
    // Habilita interrupção de recepção

    uint8_t data_out = 0x55; // Dados para enviar

    while (true) {
        // Reseta índice do buffer 
        rx_index = 0;

        // Seleciona o dispositivo
        gpio_put(5,0);

        // Envia dados via SPI
        spi_write_blocking(spi0,&data_out,1);

        // Aguarda até que todos os dados sejam recebidos
        sleep_ms(100);

        // Libera o dispositivo
        gpio_put(5,1);

        // Processa os dados recebidos
        if (rx_index > 0){
            printf("Dados recebidos");
            for (int i = 0; i < rx_index; i++){
                printf("%02x ", rx_buffer[i]);
            }
            printf("\n");
        }
        sleep_ms(1000);// Pausa de 1s antes do proximo ciclo
    }
}
