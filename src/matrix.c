#include <stdio.h>        // Biblioteca padrão para entrada e saída, utilizada para printf.
#include "pico/stdlib.h"  // Biblioteca padrão para funções básicas do Pico, como GPIO e temporização.
#include "hardware/adc.h" // Biblioteca para controle do ADC (Conversor Analógico-Digital).
#include <time.h>
#include <stdlib.h>       // Biblioteca para funções randômicas, usada em futuras melhorias.

#include "hardware/pio.h"
#include "../pio/ws2812.pio.h" // Arquivo gerado pelo PIO

#define LED_PIN 7   // Pino de controle dos LEDs
#define NUM_LEDS 25 // Número total de LEDs na matriz

PIO pio = pio0;
uint sm;

const uint8_t MATRIX[5][5] = {
    {24, 23, 22, 21, 20},
    {15, 16, 17, 18, 19},
    {14, 13, 12, 11, 10},
    {5, 6, 7, 8, 9},
    {4, 3, 2, 1, 0}};

// Inicializa o controlador WS2812B
void ws2812b_init()
{
    if (!pio_can_add_program(pio, &ws2812_program))
    {
        printf("Erro: PIO sem espaço para programa WS2812!\n");
        return;
    }

    sm = pio_claim_unused_sm(pio, true);
    if (sm == -1)
    {
        printf("Erro: Nenhuma state machine disponível!\n");
        return;
    }

    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, LED_PIN, 800000, false);
}

// Define a cor de um LED específico
void set_pixel_color(uint pixel, uint8_t r, uint8_t g, uint8_t b)
{
    if (pixel >= NUM_LEDS) return; // Evita acessar LEDs inválidos

    uint32_t color = (r << 16) | (g << 8) | b; // Formata cor RGB para 24 bits
    pio_sm_put_blocking(pio, sm, (color & 0xFFFFFF) << 8); // Envia comando com correção de bits
}

// Atualiza a matriz de LEDs com cor vermelha
void update_matrix()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            set_pixel_color(MATRIX[i][j], 255, 0, 0); // Liga LEDs em vermelho
        }
    }
    sleep_ms(500);
}

// Apaga todos os LEDs da matriz
void clear_matrix()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            set_pixel_color(MATRIX[i][j], 0, 0, 0); // Desliga LEDs
        }
    }
}
