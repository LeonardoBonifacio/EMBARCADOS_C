#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"

#define BUZZER 21
#define LEDS 25
#define PINO_LEDS 7
#define NUMERO_PIXELS 64

uint32_t definir_cor_led(double intensidade_azul, double intensidade_vermelha, double intensidade_verde);

//Pinos das linhas e colunas do teclado matricial 4x4
uint colunas[4] = {4, 3, 2, 28};
uint linhas[4] = {8, 11, 6, 5};

//Variáveis auxiliares
uint colunas_aux[4];
uint linhas_aux[4];
char matriz_teclado[16];

//Armazena uma máscara que representa todos os pinos das colunas do teclado matricial, montada com os bits correspondentes a cada coluna ativada.
uint mascara_todas_colunas = 0x0;

//Armazena uma máscara individual para cada coluna do teclado matricial.
uint mascara_coluna[4];

//Mapa para indentificar os símbolos das teclas
char MAPA_TECLAS[16] = {
    '1', '2', '3', 'A',
    '4', '5', '6', 'B',
    '7', '8', '9', 'C',
    '*', '0', '#', 'D'
};


//Frames da animação 0
double desenho_1[25] = {
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0 };
double desenho_2[25] = {
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0 };
double desenho_3[25] = {
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0 };           
double desenho_4[25] = {
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0 };
double desenho_5[25] = {
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 0.0, 1.0, 1.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 1.0, 0.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0 };


// Frames da Animação 1
double desenho_6[25] = {
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0 };

double desenho_7[25] = {
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        1.0, 0.0, 1.0, 0.0, 1.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0 };


double desenho_8[25] = {
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0 };


double desenho_9[25] = {
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 0.0, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0 };


double desenho_10[25] = {
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0 };

//Frames da animação 2

double desenho_11[25] = {
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0, 
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0 };

double desenho_12[25] = {
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        1.0, 0.0, 1.0, 0.0, 1.0, 
                        1.0, 0.0, 1.0, 0.0, 1.0,
                        1.0, 0.0, 1.0, 0.0, 1.0,
                        0.0, 1.0, 1.0, 1.0, 0.0 };

double desenho_13[25] = {
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        1.0, 0.0, 1.0, 0.0, 1.0, 
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 0.0, 1.0, 0.0, 1.0,
                        0.0, 1.0, 1.0, 1.0, 0.0 };
                        
double desenho_14[25] = {
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0 };

double desenho_15[25] = {
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0, 
                        1.0, 0.0, 1.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0 };


double desenho_16[25] = {
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        1.0, 0.0, 1.0, 0.0, 1.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0};
                        
double desenho_17[25] = {
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        1.0, 0.0, 1.0, 0.0, 1.0,
                        0.0, 1.0, 1.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0};

double desenho_18[25] = {
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0};

double desenho_19[25] = {
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0};

double desenho_20[25] = {
                        1.0, 0.0, 0.0, 0.0, 1.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 1.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 1.0, 0.0,
                        1.0, 0.0, 0.0, 0.0, 1.0};


//Inicializa o teclado matricial
void inicializar_teclado(uint colunas[4], uint linhas[4], char valores_matriz[16]) {
    for (int i = 0; i < 16; i++) {
        matriz_teclado[i] = valores_matriz[i];
    }

    for (int i = 0; i < 4; i++) {
        colunas_aux[i] = colunas[i];
        linhas_aux[i] = linhas[i];

        gpio_init(colunas_aux[i]);
        gpio_init(linhas_aux[i]);
        gpio_set_dir(colunas_aux[i], GPIO_IN);
        gpio_set_dir(linhas_aux[i], GPIO_OUT);
        gpio_put(linhas_aux[i], 1);

        mascara_todas_colunas += (1 << colunas_aux[i]);
        mascara_coluna[i] = 1 << colunas_aux[i];
    }
}

//Obtém a tecla pressionada
char obter_tecla() {
    int linha;
    uint32_t colunas_ativas;
    colunas_ativas = gpio_get_all() & mascara_todas_colunas;

    if (colunas_ativas == 0x0) {
        return 0;
    }

    for (int j = 0; j < 4; j++) {
        gpio_put(linhas_aux[j], 0);
    }

    for (linha = 0; linha < 4; linha++) {
        gpio_put(linhas_aux[linha], 1);
        busy_wait_us(10000);
        colunas_ativas = gpio_get_all() & mascara_todas_colunas;
        gpio_put(linhas_aux[linha], 0);
        if (colunas_ativas != 0x0) {
            break;
        }
    }

    for (int i = 0; i < 4; i++) {
        gpio_put(linhas_aux[i], 1);
    }

    for (int i = 0; i < 4; i++) {
        if (colunas_ativas == mascara_coluna[i]) {
            return (char) matriz_teclado[linha * 4 + i];
        }
    }
    return 0;
}

//Entra no modo bootsel
void modo_bootsel(){
    reset_usb_boot(0,0);
}

//Apaga todos os leds
void clear(PIO pio, uint sm) {
    uint32_t cor_apagada = definir_cor_led(0, 0, 0);
    for (int i = 0; i < NUMERO_PIXELS; i++){
        pio_sm_put_blocking(pio, sm, cor_apagada); 
    }
}

// Define a intensidade das cores de um LED RGB
uint32_t definir_cor_led(double intensidade_azul, double intensidade_vermelha, double intensidade_verde) {
    unsigned char vermelho, verde, azul;

    vermelho = intensidade_vermelha * 255;
    verde = intensidade_verde * 255;
    azul = intensidade_azul * 255;

    return (verde << 24) | (vermelho << 16) | (azul << 8);
}

