#include <stdio.h>             // Entrada e saída padrão
#include "pico/stdlib.h"       // Funções básicas do Pico (GPIO, temporização, etc.)
#include "hardware/adc.h"      // Controle do ADC (Conversor Analógico-Digital)
#include <time.h>
#include <stdlib.h>
#include "./src/animations.c"         // Funções de animação

// Definições dos pinos para o joystick
#define VRX_PIN 26    // Eixo X do joystick (ADC0)
#define VRY_PIN 27    // Eixo Y do joystick (ADC1)
#define SW_PIN 22     // Botão do joystick (entrada digital)

#define TAM 5
#define JOYSTICK_MEIO 2047

// Objetos no tabuleiro
enum OBJETOS {vazio = 0, cobra = 1, maca = 2};

// Direções (valores positivos ou negativos para diferenciar)
enum DIRECAO {cima = 1, baixo = -1, direita = 2, esquerda = -2};
int dir;  // direção corrente

// Tabuleiro 5x5
int tabuleiro[TAM][TAM];

// Estado do jogo
enum JOGO {morto = 0, vivo = 1};
int vida;

// Contador de ticks (para gerar maçã periodicamente)
int ticks;

// Estrutura para representar a cobra (lista ligada)
typedef struct cobra {
    int x;
    int y;
    struct cobra *prox;
} No_Cobra;

No_Cobra *cabeca = NULL;

// Retorna o tamanho da cobra (conta as células marcadas no tabuleiro)
int snake_tam(){
    int count = 0;
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            if(tabuleiro[i][j] == cobra) count++;
        }
    }
    return count;
}

// Gera uma maçã em uma posição aleatória que não esteja ocupada pela cobra
void generate_apple(){
    int x, y;
    // Repete até encontrar uma posição válida
    do {
        x = rand() % TAM;
        y = rand() % TAM;
    } while(tabuleiro[x][y] == cobra);
    tabuleiro[x][y] = maca;
}

// Adiciona um novo nó à cabeça da cobra
void add(int x, int y) {
    No_Cobra *novo = malloc(sizeof(No_Cobra));
    if(novo == NULL){
        printf("Erro ao alocar memória!\n");
        return;
    }
    novo->x = x;
    novo->y = y;
    novo->prox = cabeca;
    cabeca = novo;
}

// Remove o nó da cauda da cobra (o último nó da lista ligada)
void remover() {
    if(cabeca == NULL) return;
    
    // Se houver apenas um nó, remove a cabeça
    if(cabeca->prox == NULL){
        free(cabeca);
        cabeca = NULL;
    } else {
        // Percorre até o penúltimo nó
        No_Cobra *aux = cabeca;
        while(aux->prox->prox != NULL){
            aux = aux->prox;
        }
        free(aux->prox);
        aux->prox = NULL;
    }
}

// Finaliza o jogo: exibe mensagem, anima e libera a memória da cobra
void gameover(){
    if(snake_tam() > 24){
        printf("\nVocê venceu!\n");
        animation_win();
    } else {
        printf("\nVocê perdeu.\n");
        animation_death();
    }
    vida = morto;

    // Libera toda a lista ligada
    while(cabeca){
        remover();
    }
}

// Move a cobra com wrap-around nas bordas
int mover(int direcao){
    int x = cabeca->x;
    int y = cabeca->y;
    printf("\nDirecão escolhida: %d\n", direcao);

    // Atualiza as coordenadas com base na direção
    switch (direcao) {
        case cima:    x--; break;
        case baixo:   x++; break;
        case direita: y++; break;
        case esquerda:y--; break;
    }

    // Wrap-around: se ultrapassar as bordas, aparece do lado oposto
    if(x < 0)       x = TAM - 1;
    else if(x >= TAM) x = 0;
    
    if(y < 0)       y = TAM - 1;
    else if(y >= TAM) y = 0;
    
    // Se a nova posição já estiver ocupada pela cobra, é colisão
    if(tabuleiro[x][y] == cobra){
        gameover();
        return -1;
    }
    
    // Adiciona a nova posição na cabeça
    add(x, y);
    
    // Se não for maçã, remove a cauda (movimento normal)
    switch (tabuleiro[x][y]) {
        case vazio:
            remover();
            break;
        case maca:
            // Ao comer a maçã, a cobra cresce (não remove a cauda)
            break;
    }
    
    play_pacman_walk_sound();
    return 0;
}

