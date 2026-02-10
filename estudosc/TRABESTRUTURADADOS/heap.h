#ifndef HEAP_H
#define HEAP_H

#include "registro.h"

typedef struct {
    Registro* registros;
    int tamanho;
    int capacidade;
} Heap;

// Funções da Heap
void inicializar_heap();
void carregar_arquivo_geral_heap();
void salvar_arquivo_heap();
void heapify_up(int indice);
void heapify_down(int indice);
int inserir_heap(Registro* reg);
Registro* consultar_heap(long long chave);
int remover_heap(long long chave);
void listar_registros_heap();
void menu_heap();
void liberar_heap();
int pai(int i);
int filho_esquerdo(int i);
int filho_direito(int i);

#endif
