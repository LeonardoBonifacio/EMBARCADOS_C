#include <stdio.h>
#include "pico/stdlib.h"

#define LED_VERMELHO_PIN 13
#define LED_VERDE_PIN 11
#define BUTTON_PIN 5

bool ciclo_ativo = false;

void iniciar_ciclo(){
    while (ciclo_ativo)
    {
        // Led verde acesso por 5s
        gpio_put(LED_VERDE_PIN,true);
        sleep_ms(5000);
        gpio_put(LED_VERDE_PIN,false);

        // Led "Amarelo" acesso por 2s
        gpio_put(LED_VERDE_PIN,true);
        gpio_put(LED_VERMELHO_PIN,true);
        sleep_ms(2000);
        gpio_put(LED_VERMELHO_PIN,false);
        gpio_put(LED_VERDE_PIN,false);

        // Led vermelho acesso por 5s
        gpio_put(LED_VERMELHO_PIN,true);
        sleep_ms(5000);
        gpio_put(LED_VERMELHO_PIN,false);
    }
    
}


int main()
{
    stdio_init_all();

    // Inicializa e configura os pinos dos Leds como saida
    gpio_init(LED_AZUL_PIN);
    gpio_set_dir(LED_AZUL_PIN,GPIO_OUT);

    gpio_init(LED_VERDE_PIN);
    gpio_set_dir(LED_VERDE_PIN,GPIO_OUT);

    gpio_init(LED_VERMELHO_PIN);
    gpio_set_dir(LED_VERMELHO_PIN,GPIO_OUT);

    // Inicializa e configura o pino do push button como entrada em pull-dowm
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN , GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    while (true)
    {
        // Verifica se o botão foi pressionado 
        if (gpio_get(BUTTON_PIN) == 0)
        {
            // Alterna o estado do ciclo do semáforo
            ciclo_ativo = !ciclo_ativo;

            // Se o ciclo estiver ativo , inicia o ciclo do semáforo
            if (ciclo_ativo)
            {
                iniciar_ciclo();
            }

            // Adiciona um pequeno delay para evitar múltiplas leituras do botão
            sleep_ms(200);
            
        }
        
    }
    return 0;

}
