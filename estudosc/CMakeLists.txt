cmake_minimum_required(VERSION 3.30)
project(estudosc C)

set(CMAKE_C_STANDARD 23)

add_executable(estudosc main.c
        arquivo.c
        grafos.c
        grafoestudo.c
        arquivoteste.c
        geraçao_particoes.c
        teamufofa.c
        arvoreb.c
        heapMAX.c
        TRABALHOED/heaptrab.c
        TRABALHOED/hashtrab.c
        TRABALHOED/arvoreBplustrab.c
        TRABEDTESTE/B+teste.c
        TRABEDTESTE/hashteste.c
        TRABEDTESTE/heapteste.c
        TRABESTRUTURADADOS/registro.h
        TRABESTRUTURADADOS/registro.c
        TRABESTRUTURADADOS/bmais.h
        TRABESTRUTURADADOS/bmais.c
        TRABESTRUTURADADOS/hash.h
        TRABESTRUTURADADOS/hash.c
        TRABESTRUTURADADOS/heap.h
        TRABESTRUTURADADOS/heap.c
        TRABESTRUTURADADOS/main.c
)
