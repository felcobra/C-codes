#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_REGISTROS 10000

typedef struct {
    char nome[50];
    int cpf[11];
    int nota;
    long long chave;
} Registro;

// Funções utilitárias
long long converte_cpf_para_chave(int cpf[11]);
void gerar_nome(char *nome);
void gerar_cpf(int* cpf);
int gerar_nota();
void imprime_registro(Registro* reg);
void gerar_arquivo_geral();
Registro* le_registro_arquivo(const char* nome_arquivo, long long posicao);
void escreve_registro_arquivo(FILE* arq, Registro* reg);

#endif