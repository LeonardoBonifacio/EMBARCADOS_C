#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"



#define I2C_ADDR 0x48 // Endereço do dispositivo I2C

int main()
{
    i2c_init(i2c0, 100 * 1000); // Inicializa  I2C0 a 100khz
    gpio_set_function(4,GPIO_FUNC_I2C); // SDA
    gpio_set_function(5,GPIO_FUNC_I2C); // SCL
    gpio_pull_up(4);
    gpio_pull_up(5);
    stdio_init_all();

    uint8_t buffer[2];
    while (true) {
        i2c_read_blocking(i2c0,I2C_ADDR,buffer,2,false); // Lê 2 bytes do senhor
        uint16_t valor = (buffer[0] << 8) | buffer[1]; // Combina os dois bytes
        printf("Valor lido: %d\n", valor) ; // Exibe o valor lido
        sleep_ms(1000);
    }
}
