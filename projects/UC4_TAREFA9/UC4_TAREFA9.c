#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

#include "pico/bootrom.h"


#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C // Endereço do display ssd1306
#define JOYSTICK_X_PIN 26  // GPIO para eixo X
#define JOYSTICK_Y_PIN 27  // GPIO para eixo Y
#define JOYSTICK_PUSHBUTTON 22 // GPIO para botão do Joystick
#define Botao_A 5 // GPIO para botão A
#define Botao_B 6// GPIO para botão B para entrar no modo de gravação
#define LED_GREEN_PIN 11                    
#define LED_BLUE_PIN 12                   
#define LED_RED_PIN 13                    
const float DIVIDER_PWM = 16.0;          // Divisor fracional do clock para o PWM
const uint16_t PERIOD = 4096;            // Período do PWM (valor máximo do contador)
uint16_t led_blue_level, led_red_level = 0; // Inicialização dos níveis de PWM para os LEDs
uint slice_led_blue, slice_led_red;           // Variáveis para armazenar os slices de PWM correspondentes aos LEDs
bool ultimo_estado_pwm_leds = true; // Variável para controlar o último estado de controle dos leds azul e vermelho por pwm
ssd1306_t ssd; // Inicializa a estrutura do display
static volatile int estado = 0; // Variável para controlar que borda do display está sendo mostrada ao apertar o botão do joystick
volatile uint32_t ultimo_tempo_buttons = 0;// Para armazenar o tempo da última interrupção acionada pelo botôes da interrupção
// Define um intervalo neutro menor para maior sensibilidade(váriaveis para indicar qual o centro do joystick para o adc)
const int centro_min = 1950;
const int centro_max = 2250;
volatile uint16_t adc_value_x = 0;
volatile uint16_t adc_value_y = 0; 

void gpio_irq_handler(uint gpio, uint32_t events){
  uint32_t tempo_atual = time_us_32() / 1000;  // Obtém o tempo atual em milissegundos e o armazena
  if (tempo_atual - ultimo_tempo_buttons < 350) return;// Se o tempo passado for menor que o atraso  de debounce(350s) retorne imediatamente
  ultimo_tempo_buttons = tempo_atual;// O tempo atual corresponde ao último tempo que o botão foi pressionado, ja que ele passou pela verificação acima
  if (gpio == Botao_B){
    reset_usb_boot(0, 0);// Entra no mode de gravação após a interrupção ser ativada pelo botão B
  }
  else if(gpio == JOYSTICK_PUSHBUTTON){// Muda o esta do led verde e muda entre sem borda, borda simples e dupla no display 1306
    gpio_put(LED_GREEN_PIN, !gpio_get(LED_GREEN_PIN));
    ssd1306_fill(&ssd, false); // Limpa o display
    switch (estado){
        case 0: // sem borda
          ssd1306_rect(&ssd, 3, 3, 122, 60, false, false);
        break;
        case 1: // Borda simples
          ssd1306_rect(&ssd,3,3,122,60,true,false);
        break;
        case 2: // Borda dupla
          ssd1306_rect(&ssd,3,3,122,60,true,false);
          ssd1306_rect(&ssd,4,4,120,58,true,false);
        break;
        
    }
    // Atualiza o estado para o próximo ciclo
    estado = (estado + 1) % 3;
    ssd1306_send_data(&ssd); // Atualiza o display
  }
  else if(gpio == Botao_A){ // Ativa o controle dos leds azul e vermelhor por pwm
    pwm_set_enabled(slice_led_blue,!ultimo_estado_pwm_leds);
    pwm_set_enabled(slice_led_red,!ultimo_estado_pwm_leds);
    ultimo_estado_pwm_leds = !ultimo_estado_pwm_leds;
  }
}


void init_gpios_and_adc_and_leds(){
  gpio_init(Botao_A);
  gpio_set_dir(Botao_A, GPIO_IN);
  gpio_pull_up(Botao_A);
  gpio_set_irq_enabled_with_callback(Botao_A,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);
  
  gpio_init(Botao_B);
  gpio_set_dir(Botao_B, GPIO_IN);
  gpio_pull_up(Botao_B);
  gpio_set_irq_enabled_with_callback(Botao_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  
  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN);
  adc_gpio_init(JOYSTICK_Y_PIN); 

  gpio_init(JOYSTICK_PUSHBUTTON);
  gpio_set_dir(JOYSTICK_PUSHBUTTON, GPIO_IN);
  gpio_pull_up(JOYSTICK_PUSHBUTTON); 
  gpio_set_irq_enabled_with_callback(JOYSTICK_PUSHBUTTON,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);

  gpio_init(LED_BLUE_PIN);
  gpio_init(LED_GREEN_PIN);
  gpio_init(LED_RED_PIN);
  gpio_set_dir(LED_BLUE_PIN,GPIO_OUT);
  gpio_set_dir(LED_RED_PIN,GPIO_OUT);
  gpio_set_dir(LED_GREEN_PIN,GPIO_OUT);

}

void init_i2c_and_display_ssd1306(){
    // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line

  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd); // Configura o display
  ssd1306_fill(&ssd, false);// Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_send_data(&ssd); // Envia os dados para o display

}

