cmake_minimum_required(VERSION 3.10)

project(RachaCuca
    VERSION 0.2.0
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
# Tabuleiro, estruturas de dados (Fila, Pilha, Tabela Hash, Lista ordenada)
# e o Solucionador. A Imagem entra numa proxima etapa.
add_library(nucleo
    Tabuleiro.cpp
    Fila.cpp
    Pilha.cpp
    TabelaHash.cpp
    ListaOrdenada.cpp
    Solucionador.cpp)
target_include_directories(nucleo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# --- Executavel principal do simulador ---
add_executable(racha_cuca main.cpp)
target_link_libraries(racha_cuca PRIVATE nucleo)

# --- Executaveis de teste (cada estrutura e testada sozinha) ---
add_executable(testes testes.cpp)
target_link_libraries(testes PRIVATE nucleo)

add_executable(testes_fila testes_fila.cpp)
target_link_libraries(testes_fila PRIVATE nucleo)

add_executable(testes_pilha testes_pilha.cpp)
target_link_libraries(testes_pilha PRIVATE nucleo)

add_executable(testes_hash testes_hash.cpp)
target_link_libraries(testes_hash PRIVATE nucleo)

add_executable(testes_lista testes_lista.cpp)
target_link_libraries(testes_lista PRIVATE nucleo)

add_executable(testes_solucionador testes_solucionador.cpp)
target_link_libraries(testes_solucionador PRIVATE nucleo)

# Integracao com CTest: 'ctest' na pasta de build executa todos.
enable_testing()
add_test(NAME tabuleiro      COMMAND testes)
add_test(NAME fila           COMMAND testes_fila)
add_test(NAME pilha          COMMAND testes_pilha)
add_test(NAME tabela_hash    COMMAND testes_hash)
add_test(NAME lista_ordenada COMMAND testes_lista)
add_test(NAME solucionador   COMMAND testes_solucionador)
