#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdlib.h>

// Define os pinos do buzzer
#define BUZZER_PIN 10
#define BUZZER_PIN2 21

#define BUFFER_SIZE 1024
uint16_t audio_buffer[BUFFER_SIZE];

// Tabela de frequências para sons do jogo da cobrinha
#define NOTE_LOW 200
#define NOTE_MEDIUM 400
#define NOTE_HIGH 600
#define NOTE_SCORE 800
#define NOTE_GAME_OVER 150

//------------------------------------------------------------------------------
// Função para tocar uma nota utilizando PWM
//------------------------------------------------------------------------------
void notes(uint buzzer_pin, int frequency, int duration_ms)
{
    if (frequency == 0)
    {
        gpio_put(buzzer_pin, 0); // Silencia o buzzer
        sleep_ms(duration_ms);
        return;
    }

    uint slice_num = pwm_gpio_to_slice_num(buzzer_pin);
    pwm_set_clkdiv(slice_num, 4.0);
    pwm_set_wrap(slice_num, 125000000 / (4 * frequency) - 1);
    pwm_set_enabled(slice_num, true);

    sleep_ms(duration_ms);
    pwm_set_enabled(slice_num, false);
}

//------------------------------------------------------------------------------
// Configura os pinos do buzzer para PWM
//------------------------------------------------------------------------------
void buzzer()
{
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    gpio_set_function(BUZZER_PIN2, GPIO_FUNC_PWM);
    uint slice_num2 = pwm_gpio_to_slice_num(BUZZER_PIN2);

    pwm_set_wrap(slice_num, 12500);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BUZZER_PIN), 6250);
    pwm_set_chan_level(slice_num2, pwm_gpio_to_channel(BUZZER_PIN2), 6250);
    pwm_set_enabled(slice_num, true);
}

//------------------------------------------------------------------------------
// Toca o som do movimento da cobrinha
//------------------------------------------------------------------------------
void basic_melody()
{
    notes(BUZZER_PIN, NOTE_LOW, 100);
    sleep_ms(50);
    notes(BUZZER_PIN, NOTE_MEDIUM, 100);
}

//------------------------------------------------------------------------------
// Som ao comer um item
//------------------------------------------------------------------------------
void music()
{
    notes(BUZZER_PIN, NOTE_SCORE, 150);
    sleep_ms(50);
    notes(BUZZER_PIN, NOTE_HIGH, 200);
}

//------------------------------------------------------------------------------
// Som de colisão (fim de jogo)
//------------------------------------------------------------------------------
void death()
{
    for (int freq = 1000; freq > 200; freq -= 200)
    {
        notes(BUZZER_PIN, freq, 50);
        sleep_ms(20);
    }
    notes(BUZZER_PIN, NOTE_GAME_OVER, 500);
}

//------------------------------------------------------------------------------
// Som de transição entre estados do jogo (início, pausa, etc.)
//------------------------------------------------------------------------------
void walk()
{
    notes(BUZZER_PIN, NOTE_MEDIUM, 100);
    sleep_ms(50);
    notes(BUZZER_PIN, NOTE_LOW, 100);
}