// Ativa a matriz de LEDs com cor vermelha
void acionar_matriz_leds(double r, double b, double g, double desenho[], uint32_t valor_led, PIO pio, uint sm) {
    for (int16_t i = 0; i < LEDS; i++) {      
        if(r == 1.0){
            valor_led = definir_cor_led(b, desenho[24-i], g); 
            pio_sm_put_blocking(pio, sm, valor_led);
        }else if (b == 1.0){
            valor_led = definir_cor_led(desenho[24-i], r, g); 
            pio_sm_put_blocking(pio, sm, valor_led);
        }else if(g == 1.0){
            valor_led = definir_cor_led(b, r, desenho[24-i]); 
            pio_sm_put_blocking(pio, sm, valor_led);
        } 
    }
}


//Ligar leds azúis em 100%
void ligar_leds_azul(uint32_t valor_led, PIO pio, uint sm){
    for (int16_t i = 0; i < LEDS; i++) {      
        valor_led = definir_cor_led(1.0, 0.0, 0.0); 
        pio_sm_put_blocking(pio, sm, valor_led);
    }  
}

//Ligar leds vermelhos em 80%
void ligar_leds_vermelho(uint32_t valor_led, PIO pio, uint sm){
    for (int16_t i = 0; i < LEDS; i++) {      
        valor_led = definir_cor_led(0.0, 0.8, 0.0); 
        pio_sm_put_blocking(pio, sm, valor_led);
    }  
}

//Ligar leds verdes em 50%
void ligar_leds_verde(uint32_t valor_led, PIO pio, uint sm){
    for (int16_t i = 0; i < LEDS; i++) {      
        valor_led = definir_cor_led(0.0, 0.0, 0.5); 
        pio_sm_put_blocking(pio, sm, valor_led);
    }  
}

//Ligar leds brancos em 20%
void ligar_leds_branco(uint32_t valor_led, PIO pio, uint sm){
    for (int16_t i = 0; i < LEDS; i++) {      
        valor_led = definir_cor_led(0.2, 0.2, 0.2); 
        pio_sm_put_blocking(pio, sm, valor_led);
    }  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(){
    //Inicializa os recursos do teclado
    stdio_init_all();
    inicializar_teclado(colunas, linhas, MAPA_TECLAS);
    char tecla; 

    //Váriaveis para o PIO
    PIO pio = pio0;
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 1.0, b = 0.0 , g = 0.0;

    //Configurações
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, PINO_LEDS);

    while (true){
        tecla = obter_tecla();

        if(tecla == '0'){
            // Animação 1
            for (int i = 0; i < 2; i++)
            {
                acionar_matriz_leds(r, b, g, desenho_1, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_2, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_1, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_2, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_3, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_2, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_3, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_2, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_1, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_4, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_1, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_4, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_1, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_5, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_1, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_5, valor_led, pio, sm);
                sleep_ms(200);
            }
            
        }else if(tecla == '1'){
            //Animação 2
            for (int i = 0; i < 5; i++)
            {
                acionar_matriz_leds(r,g,b,desenho_6,valor_led,pio,sm);
                sleep_ms(200);
                acionar_matriz_leds(r,g,b,desenho_7,valor_led,pio,sm);
                sleep_ms(300);
                acionar_matriz_leds(r,g,b,desenho_8,valor_led,pio,sm);
                sleep_ms(200);
                acionar_matriz_leds(r,g,b,desenho_9,valor_led,pio,sm);
                sleep_ms(200);
                acionar_matriz_leds(r,g,b,desenho_10,valor_led,pio,sm);
                sleep_ms(200);
            }
            
        }else if(tecla == '2'){
            //Animação 3
            for (int i = 0; i < 5; i++)
            {
                acionar_matriz_leds(r,g,b,desenho_11,valor_led,pio,sm);
                sleep_ms(200);
                acionar_matriz_leds(r,g,b,desenho_12,valor_led,pio,sm);
                sleep_ms(300);
                acionar_matriz_leds(r,g,b,desenho_13,valor_led,pio,sm);
                sleep_ms(200);
                acionar_matriz_leds(r,g,b,desenho_14,valor_led,pio,sm);
                sleep_ms(200);
                acionar_matriz_leds(r,g,b,desenho_15,valor_led,pio,sm);
                sleep_ms(200);
            }
        }else if(tecla == '3'){
            //Animação 4
             for (int i = 0; i < 5; i++) {
                acionar_matriz_leds(r, b, g, desenho_16, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_17, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_18, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_19, valor_led, pio, sm);
                sleep_ms(200);
                acionar_matriz_leds(r, b, g, desenho_20, valor_led, pio, sm);
                sleep_ms(200);
            }
        }else if(tecla == '4'){
            //Animação 5
        }else if(tecla == 'A'){
            clear(pio, sm);
        }else if(tecla == 'B'){
            ligar_leds_azul(valor_led, pio, sm);
        }else if(tecla == 'C'){
            ligar_leds_vermelho(valor_led, pio, sm);
        }else if(tecla == 'D'){
            ligar_leds_verde(valor_led, pio, sm);
        }else if(tecla == '#'){
            ligar_leds_branco(valor_led, pio, sm);
        }else if(tecla == '*'){ 
            modo_bootsel();
        }
        sleep_ms(200);
    }
}