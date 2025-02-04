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

int index_fora_do_coracao[10] = {2,6,8,10,14,15,17,19,21,23};
int index_dentro_coracao[6] = {7,11,12,13,16,18};
int index_cantos[4] = {0,4,20,24};
int index_restante[5] = {1,3,5,9,22};


const uint8_t colunas[4] = {19, 18, 17, 16}; // Pinos das colunas
const uint8_t linhas[4] = {26, 22, 21, 20};  // Pinos das linhas

const char teclado[4][4] = 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void configurar_teclado_matricial(){
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
                    sleep_ms(10); // Aguarda a tecla ser liberada
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


void desenha_parte_do_coracao(int indices[], int tamanho, int r, int g, int b) {
    for (int i = 0; i < tamanho; i++) {
        npSetLED(indices[i], r, g, b);
    }
    npWrite();
}

void preenche_todos_leds(int red, int green, int blue){
  for (int i = 0; i < 25; i++){
        npSetLED(i,red,green,blue);
      }
      npWrite();
}


void desenha_coracao_completo(int red_intensidade){
   for (int i = 0; i < 15; i++) {
      desenha_parte_do_coracao(index_dentro_coracao, 6, red_intensidade,0,0);   // Parte interna
      sleep_ms(200);
      npClear();
      desenha_parte_do_coracao(index_fora_do_coracao, 10, red_intensidade,0,0); // Parte externa
      sleep_ms(200);
      npClear();
      desenha_parte_do_coracao(index_restante,5,red_intensidade,0,0);
      sleep_ms(200);
      npClear();
      desenha_parte_do_coracao(index_cantos,4,red_intensidade,0,0);
      sleep_ms(200);
      npClear();
    }
}




int main() {

  // Inicializa entradas e saídas.
  // stdio_init_all();
  configurar_teclado_matricial();

  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();
  npWrite();
  while (true) {
    char tecla = leitura_teclado();
    
    if (tecla != 'n') // Tecla pressionada
    {
        if(tecla == '0'){
          //("Animação 1")
          desenha_coracao_completo(255);
        }

        else if(tecla == '1'){
          //("Animação 2");
        }

        else if(tecla == '2'){
          //("Animação 3");
        }

        else if(tecla == '3'){
          //("Animação 4");
        }
        
        else if(tecla == '4'){
          //("Animação 5");
        }

        else if(tecla == 'A'){
          // Todos os leds desligados
          npClear();
          npWrite();
        }

        else if(tecla == 'B'){
          //("Todos os leds ligados na cor Azul com 100%%");
          npClear();
          npWrite();
          preenche_todos_leds(0,0,255);
          
        }

        else if(tecla == 'C'){
          //("Todos os leds ligados na cor Vermelha com 80%%");
          npClear();
          npWrite();
          preenche_todos_leds(204,0,0);
        }

        else if(tecla == 'D'){
          //("Todos os leds ligados na cor verde com 50%%");
          npClear();
          npWrite();
          preenche_todos_leds(0,127,0);
        }

        else if(tecla == '#'){
          //("Todos os leds ligados na cor branca com 20%%");
          npClear();
          npWrite();
          preenche_todos_leds(51,51,51);
        }
        else if(tecla == '*'){
          // Habilita modo de gravação
          reset_usb_boot(0,0); //habilita o modo de gravação do microcontrolador
        }
    }
    sleep_ms(100); // Intervalo de tempo menor para uma leitura mais rápida
  }
}