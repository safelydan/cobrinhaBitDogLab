# Jogo da Cobrinha (Snake Game) - Raspberry Pi Pico

Este é um jogo da cobrinha desenvolvido para o Raspberry Pi Pico W. O jogo inclui sons, animações e efeitos visuais em uma matriz de LEDs WS2812.

## 🎮 Funcionalidades
- Movimento da cobrinha em uma matriz de LEDs WS2812.
- Sons temáticos para ações como comer comida e colisão.
- Pausar e retomar o jogo com indicação sonora.
- Controle via botões físicos no Raspberry Pi Pico.

## 🛠️ Requisitos
- **Hardware:**
  - Raspberry Pi Pico W
  - Matriz de LEDs WS2812
  - Buzzer para efeitos sonoros
  - Botões para controle
- **Software:**
  - SDK do Raspberry Pi Pico
  - CMake
  - Compilador ARM GCC

## Envie o firmware para o Raspberry Pi Pico

1. Conecte o Pico ao PC segurando o botão **BOOTSEL**.
2. Copie o arquivo `.uf2` gerado para o drive do Pico.

## 🎵 Sons e Efeitos

- **Movimento:** Som grave curto.
- **Comer comida:** Som médio agudo.
- **Colisão/Game Over:** Som grave prolongado.
- **Pause/Resume:** Nota C4 indicando mudança de estado.

---

## 📝 Melhorias Futuras

- Implementação de níveis de dificuldade.
- Exibição de pontuação na matriz de LEDs.
- Modos multiplayer com comunicação serial.

---

## 📄 Licença

Este projeto é de código aberto sob a licença **MIT**. Sinta-se livre para contribuir! 🚀
