
#include <stdio.h>
#include "pico/stdlib.h"

#define GPIO_BOTAO 5
#define GPIO_LED 13

int main()
{
    uint estado_led =0; //estado inicial do led

    //inicializa as portas digitais
    gpio_init(GPIO_BOTAO);
    gpio_set_dir(GPIO_BOTAO, GPIO_IN); //botão definido como entrada
    gpio_pull_up(GPIO_BOTAO); //Ativa o resistor de pull-up interno

    gpio_init(GPIO_LED);
    gpio_set_dir(GPIO_LED,GPIO_OUT); //led definido como saída

    stdio_init_all();

    //rotina de verificação do estado do botão
    while (true) {
        
        if(gpio_get(GPIO_BOTAO)) //botão em nível alto
        {
            gpio_put(GPIO_LED,0); 

            if (estado_led==0)
            {
                printf("desliga LED \r\n");
            }
            estado_led = 1;
            
        }
        else
        {
            gpio_put(GPIO_LED,1); //botão em nível baixo

            if (estado_led==1)
            {
                printf("liga LED \r\n");
            }
            estado_led = 0; 
        }
    }
}
