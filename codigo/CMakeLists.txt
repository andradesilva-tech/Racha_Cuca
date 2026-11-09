cmake_minimum_required(VERSION 3.10)

project(RachaCuca
    VERSION 0.1.0
    DESCRIPTION "Simulador do quebra-cabeca deslizante Racha Cuca - SIN 216"
    LANGUAGES CXX)

# Padrao C++14 (compativel com o g++ 6.3.0 do MinGW).
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Tipo de build" FORCE)
endif()

# --- Biblioteca com a logica do nucleo (sem depender de imagem) ---
# Por enquanto contem apenas o Tabuleiro; Imagem e Solucionador entram
# nas proximas etapas.
add_library(nucleo
    Tabuleiro.cpp)
target_include_directories(nucleo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- Executavel principal do simulador ---
add_executable(racha_cuca main.cpp)
target_link_libraries(racha_cuca PRIVATE nucleo)

# --- Executavel de testes ---
add_executable(testes testes.cpp)
target_link_libraries(testes PRIVATE nucleo)

# Integracao com CTest: 'ctest' na pasta de build executa a suite.
enable_testing()
add_test(NAME testes_unitarios COMMAND testes)