// Atualiza o tabuleiro: limpa posições que não são maçã e marca a posição da cobra
void verifica(){
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            if(tabuleiro[i][j] != maca){
                tabuleiro[i][j] = vazio;
            }
        }
    }
    
    No_Cobra *aux = cabeca;
    while(aux){
        tabuleiro[aux->x][aux->y] = cobra;
        aux = aux->prox;
    }
}

// Determina a direção com base nos valores do joystick
int direcao(int x, int y){
    int novaDir = 0;
    if(x == -1 || y == -1){
        // Caso de teste: entrada manual (não esperado em operação normal)
        printf("\nValor X: ");
        scanf("%d", &x);
        printf("\nValor Y: ");
        scanf("%d", &y);
    }

    // Se o movimento horizontal for mais pronunciado
    if(abs(x - JOYSTICK_MEIO) > abs(y - JOYSTICK_MEIO)){
        if(abs(x - JOYSTICK_MEIO) > 500){
            novaDir = (x - JOYSTICK_MEIO > 0) ? baixo : cima;
        }
    }
    // Se o movimento vertical for mais pronunciado
    if(abs(y - JOYSTICK_MEIO) > abs(x - JOYSTICK_MEIO)){
        if(abs(y - JOYSTICK_MEIO) > 500){
            novaDir = (y - JOYSTICK_MEIO > 0) ? direita : esquerda;
        }
    }
    // Impede que a cobra se mova na direção oposta imediata (exceto se tiver tamanho 1)
    if((novaDir + dir == 0 && snake_tam() > 1) || novaDir == 0) return dir; 
    dir = novaDir;
    return novaDir;
}

// Exibe o tabuleiro no terminal para depuração
void show(){
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            printf("%d |", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

// Atualiza a matriz LED (assumindo que MATRIX e set_pixel_color estão definidos em animation.c)
void set_matrix(){ 
    for (size_t i = 0; i < TAM; i++){
        for (size_t j = 0; j < TAM; j++){
            // A cada linha, inverter a ordem para o efeito "snake" (como na animação)
            int pos_j = (i % 2 == 0) ? TAM - j - 1 : j;
            if(i == cabeca->x && pos_j == cabeca->y){
                // Destaque para a cabeça da cobra
                set_pixel_color(MATRIX[i][j], 20, 20, 0);
            } else {
                switch (tabuleiro[i][pos_j]) {
                    case vazio:
                        set_pixel_color(MATRIX[i][j], 0, 0, 0);
                        break;
                    case cobra:
                        set_pixel_color(MATRIX[i][j], 20, 0, 0);
                        break;
                    case maca:
                        set_pixel_color(MATRIX[i][j], 0, 20, 0);
                        break;
                }
            }
        }
    }
}

// Função principal de atualização do jogo (tick)
int tick(int x, int y){
    if(mover(direcao(x, y)) == -1)
        return -1;
    
    if(!vida)
        return -1;
    
    verifica();
    show();
    set_matrix();
    
    if(ticks >= 5){
        generate_apple();
        ticks = 0;  
    } else {
        ticks++;
    }
    return 0;
}

// Inicializa a direção aleatória (ajustando se estiver na borda)
void dir_init(int x_inicial, int y_inicial){
    int d = rand() % 2 + 1;  // gera 1 ou 2
    int sentido = rand() % 2;
    dir = (sentido == 1) ? d : d * -1;

    if(x_inicial == 0 && dir == cima)
        dir = baixo;
    if(x_inicial == TAM - 1 && dir == baixo)
        dir = cima;
    if(y_inicial == 0 && dir == esquerda)
        dir = direita;
    if(y_inicial == TAM - 1 && dir == direita)
        dir = esquerda;
}

// Inicializa o jogo: estado, tabuleiro, cobra e direção
void init(){
    vida = vivo;
    ticks = 0;
    int x_inicial = 2;
    int y_inicial = 2;
    
    // Zera o tabuleiro
    for(int i = 0; i < TAM; i++){
        for(int j = 0; j < TAM; j++){
            tabuleiro[i][j] = vazio;
        }
    }
    
    tabuleiro[x_inicial][y_inicial] = cobra;
    add(x_inicial, y_inicial);
    dir_init(x_inicial, y_inicial);
}
