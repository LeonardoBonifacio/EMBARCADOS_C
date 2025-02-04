#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"


#define LED_PIN_RED 13
#define LED_PIN_GREEN 11
#define BUZZER_PIN 15
#define BUZZER_FREQUENCY 5000

const uint8_t colunas[4] = {19, 18, 17, 16}; // Pinos das colunas
const uint8_t linhas[4] = {26, 22, 21, 20};  // Pinos das linhas
char senha_do_cofre[4] = {'1', '2', '3', '4'}; // Senha para testes
char tentativa_de_senha[4];
int contador_tentativas = 0;
int qtd_num_digitados = 0;
int qtd_num_digitados_corretos = 0;
int milhar = 0;
int centena = 0;
int dezena = 0;
int unidade = 0;

const char teclado[4][4] = 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

const uint8_t segment_pins[] = {0, 1, 2, 3, 4, 5, 6}; // Pinos dos segmetos a,b,c,d,e,f,g
const uint8_t display_pins[] = {7, 8, 9, 10}; // Pinos de controle dos displays (Dig1 e Dig2) 


// Mapeamento dos digitos para os segmentos do display (0 = Desligado , 1 = ligado)
const uint8_t digits [10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 0, 0, 1, 1}, // 9
};



// Função para configurar todos os pinos do display de 7 segmentos como saída
void setup_display(){
    for (int i = 0; i < 7; i++)
    {
        gpio_init(segment_pins[i]);
        gpio_set_dir(segment_pins[i], GPIO_OUT);
    }
    for (int i = 0; i < 4; i++)
    {
        gpio_init(display_pins[i]);
        gpio_set_dir(display_pins[i], GPIO_OUT);
        gpio_put(display_pins[i], 1); // Desativa inicialmente os displays
    }
    
}

void clear_displays() {
    // Desativa todos os displays
    for (int i = 0; i < 4; i++) {
        gpio_put(display_pins[i], 1);
    }
    // Desativa todos os segmentos
    for (int i = 0; i < 7; i++) {
        gpio_put(segment_pins[i], 0);
    }
}


void setup_teclado_matricial(){
     // Configuração dos pinos das colunas como saídas digitais
    for (int i = 0; i < 4; i++)
    {
        gpio_init(colunas[i]);
        gpio_set_dir(colunas[i], GPIO_OUT);
        gpio_put(colunas[i], 1); // Inicializa todas as colunas como baixo
    }

    // Configuração dos pinos das linhas como entradas digitais
    for (int i = 0; i < 4; i++)
    {
        gpio_init(linhas[i]);
        gpio_set_dir(linhas[i], GPIO_IN);
        gpio_pull_up(linhas[i]); // Habilita pull-up para as linhas
    }
}

void setup_leds(){
    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_GREEN);

    gpio_set_dir(LED_PIN_RED,GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN,GPIO_OUT);

    gpio_put(LED_PIN_GREEN,false);
    gpio_put(LED_PIN_RED,false);
}

// Função para exibir um número em um display específico
void show_digit(uint8_t display, uint8_t digit){
    // Desativa ambos displays(Catodo comum é desativado com nivel alto)
    gpio_put(display_pins[0],1);
    gpio_put(display_pins[1],1);
    gpio_put(display_pins[2],1);
    gpio_put(display_pins[3],1);


    // Ativa os segmentos correspondentes ao dígito
    for (int  i = 0; i < 7; i++)
    {
        gpio_put(segment_pins[i], digits[digit][i]);
    }

    // Ativa o display desejado
    gpio_put(display_pins[display],0);
    
    
}

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Divisor de clock
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin, 0);
}

