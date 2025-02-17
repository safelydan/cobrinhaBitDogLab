# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Importa o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

# Define o projeto
project(game C CXX ASM)  # <<< ALTERADO DE "main" PARA "game"

# Inicializa o SDK
pico_sdk_init()

# Adiciona os arquivos do projeto
add_executable(game  # <<< ALTERADO DE "main" PARA "game"
    game.c
)

# Define o nome e versão do programa
pico_set_program_name(game "game")  # <<< ALTERADO
pico_set_program_version(game "0.1")

# Ativa comunicação (modifique conforme necessário)
pico_enable_stdio_uart(game 1)
pico_enable_stdio_usb(game 1)

# Inclui bibliotecas necessárias
target_link_libraries(game 
    pico_stdlib 
    hardware_adc
    hardware_pwm
    hardware_i2c
    hardware_pio
)

# Define diretórios de include
include_directories(${CMAKE_SOURCE_DIR}/pico-ssd1306)

# Gera arquivos extras (UF2, BIN, HEX)
pico_add_extra_outputs(game)
