#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/bootrom.h"

// Biblioteca gerada pelo arquivo .pio durante compilação.
#include "ws2818b.pio.h"

// Definição do número de LEDs e pino.
#define LED_COUNT 25
#define LED_PIN 7
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTICK 22 
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12

                                            // index 9
int index_crux[] = {2,7,10,11,12,13,14,17,22,0,1,3,4,5,6,8,9,15,16,18,19,20,21,23,24}; // tamanho 25
int index_x[] =  {0,1,3,4,5,6,7,8,9,11,12,13,15,16,17,18,19,20,21,23,24}; // tamanho 21


// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}


void inicializar_tudo(){
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A,GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B,GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(BUTTON_JOYSTICK);
    gpio_set_dir(BUTTON_JOYSTICK,GPIO_IN);
    gpio_pull_up(BUTTON_JOYSTICK);

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED,GPIO_OUT);
    gpio_put(LED_RED,false);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN,GPIO_OUT);
    gpio_put(LED_GREEN,false);

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE,GPIO_OUT);
    gpio_put(LED_BLUE,false);
}

void desenhar_na_matriz(int posicoes[], int tamanho_posicoes, int red, int green, int blue){
    for (int i = 0; i < tamanho_posicoes; i++)
    {
        npSetLED(posicoes[i],red,green,blue);
    }
    npWrite();
    
}

void desenhar_Cruz(){
    for (int i = 0; i < 25; i++){
        if (i < 9) {
            npSetLED(index_crux[i],255,0,0);
        }
        else
        {
            npSetLED(index_crux[i],0,0,255);
        }
        
    }
    npWrite();
}

void desenhar_X(){
     for (int i = 0; i < 21; i++){
        if (index_x[i] % 2 == 0 ){
            npSetLED(index_x[i],255,0,0);
        }
        else{
            npSetLED(index_x[i],0,0,255);
        }
    }
    npWrite();
}

void apagar_leds_de_cima_para_baixo(){
     for (int i = 24; i >= 0; i--){
        npSetLED(i,0,0,0);
        npWrite();
        sleep_ms(75);
    }
}


int main()
{
    stdio_init_all();
    inicializar_tudo();
     // Inicializa matriz de LEDs NeoPixel.
    npInit(LED_PIN);
    npClear();
    npWrite();

  
    while (true) {
        if (gpio_get(BUTTON_JOYSTICK) == 0){
            reset_usb_boot(0,0);
        }
        else if (gpio_get(BUTTON_A) == 0){
            for (int i = 0; i < 25; i++){
                
                // if (i >= 1)
                // {
                //     npSetLED(i-1,0,0,0);
                // }
                npSetLED(i,255,255,255);
                npWrite();
                sleep_ms(75);
            }
            // for (int i = 0; i < 15; i++){
            //     npClear();            
            //     desenhar_Cruz();
            //     sleep_ms(300);
            //     npClear();
            //     desenhar_X();
            //     sleep_ms(300);
            // }
            
        }
        else if(gpio_get(BUTTON_B) == 0){
           apagar_leds_de_cima_para_baixo();
        }        
        
    }
}
