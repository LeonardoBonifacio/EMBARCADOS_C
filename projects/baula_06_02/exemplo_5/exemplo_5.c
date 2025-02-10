#include <stdio.h> //biblioteca padrão da linguagem C
#include "pico/stdlib.h" //subconjunto central de bibliotecas do SDK Pico
#include "hardware/pwm.h" //biblioteca para controlar o hardware de PWM

#define PWM_LED 16 //pino do LED conectado a GPIO como PWM

//rotina principal
int main()
{
    stdio_init_all(); //inicializa o sistema padrão de I/O

    gpio_set_function(PWM_LED, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM

    uint slice = pwm_gpio_to_slice_num(PWM_LED); //obter o canal PWM da GPIO

    pwm_set_clkdiv(slice, 2.0); //define o divisor de clock do PWM

    pwm_set_wrap(slice, 2000); //definir o valor de wrap

    pwm_set_gpio_level(PWM_LED, 500); //definir o ciclo de trabalho (duty cycle) do pwm

    pwm_set_enabled(slice, true); //habilita o pwm no slice correspondente

    //loop principal
    while (true) {

        printf("slice:%d\n",slice);
        sleep_ms(1000);
    }
}