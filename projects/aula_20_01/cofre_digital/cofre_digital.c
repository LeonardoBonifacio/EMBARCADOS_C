#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN_RED 13
#define LED_PIN_GREEN 11

const uint8_t colunas[4] = {19, 18, 17, 16}; // Pinos das colunas
const uint8_t linhas[4] = {26, 22, 21, 20};  // Pinos das linhas
const char senha_armazenada[4] = {'1', '2', '3', '4'}; // Senha para testes
char senha_digitada[4];

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
    for (int i = 0; i < 2; i++)
    {
        gpio_init(display_pins[i]);
        gpio_set_dir(display_pins[i], GPIO_OUT);
        gpio_put(display_pins[i], 1); // Desativa inicialmente os displays
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
}

// Função para exibir um número em um display específico
void show_digit(uint8_t display, uint8_t digit){
    // Desativa ambos displays(Catodo comum é desativado com nivel alto)
    gpio_put(display_pins[0],1);
    gpio_put(display_pins[1],1);


    // Ativa os segmentos correspondentes ao dígito
    for (int  i = 0; i < 7; i++)
    {
        gpio_put(segment_pins[i], digits[digit][i]);
    }

    // Ativa o display desejado
    gpio_put(display_pins[display],0);
    
    
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
        // Ativa a coluna atual (coloca o pino da coluna como 1)
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

        // Desativa a coluna atual (coloca o pino da coluna como 0)
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

    while (true) {
        char tecla = leitura_teclado(); 
        if (tecla != 'n')
        {
             printf("A tecla digitada foi %c\n", tecla);
        }
        sleep_ms(100);
        
        
    }
}
