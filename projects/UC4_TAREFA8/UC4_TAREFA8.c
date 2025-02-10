#include <stdio.h> //biblioteca padrão da linguagem C
#include "pico/stdlib.h" //subconjunto central de bibliotecas do SDK Pico
#include "hardware/pwm.h" //biblioteca para controlar o hardware de PWM

#define PWM_SERVO_MOTOR 22 //pino do servo motor conectado a GPIO como PWM
// Obs para testar as funcionalidades desse código no led azul da bitdogled basta mudar a gpio acima para 12

void mexe_servo_motor_de_0_a_180_e_reverso(){
    uint ciclo_ativo = 625; // Inicializa o ciclo ativo apontado o servo motor a 0 graus ou aonde ele parou após a função main
    while (ciclo_ativo <= 3000)// Enquanto o servo motor não chegar a 180graus ou 12% do wrap que é o duty cycle em questão
    {
        ciclo_ativo += 5;// aumenta devagar o tempo que fica em ciclo ativo em 5us
        pwm_set_gpio_level(PWM_SERVO_MOTOR,ciclo_ativo);// Define o novo tempo
        sleep_ms(10);// delay para suavizar o movimento
    }

    while (ciclo_ativo >= 625)// Enquanto o servo motor não chegar a 0 graus ou 2.5% do wrap que é o duty cycle em questão
    {
        ciclo_ativo -= 5;// Diminui devagar o tempo em que fica em ciclo ativo em 5us
        pwm_set_gpio_level(PWM_SERVO_MOTOR,ciclo_ativo);// Define o novo tempo
        sleep_ms(10);// delay para suavizar o movimento
    }
    
    
}

//rotina principal
int main()
{
    stdio_init_all(); //inicializa o sistema padrão de I/O
    gpio_set_function(PWM_SERVO_MOTOR, GPIO_FUNC_PWM); //habilitar o pino GPIO como PWM
    uint slice = pwm_gpio_to_slice_num(PWM_SERVO_MOTOR); //obter o canal PWM da GPIO
    // Frequência de 50hz e 20ms de periodo para clock do sistema de 125mhz, divisor de clock de 100 e wrap de 25000
    pwm_set_clkdiv(slice, 100); //define o divisor de clock do PWM
    pwm_set_wrap(slice, 25000); //definir o valor de wrap
    pwm_set_enabled(slice, true); //habilita o pwm no slice correspondente
    pwm_set_gpio_level(PWM_SERVO_MOTOR,3000); // 180 graus duty cicle = 2.4/20 x 100 = 12% de 25000
    sleep_ms(5000);
    pwm_set_gpio_level(PWM_SERVO_MOTOR,1838); // 90 graus duty cicle  = 1.47/20 x 100 = 7.35% de 25000
    sleep_ms(5000);
    pwm_set_gpio_level(PWM_SERVO_MOTOR,625); // 0 graus duty cicle    = 500us(0.5ms)/20 x 100 = 2.5% de 25000
    sleep_ms(5000);
    
    //loop principal
    while (true) {
        mexe_servo_motor_de_0_a_180_e_reverso();
    }
}