void joystick_read_axis(uint16_t adc_value_x, uint16_t adc_value_y) {
  adc_select_input(0); 
  sleep_us(2);
  adc_value_x = adc_read();

  adc_select_input(1); 
  sleep_us(2);
  adc_value_y = adc_read();
}


// Função para configurar o PWM de um LED (genérica para azul e vermelho)
void setup_pwm_led(uint led, uint *slice, uint16_t level)
{
  gpio_set_function(led, GPIO_FUNC_PWM); // Configura o pino do LED como saída PWM
  *slice = pwm_gpio_to_slice_num(led);   // Obtém o slice do PWM associado ao pino do LED
  pwm_set_clkdiv(*slice, DIVIDER_PWM);   // Define o divisor de clock do PWM
  pwm_set_wrap(*slice, PERIOD);          // Configura o valor máximo do contador (período do PWM)
  pwm_set_gpio_level(led, level);        // Define o nível inicial do PWM para o LED
  pwm_set_enabled(*slice, true);         // Habilita o PWM no slice correspondente ao LED
}

void apaga_ou_aumenta_leds_conforme_joystick(uint16_t adc_value_x, uint16_t adc_value_y, uint16_t intensidade_x, uint16_t intensidade_y){
  // Se o joystick estiver na posição neutra, desliga os LEDs
  if ((adc_value_x >= centro_min && adc_value_x <= centro_max) && (adc_value_y >= centro_min && adc_value_y <= centro_max)) {
      pwm_set_gpio_level(LED_RED_PIN, 0);  // Desliga o LED vermelho
      pwm_set_gpio_level(LED_BLUE_PIN, 0); // Desliga o LED azul
  } else {

      // Mapeia a intensidade de 0 a PERIOD para o LED vermelho (Eixo X)
      if (adc_value_x < centro_min) {
          intensidade_x = ((centro_min - adc_value_x) * PERIOD) / centro_min;
      } else if (adc_value_x > centro_max) {
          intensidade_x = ((adc_value_x - centro_max) * PERIOD) / (4095 - centro_max);
      }

      // Mapeia a intensidade de 0 a PERIOD para o LED azul (Eixo Y)
      if (adc_value_y < centro_min) {
          intensidade_y = ((centro_min - adc_value_y) * PERIOD) / centro_min;
      } else if (adc_value_y > centro_max) {
          intensidade_y = ((adc_value_y - centro_max) * PERIOD) / (4095 - centro_max);
      }

      // Limita ao máximo do PWM
      if (intensidade_x > PERIOD) intensidade_x = PERIOD;
      if (intensidade_y > PERIOD) intensidade_y = PERIOD;

      // Aplica os valores aos LEDs
      pwm_set_gpio_level(LED_RED_PIN, intensidade_y);
      pwm_set_gpio_level(LED_BLUE_PIN, intensidade_x);
    }
}


// void joystick_to_display(uint16_t adc_x, uint16_t adc_y) {
//   static int last_x = 93, last_y = 58; 

//   // Corrige a leitura dos eixos
//   int pos_x = (adc_x * (256 - 100)) / 4095; 
//   int pos_y = 110 - (adc_y * (110)) / 4095;


//   // Se a posição não mudou, não faz nada
//   if (pos_x == last_x && pos_y == last_y) return;

//   // Apaga o quadrado anterior
//   ssd1306_rect(&ssd, last_x, last_y, 8, 8, false, true);
  
//   // Desenha o novo quadrado
//   ssd1306_rect(&ssd, pos_x, pos_y, 8, 8, true, true);

//   // Atualiza o display
//   ssd1306_send_data(&ssd);

//   // Salva a nova posição
//   last_x = pos_x;
//   last_y = pos_y;
//   printf("Last x %d\n",last_x);
//   printf("Last y %d\n",last_y);
// }

void joystick_to_display(uint16_t adc_x, uint16_t adc_y) {
  static int last_x = 128, last_y = 64; // Começa no centro do display

  // Mapeia o ADC (0-4095) para a tela SSD1306 (0-127 para X e 0-63 para Y)
  int pos_x = (adc_x * 256) / 4095;  
  int pos_y = (adc_y * 128) / 4095;   


  // Se a posição não mudou, não redesenha
  if (pos_x == last_x && pos_y == last_y) return;

  // Apaga o quadrado anterior
  ssd1306_rect(&ssd, last_x, last_y, 8, 8, false, true);
  
  // Desenha o novo quadrado
  ssd1306_rect(&ssd, pos_x, pos_y, 8, 8, true, true);

  // Atualiza o display
  ssd1306_send_data(&ssd);

  // Salva a nova posição
  last_x = pos_x;
  last_y = pos_y;

  printf("X: %d, Y: %d\n", pos_x, pos_y);
}


int main()
{
    init_gpios_and_adc_and_leds();
    init_i2c_and_display_ssd1306();
    setup_pwm_led(LED_BLUE_PIN,&slice_led_blue,0);
    setup_pwm_led(LED_RED_PIN,&slice_led_red,0);
    stdio_init_all();

    while (true) {
      uint16_t intensidade_x = 0;
      uint16_t intensidade_y = 0;
      joystick_read_axis(adc_value_x, adc_value_y); 
      apaga_ou_aumenta_leds_conforme_joystick(adc_value_x,adc_value_y,intensidade_x,intensidade_y);
      joystick_to_display(adc_value_x,adc_value_y);
      sleep_ms(100);
    }
}
