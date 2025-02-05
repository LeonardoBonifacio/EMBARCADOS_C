#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"



int main()
{
    spi_init(spi0,500000); // Inicializa SPI com velocidade de 500khz
    gpio_set_function(2,GPIO_FUNC_SPI); // Configura pino 2 como SCLK
    gpio_set_function(3,GPIO_FUNC_SPI); // Configura pino 3 como MOSI
    gpio_set_function(4,GPIO_FUNC_SPI); // Configura pino 4 como MISO
    gpio_set_function(5,GPIO_FUNC_SPI); // Configura pino 5 como CS

    uint8_t data_out = 0x55; // Dados para enviar
    uint8_t data_in = 0; // Dados para receber

    while (true) {
        gpio_put(5,0); // Seleciona o dispositivo
        spi_write_read_blocking(spi0,&data_out,&data_in,1); // Envia e recebe 1 byte
        gpio_put(5,1); // Libera o dispositivo
        sleep_ms(1000); // Pausa de 1 s
    }
}
