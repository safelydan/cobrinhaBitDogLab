#include "snakeGame.c"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include <stdlib.h>
#include <time.h>

#define BUTTON_A 5
#define BUTTON_B 6 // Novo botão para pausar e retomar o jogo
#define NOTE_C4 262

bool game_paused = false; // Variável para controlar o estado do jogo

void init_hardware() {
    stdio_init_all();
    
    adc_init();
    ws2812b_init();

    adc_gpio_init(VRX_PIN); // GP26 - Eixo X do joystick
    adc_gpio_init(VRY_PIN); // GP27 - Eixo Y do joystick

    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    gpio_set_function(BUZZER_PIN2, GPIO_FUNC_PWM);
    pwm_set_enabled(pwm_gpio_to_slice_num(BUZZER_PIN2), true);

    buzzer();
    notes(BUZZER_PIN2, 1, 10);
}

uint16_t read_adc(uint input) {
    adc_select_input(input);
    return adc_read();
}

void wait_for_button_press(uint pin) {
    while (gpio_get(pin) != 0) {
        sleep_ms(50);
    }
}

void toggle_pause() {
    game_paused = !game_paused; // Alterna entre pausado e rodando
    if (game_paused) {
        notes(BUZZER_PIN2, NOTE_C4, 200); // Sinal sonoro para pausa
    } else {
        notes(BUZZER_PIN2, NOTE_C4, 200); // Sinal sonoro para retomada
    }
}

int main() {
    srand(time(NULL));
    init_hardware();

    while (1) {
        seta();
        music();

        wait_for_button_press(BUTTON_A);

        init();
        counting();
        verifica();
        show();
        set_matrix();
        sleep_ms(1000);

        while (vida) {
            if (gpio_get(BUTTON_B) == 0) { // Verifica se o botão B foi pressionado
                sleep_ms(200); // Pequeno debounce
                while (gpio_get(BUTTON_B) == 0); // Aguarda o botão ser solto
                toggle_pause();
            }
            
            if (!game_paused) {
                uint16_t vrx_value = read_adc(0); // Eixo X
                uint16_t vry_value = read_adc(1); // Eixo Y

                tick(vrx_value, vry_value);
                sleep_ms(400);
            }
        }
    }
    return 0;
}