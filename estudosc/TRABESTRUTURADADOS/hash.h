#ifndef HASH_H
#define HASH_H

#include "registro.h"

#define TAMANHO_HASH 100000

typedef struct {
    Registro reg;
    int ocupado;
    int removido;
} SlotHash;

typedef struct {
    SlotHash* tabela;
    int count;
} TabelaHash;

// Funções da Hash
void inicializar_hash();
void carregar_arquivo_geral_hash();
void salvar_arquivo_hash();
int funcao_hash(long long chave);
int inserir_hash(Registro* reg);
Registro* consultar_hash(long long chave);
int remover_hash(long long chave);
void listar_registros_hash();
void menu_hash();
void liberar_hash();

#endif
