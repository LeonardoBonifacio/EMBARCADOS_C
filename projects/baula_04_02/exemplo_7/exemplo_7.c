#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/irq.h"



#define I2C_SLAVE_ADDRESS 0x42 // Endereço I2C do dispositivo slave

uint8_t rx_buffer[16]; // Buffer para armazenar os dadso
volatile int rx_index = 0;


// Função de callback para interrupção I2C
void i2c0_irq_handler(){
    // Enquanto houver dados para ler na i2c
    while (i2c_get_read_available(i2c0)){
        // Lê os dados da I2C e armazena no buffer
        if (rx_index < sizeof(rx_buffer)){
            i2c_read_raw_blocking(i2c0,&rx_buffer[rx_index],1);
            rx_index++;
        }
    }
      
}


int main()
{
    i2c_init(i2c0, 100 * 1000); // Inicializa  I2C0 a 100khz
    gpio_set_function(4,GPIO_FUNC_I2C); // SDA
    gpio_set_function(5,GPIO_FUNC_I2C); // SCL
    gpio_pull_up(4);
    gpio_pull_up(5);
    stdio_init_all();
    stdio_init_all();


    //  Configura o pico com dispositivo I2C SLAVE
    i2c_set_slave_mode(i2c0,true,I2C_SLAVE_ADDRESS);
    // Configura o manipulador de interrupção e habilita a interrupção na I2C
    irq_set_exclusive_handler(I2C0_IRQ,i2c0_irq_handler);
    irq_set_enabled(I2C0_IRQ,true);
    // Habilita interupções de recebimento (RX) para o modo slave
    i2c0->hw->intr_mask = I2C_IC_INTR_MASK_M_RX_FULL_BITS;
    rx_index = 0; // Limpa o buffer antes de começar 
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
