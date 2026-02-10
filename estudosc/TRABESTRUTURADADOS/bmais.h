#ifndef BMAIS_H
#define BMAIS_H

#include "registro.h"

#define ORDEM 4
#define MAX_CHAVES (ORDEM - 1)
#define MIN_CHAVES ((ORDEM / 2) - 1)

typedef struct NoBMais {
    int folha;
    int num_chaves;
    long long chaves[MAX_CHAVES];
    long long posicoes[MAX_CHAVES];
    struct NoBMais* filhos[ORDEM];
    struct NoBMais* proximo;
} NoBMais;

// Estrutura para manter registros em memória
typedef struct {
    Registro* registros;
    int capacidade;
    int count;
} MemoriaRegistros;

// Funções da B+
void inicializar_bmais();
void carregar_arquivo_geral_bmais();
void salvar_arquivo_bmais();
NoBMais* cria_no(int folha);
void divide_no(NoBMais* pai, int idx, NoBMais* cheio);
void insere_nao_cheio(NoBMais* no, long long chave, long long pos);
void insere_bmais(long long chave, long long pos);
long long busca_posicao(NoBMais* no, long long chave);
Registro* consulta_bmais(long long chave);
int remove_chave(NoBMais* no, long long chave);
void listar_registros_bmais();
void menu_bmais();

// Funções para gerenciar memória
void inicializar_memoria();
void adicionar_registro_memoria(Registro* reg);
Registro* buscar_registro_memoria(long long chave);
int remover_registro_memoria(long long chave);
void liberar_memoria();

#endif