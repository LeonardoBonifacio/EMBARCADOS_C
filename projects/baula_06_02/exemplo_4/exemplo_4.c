#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"


// Definições de constantes para o controle do PWM
const uint LED = 12;                      // Pino do Led conectado   
const uint16_t PERIOD = 2000;             // Período do PWM (valor máximo do contador)
const float DIVIDER_PWM = 16.0;           // Divisor fracional do clock para o PWM
const uint16_t LED_STEP = 100;            // Passo de incremento/decremento para o duty cycle
const uint32_t PWM_REFRESH_LEVEL = 400000;// Nível de contagem para suavizar o fade do LED
uint16_t led_level = 100;                 // Nível inicial do PWM (duty cicle do LED)


// Função de tratamento da interrupção do PWM
void pwm_irq_handler()
{
    static uint up_down = 1;    // Variável que controla se o LED está aumentando ou diminuindo o brilho
    static uint32_t count = 0;  // Contador para controlar a frequência de atualização do PWM
    uint32_t slice = pwm_get_irq_status_mask(); // Obtém o status da interrupção do PWM
    pwm_clear_irq(slice); // Limpa a interrupção do slice correspondente
    if (count++ < PWM_REFRESH_LEVEL) return; // Verifica se o contador atingiu o nível de atualização do PWM        
    count = 0; // Reseta o contador para iniciar a próxima verificação    
    if (up_down){// Ajusta o duty cycle do LED, alternando entre aumentar e diminuir
        led_level += LED_STEP; // Incrementa o nível do LED
        if (led_level >= PERIOD)
            up_down = 0; // Muda a direção para diminuir o brilho quando atinge o valor máximo
    }else{
        led_level -= LED_STEP; // Decrementa o nível do LED
        if (led_level <= LED_STEP)
            up_down = 1; // Muda a direção para aumentar o brilho quando atinge o valor mínimo
    }
    pwm_set_gpio_level(LED, led_level); // Atualiza o duty cycle do PWM para o pino do LED
}



// Função para configurar o PWM para o LED
void setup_pwm(){
    uint slice;

    gpio_set_function(LED,GPIO_FUNC_PWM);  // Configura o pino do Led para função PWM
    slice = pwm_gpio_to_slice_num(LED);    // Obtém o slice do PWM associado ao pino do led
    pwm_set_clkdiv(slice,DIVIDER_PWM);     // Define o divisor de clock do PWM
    pwm_set_wrap(slice,PERIOD);            // Configura o valor máximo do contador(PERIODO)
    pwm_set_gpio_level(LED,led_level);     // Define o nível inicial do PWM para o pino do Led
    pwm_set_enabled(slice,true);           // Habilita o pwm no slice correspondente


    // Configura e habilita a interrupção do PWM
    irq_set_exclusive_handler(PWM_IRQ_WRAP,pwm_irq_handler); // Define o handler da interrupção
    pwm_clear_irq(slice);                                    // Limpar interrupções pendentes no slice
    pwm_set_irq_enabled(slice,true);                         // Habilita interrupções para o slice do pwm
    irq_set_enabled(PWM_IRQ_WRAP,true);                      // Habilita interrupções globais para o pwm

}


int main()
{
    stdio_init_all(); // Inicializa o sistema padrão de I/O (UART, etc.)
    setup_pwm();      // Configura o PWM para o LED


    // Loop infinito - o controle do PWM é feito pela interrupção
    while (true)
    {
        // O loop principal pode ser usado para outras tarefas, se necessário
    }

}
