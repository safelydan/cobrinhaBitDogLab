#include "matrix.c"
#include "music.c"

#define LED_ON 1

// Função auxiliar para desenhar um frame (matriz 5x5) usando padrão “snake”
void draw_frame(int frame[5][5][3])
{
    for (int i = 0; i < 5; i++)
    {
        if (i % 2 == 0)
        {
            for (int j = 0; j < 5; j++)
            {
                // Na varredura invertida, usamos a linha 4-i e colunas de 4 a 0
                set_pixel_color(LED_ON, frame[4 - i][4 - j][0],
                                frame[4 - i][4 - j][1],
                                frame[4 - i][4 - j][2]);
            }
        }
        else
        {
            for (int j = 0; j < 5; j++)
            {
                set_pixel_color(LED_ON, frame[4 - i][j][0],
                                frame[4 - i][j][1],
                                frame[4 - i][j][2]);
            }
        }
    }
}

// Animação de sorriso: define um frame com rosto sorridente
void smile()
{
    int smile_frame[5][5][3] = {
        // Linha superior (testa)
        {{0, 0, 0}, {0, 0, 0}, {20, 20, 20}, {0, 0, 0}, {0, 0, 0}},
        // Linha dos olhos
        {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
        // Linha intermediária (espaço)
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        // Linha da boca (sorriso curvado para cima)
        {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
        // Linha inferior (boca)
        {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
    draw_frame(smile_frame);
}

// Animação de tristeza: define um frame com rosto triste
void sad()
{
    int sad_frame[5][5][3] = {
        // Linha superior (testa)
        {{0, 0, 0}, {0, 0, 0}, {20, 20, 20}, {0, 0, 0}, {0, 0, 0}},
        // Linha dos olhos
        {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
        // Linha intermediária (espaço)
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        // Linha da boca (curva para baixo)
        {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        // Linha inferior (boca)
        {{20, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {20, 0, 0}}};
    draw_frame(sad_frame);
}

// Animação de explosão: utiliza uma sequência de frames para simular a explosão
void explosion(int times)
{
    // Definindo alguns frames de explosão (você pode incrementar mais frames para suavizar)
    int explosion_frames[][5][5][3] = {
        // Frame 1: Pico central de intensidade
        {
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
        // Frame 2: Raio em forma de cruz
        {
            {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}}},
        // Frame 3: Expansão lateral
        {
            {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
            {{20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}},
            {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},
            {{20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}},
            {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}}}};

    int num_frames = sizeof(explosion_frames) / sizeof(explosion_frames[0]);
    int delayExplosion = 300; // milissegundos

    for (int t = 0; t < times; t++)
    {
        for (int f = 0; f < num_frames; f++)
        {
            draw_frame(explosion_frames[f]);
            death();
            sleep_ms(delayExplosion);
        }
    }
}

// Animação de contagem regressiva: 3, 2, 1
void counting()
{
    int number1[5][5][3] = {
        {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}};
    int number2[5][5][3] = {
        {{0, 0, 0}, {0, 20, 0}, {0, 20, 0}, {0, 20, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 20, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 20, 0}, {0, 20, 0}, {0, 20, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 20, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 20, 0}, {0, 20, 0}, {0, 20, 0}, {0, 0, 0}}};
    int number3[5][5][3] = {
        {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {0, 0, 0}}};

    draw_frame(number3);
    sleep_ms(1000);
    draw_frame(number2);
    sleep_ms(1000);
    draw_frame(number1);
    sleep_ms(1000);
}

// Ícone de seta (por exemplo, para indicar um botão)
void seta()
{
    int seta_frame[5][5][3] = {
        {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}, {20, 0, 0}},
        {{0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {20, 0, 0}, {0, 0, 0}, {0, 0, 0}}};
    draw_frame(seta_frame);
}

// Animação de "death" unindo expressões e explosão
void animation_death()
{
    sad();
    sleep_ms(500);
    explosion(1);
}

// Ícone de troféu
void trophy()
{
    int trophy_frame[5][5][3] = {
        {{20, 20, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 20, 0}},
        {{20, 20, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 20, 0}},
        {{20, 20, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {20, 20, 0}},
        {{0, 0, 0}, {20, 20, 0}, {20, 20, 0}, {20, 20, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {20, 20, 0}, {0, 0, 0}, {0, 0, 0}}};
    draw_frame(trophy_frame);
}

// Animação de vitória unindo sorriso e troféu
void animation_win()
{
    smile();
    sleep_ms(500);
    trophy();
    sleep_ms(500);
}
