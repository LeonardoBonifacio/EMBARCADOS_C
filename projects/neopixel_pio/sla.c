#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

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

int index_fora_do_coracao[10] = {0,0,0,0,0,0,0,0,0,0};
int index_dentro_coracao[6] = {0,0,0,0,0,0};
int index_cantos[4] = {0,0,0,0};
int index_restante[5] = {0,0,0,0,0};
int todos_index[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int colunas_fora_coracao[] = {2,1,3,0,4,0,2,4,1,3} ; //COLUNAS
int linhas_fora_coracao[] = {0,1,1,2,2,3,3,3,4,4} ; // LINHAS
int colunas_dentro_coracao[] = {2,1,2,3,1,3};
int linhas_dentro_coracao[] = {1,2,2,2,3,3};
int colunas_canto[] = {0,0,4,4};
int linhas_canto[] = {0,4,0,4};
int colunas_restante[] = {0,1,3,4,2};
int linhas_restante[] = {1,0,0,1,4};
int todas_colunas[] = {0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4,0,1,2,3,4};
int todas_linhas[] = {0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4};

const uint8_t colunas[4] = {19, 18, 17, 16}; // Pinos das colunas
const uint8_t linhas[4] = {26, 22, 21, 20};  // Pinos das linhas

const char teclado[4][4] = 
{
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};


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


/**
 * Converte as coordenadas (x, y) na matriz 5x5 para o índice da fila linear.
 * 
 * @param x A coluna (0 a 4).
 * @param y A linha (0 a 4).
 * @return O índice correspondente na fila (0 a 24).
 */
int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return y * 5 + x; // Linha par (esquerda para direita).
    } else {
        return y * 5 + (4 - x); // Linha ímpar (direita para esquerda).
    }
}

void desenha_parte_coracao(int indices[], int tamanho, int r, int g, int b) {
    for (int i = 0; i < tamanho; i++) {
        npSetLED(indices[i], r, g, b);
    }
    npWrite();
}

void desenha_parte_coracao_pulsante(int indices[], int tamanho, int r, int g, int b, int pulsos, int delay_ms) {
    for (int p = 0; p < pulsos; p++) {
        // Aumenta a intensidade (fade in)
        for (int intensidade = 0; intensidade <= 255; intensidade += 5) {
            for (int i = 0; i < tamanho; i++) {
                npSetLED(indices[i], (r * intensidade) / 255, (g * intensidade) / 255, (b * intensidade) / 255);
            }
            npWrite();
            sleep_ms(delay_ms);
        }
        // Diminui a intensidade (fade out)
        for (int intensidade = 255; intensidade >= 0; intensidade -= 5) {
            for (int i = 0; i < tamanho; i++) {
                npSetLED(indices[i], (r * intensidade) / 255, (g * intensidade) / 255, (b * intensidade) / 255);
            }
            npWrite();
            sleep_ms(delay_ms);
        }
    }
}

void desenha_coracao_completo(){
   for (int i = 0; i < 5; i++) {
      desenha_coracao(index_dentro_coracao, 6, 250, 0, 0);   // Parte interna
      sleep_ms(300);
      desenha_coracao(index_fora_do_coracao, 10, 250, 0, 0); // Parte externa
      sleep_ms(300);
      npClear();
      desenha_coracao(index_restante,5,250,0,0);
      sleep_ms(150);
      npClear();
      desenha_coracao(index_cantos,4,250,0,0);
      sleep_ms(150);
      npClear();
      desenha_coracao(todos_index,25,250,0,0);
      sleep_ms(150);
      npClear();
    }
}

void gerar_index_desenho(){
   for (int i = 0; i < 25; i++){
      todos_index[i] = getIndex(todas_colunas[i],todas_linhas[i]);

      if (i < 10){
      index_fora_do_coracao[i] = getIndex(colunas_fora_coracao[i],linhas_fora_coracao[i]);
        
      }
    
      if (i < 6) {
            index_dentro_coracao[i] = getIndex(colunas_dentro_coracao[i], linhas_dentro_coracao[i]);
      }

      if (i < 4){
        index_cantos[i] = getIndex(colunas_canto[i],linhas_canto[i]);
      }

      if (i < 5){
        index_restante[i] = getIndex(colunas_restante[i],linhas_restante[i]);
      }   
  }
}


int main() {

  // Inicializa entradas e saídas.
  stdio_init_all();

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



  // Inicializa matriz de LEDs NeoPixel.
  npInit(LED_PIN);
  npClear();
  gerar_index_desenho();
      
  // Não faz mais nada. Loop infinito.
  while (true) {
     desenha_coracao_completo();
  }
}
  
