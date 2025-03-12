#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "pico/bootrom.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDR 0x3C

#define BTN_A 5
#define BTN_B 6
#define JOY_B 22
#define JOY_X 26
#define JOY_Y 27
#define DEBOUNCE_DELAY_MS 500  // Tempo de debounce em milissegundos 

volatile uint32_t ultimo_tempo_buttons = 0;  // Para armazenar o tempo da última interrupção acionada pelos botões


ssd1306_t oled;

char board[8][8] = {
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}
};

// Torre branca
const uint8_t rook_white[8] = {
    0b00111000,
    0b00101000,
    0b00111000,
    0b00111000,
    0b00111000,
    0b00111000,
    0b00111000,
    0b00000000
};

// Torre preta
const uint8_t rook_black[8] = {
    0b00000000,
    0b00111000,
    0b00111000,
    0b00111000,
    0b00111000,
    0b00111000,
    0b00101000,
    0b00111000
};

// Cavalo branco
const uint8_t knight_white[8] = {
    0b00011000,
    0b00111000,
    0b00110000,
    0b00111000,
    0b00011100,
    0b00001100,
    0b00001100,
    0b00000000
};

// Cavalo preto
const uint8_t knight_black[8] = {
    0b00000000,
    0b00001100,
    0b00001100,
    0b00011100,
    0b00111000,
    0b00110000,
    0b00111000,
    0b00011000
};

// Bispo branco
const uint8_t bishop_white[8] = {
    0b00001000,
    0b00011000,
    0b00011000,
    0b00001000,
    0b00111100,
    0b00111100,
    0b00011000,
    0b00000000
};

// Bispo preto
const uint8_t bishop_black[8] = {
    0b00000000,
    0b00011000,
    0b00111100,
    0b00111100,
    0b00001000,
    0b00011000,
    0b00011000,
    0b00001000
};

// Rainha branca
const uint8_t queen_white[8] = {
    0b00011000,
    0b00101100,
    0b00111000,
    0b00011000,
    0b00111100,
    0b00111100,
    0b00111100,
    0b00000000
};

// Rainha preta
const uint8_t queen_black[8] = {
    0b00000000,
    0b00111100,
    0b00111100,
    0b00111100,
    0b00011000,
    0b00111000,
    0b00101100,
    0b00011000
};

// Rei branco
const uint8_t king_white[8] = {
    0b00011000,
    0b00011000,
    0b00111100,
    0b00011000,
    0b00111100,
    0b00111100,
    0b00111100,
    0b00000000
};

// Rei preto
const uint8_t king_black[8] = {
    0b00000000,
    0b00111100,
    0b00111100,
    0b00111100,
    0b00011000,
    0b00111100,
    0b00011000,
    0b00011000
};

// Peão branco
const uint8_t pawn_white[8] = {
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00111100,
    0b00111100,
    0b00000000
};

// Peão preto
const uint8_t pawn_black[8] = {
    0b00000000,
    0b00111100,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000
};


void init_display() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&oled, 128, 64, false, OLED_ADDR, I2C_PORT);
    ssd1306_fill(&oled, false);
    ssd1306_send_data(&oled);
}
const uint8_t* get_piece_bitmap(char piece) {
    switch (piece) {
        case 'P': return pawn_white;
        case 'p': return pawn_black;
        case 'R': return rook_white;
        case 'r': return rook_black;
        case 'N': return knight_white;
        case 'n': return knight_black;
        case 'B': return bishop_white;
        case 'b': return bishop_black;
        case 'Q': return queen_white;
        case 'q': return queen_black;
        case 'K': return king_white;
        case 'k': return king_black;
        default: return NULL;
    }
}
void draw_bitmap(ssd1306_t *disp, int x, int y, const uint8_t *bitmap) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            bool pixel = (bitmap[row] >> (7 - col)) & 1;  // Extrai o bit correto
            ssd1306_pixel(&oled, x + col, y + row, pixel);
        }
    }
}


void draw_board() {
    ssd1306_fill(&oled, false);

    int square_size = 8;  
    int offset_x = 32;  // Centraliza corretamente
    int offset_y = 32;  // Ajuste fino na centralização vertical

    // Desenhar os quadrados do tabuleiro
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int x_pos = offset_x + x * square_size;
            int y_pos = offset_y + y * square_size;
            bool is_white_square = (x + y) % 2 == 0;
            
            // Desenha o quadrado do tabuleiro
            ssd1306_rect(&oled, x_pos, y_pos, square_size, square_size, true, is_white_square);

            char piece = board[y][x];
            const uint8_t* bitmap = get_piece_bitmap(piece);
            if (bitmap) {
                draw_bitmap(&oled, x_pos, y_pos, bitmap);
            }
        }
    }
    ssd1306_send_data(&oled);
}


void gpio_irq_handler(uint gpio, uint events){
    uint32_t tempo_atual = time_us_32() / 1000;  // Obtém o tempo atual em milissegundos e o armazena
    // Se o tempo passado for menor que o atraso  de debounce(500ms) retorne imediatamente
    if (tempo_atual - ultimo_tempo_buttons < DEBOUNCE_DELAY_MS) return;

    if (gpio == JOY_B)
    {
        // Interrupção para habilitar o modo de gravação do microcontrolador
        reset_usb_boot(0,0);
    }
}

void init_gpios(){
    gpio_init(BTN_A);
    gpio_init(BTN_B);
    gpio_init(JOY_B);
    gpio_set_dir(BTN_A,GPIO_IN);
    gpio_set_dir(BTN_B,GPIO_IN);
    gpio_set_dir(JOY_B,GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_pull_up(BTN_B);
    gpio_pull_up(JOY_B);
    gpio_set_irq_enabled_with_callback(JOY_B,GPIO_IRQ_EDGE_FALL,true,&gpio_irq_handler);
}


void game_loop() {
    while (1) {
        draw_board();
        sleep_ms(500);
    }
}

int main() {
    stdio_init_all();
    init_gpios();
    init_display();
    game_loop();
    return 0;
}
