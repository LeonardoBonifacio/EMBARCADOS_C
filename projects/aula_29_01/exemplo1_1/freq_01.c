#include <stdio.h> //Biblioteca padrão do C
#include "pico/stdlib.h"     // Biblioteca padrão para funções básicas de E/S, temporização, controle de GPIO, etc.
#include "hardware/clocks.h" // Biblioteca para configuração e gerenciamento dos clocks do microcontrolador RP2040.
#include "hardware/pll.h"    // Biblioteca para controle e configuração dos Phase-Locked Loops (PLLs).

//função para obter as frequências de clock presentes no rp2040 
void measure_freqs(void) {
    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

    printf("pll_sys  = %dkHz\n", f_pll_sys);
    printf("pll_usb  = %dkHz\n", f_pll_usb);
    printf("rosc     = %dkHz\n", f_rosc);
    printf("clk_sys  = %dkHz\n", f_clk_sys);
    printf("clk_peri = %dkHz\n", f_clk_peri);
    printf("clk_usb  = %dkHz\n", f_clk_usb);
    printf("clk_adc  = %dkHz\n", f_clk_adc);
    printf("clk_rtc  = %dkHz\n", f_clk_rtc);
}

int main() //função principal
{
    // Inicializa a comunicação serial, permitindo o uso das funções padrão de E/S como printf.
    stdio_init_all();

    //Configura a frequência do clock do sistema (clk_sys) para um valor específico em kHz
    set_sys_clock_khz(100000, false);

    // Configura o PLL_SYS (PLL do sistema) para gerar uma frequência de 125 MHz.
    // Parâmetros:
    // pll_sys: PLL do sistema a ser configurado.
    // 1: Pós-divisor de saída, que controla o número de fases. Neste caso, 1 para uma única fase.
    // 1500 * MHZ: Frequência alvo do PLL (1500 MHz).
    // 6 * MHZ: Frequência de referência de entrada do PLL, derivada do cristal externo de 12 MHz, dividida por 2.
    // 1: Pré-divisor de entrada do PLL, que divide a frequência de entrada por 1 (sem alteração).
    //pll_init(pll_sys, 1, 1500 * MHZ, 6 * MHZ, 1);

    // Configura o clock do sistema (CLK_SYS) para usar o PLL_SYS como fonte de clock.
    // Parâmetros:
    // clk_sys: O clock do sistema a ser configurado.
    // CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX: Seleciona o clock auxiliar como fonte principal.
    // CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS: Define o PLL_SYS como fonte auxiliar de clock.
    // 125 * MHZ: A frequência desejada para o clock do sistema (125 MHz).
    // 125 * MHZ: A frequência de origem (125 MHz), que deve corresponder à saída do PLL_SYS.
    /* 
    clock_configure(clk_sys,
                    CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
                    CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 
                    100 * MHZ,   // Frequência desejada para o CLK_SYS.
                    100 * MHZ);  // Frequência de saída do PLL_SYS.
    */

    while (true)
    {
        printf("imprimindo frequências\r\n");
        measure_freqs(); //imprimir frequências
        sleep_ms(5000); //delay
    }
}