// Definição de uma função para emitir um beep com duração especificada
void beep_erro(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

void setup_buzzer(){
    // Configuração do GPIO para o buzzer como saída
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    // Inicializar o PWM no pino do buzzer
    pwm_init_buzzer(BUZZER_PIN);
}

void contar_regressivamente_30s(){
    int dezenas_contagem_regressiva = 0;
    int unidades_contagem_regressiva = 0;
    for (uint8_t numero = 30; numero > 0; numero--){
        dezenas_contagem_regressiva = numero / 10; //  Calcula o dígito das dezenas
        unidades_contagem_regressiva = numero % 10; // Calcula o dígito das unidades

        // Multiplexa rapidamente entre os displays
        for (int j = 0; j < 25; j++) {// Ajusta o valor de j para controlar a velocidade de atualização 
            show_digit(2,dezenas_contagem_regressiva); // Exibe o dígito das dezenas no primeiro display
            sleep_ms(5); // Tempo de permanência no primeiro display
            show_digit(3, unidades_contagem_regressiva); // Exibe o dígito das unidades no segundo display
            sleep_ms(5); // Tempo de permanência no segundo display
        }
        sleep_ms(700); 
    }
}

void bloquear_cofre_por_30s(){
    for (int i = 0; i < 50; i++)
    {
        show_digit(0,0);
        sleep_ms(5);
        show_digit(1,0);
        sleep_ms(5);
        show_digit(2,0);
        sleep_ms(5);
        show_digit(3,0);
        sleep_ms(5);
    }
    beep_erro(BUZZER_PIN,3000);
    contar_regressivamente_30s();
}

void resetar_senha_e_variaveis_de_controle(){
    for (int i = 0; i < 4; i++) {
        tentativa_de_senha[i] = '\0';
    }
    qtd_num_digitados_corretos = qtd_num_digitados = 0;
    milhar = centena = dezena = unidade = 0;
}

void verificar_senha(){
    for (int i = 0; i < 4; i++){
        if (senha_do_cofre[i] == tentativa_de_senha[i])
        {
            qtd_num_digitados_corretos++;
        }
    }
}

void piscar_led_apos_verificar_senha(){
    if (qtd_num_digitados_corretos == 4){
        gpio_put(LED_PIN_GREEN,true);
        sleep_ms(1000);
        gpio_put(LED_PIN_GREEN,false);
    }
    else{
        beep_erro(BUZZER_PIN,500);
        gpio_put(LED_PIN_RED,true);
        sleep_ms(1000);
        gpio_put(LED_PIN_RED,false);
        contador_tentativas++;
    }
}

// Função para ler o teclado matricial
char leitura_teclado()
{   
    char numero = 'n'; // Valor padrão para quando nenhuma tecla for pressionada

    // Desliga todos os pinos das colunas
    for (int i = 0; i < 4; i++)
    {
        gpio_put(colunas[i], 1);
    }

    for (int coluna = 0; coluna < 4; coluna++)
    {
        // Ativa a coluna atual (coloca o pino da coluna como 0)
        gpio_put(colunas[coluna], 0);

        for (int linha = 0; linha < 4; linha++)
        {
            // Verifica o estado da linha. Se estiver em 0, a tecla foi pressionada
            if (gpio_get(linhas[linha]) == 0)
            {
                numero = teclado[linha][coluna]; // Mapeia a tecla pressionada
                // Aguarda a tecla ser liberada (debounce)
                while (gpio_get(linhas[linha]) == 0)
                {
                    sleep_ms(20); // Aguarda a tecla ser liberada
                }
                break; // Sai do laço após detectar a tecla
            }
        }

        // Desativa a coluna atual (coloca o pino da coluna como 1)
        gpio_put(colunas[coluna], 1);

        if (numero != 'n') // Se uma tecla foi pressionada, sai do laço de colunas
        {
            break;
        }
    }

    return numero; // Retorna a tecla pressionada
}

int main()
{
    stdio_init_all();
    setup_display();
    setup_teclado_matricial();
    setup_leds();
    setup_buzzer();
    // TO DO Adicionar um lcd para emitir mensagem de configuração de senha e fazer uma configuração de senha manual
    // Melhorar a logica da multiplexação dos displays e implementar interrupções com temporizadores
    while (true) {
        char tecla = leitura_teclado(); 
        if (tecla >= '0' && tecla <= '9')
        {
            switch (qtd_num_digitados)
            {
            case 0:
                milhar = tecla - '0';
                break;
            case 1:
                centena = tecla - '0';
                break;
            case 2:
                dezena = tecla - '0';
                break;
            case 3:
                unidade = tecla - '0';
            default:
                break;
            }
            for (int i = 0; i < 50; i++) {
                show_digit(0, milhar);
                sleep_ms(5); 
                show_digit(1, centena);
                sleep_ms(5); 
                show_digit(2, dezena);
                sleep_ms(5);
                show_digit(3, unidade);
                sleep_ms(5);
            }

            tentativa_de_senha[qtd_num_digitados] = tecla;
            qtd_num_digitados++; 
          
            if (qtd_num_digitados == 4){
                verificar_senha();
                clear_displays();
                piscar_led_apos_verificar_senha();

                if (contador_tentativas == 3){
                    contador_tentativas = 0;
                    bloquear_cofre_por_30s();
                    clear_displays();
                }
                resetar_senha_e_variaveis_de_controle();
                
                
            }
        }
        sleep_ms(100);   
    }
}
