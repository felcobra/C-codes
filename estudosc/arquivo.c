#include<stdio.h>
#include<stdlib.h>
int main(int arqc, char** arqv){
    FILE *arq;
    arq = fopen("teste.txt", "r"); // Abre o arquivo para leitura
    // w abre pra escrita, sobrescrevendo o arquivo se ele já existir
    // a abre pra escrita, adicionando no final do arquivo se ele já existir
    // r+ abre pra leitura e escrita, mas não sobrescreve o arquivo
    // w+ abre pra leitura e escrita, sobrescrevendo o arquivo se ele já existir
    // a+ abre pra leitura e escrita, adicionando no final do arquivo se ele já existir
    if (arq != NULL) {
        // Lê o arquivo linha por linha
        
        fclose(arq);
    }
    else printf("Erro ao abrir o arquivo\n");
}//
// Created by srvit on 06/05/2025.
//
typedef struct No {
    TCliente *vencedor;
    struct No *endVencedor;
    FILE *f;
    struct No *pai;
    struct No *dir;
    struct No *esq;
}Tno;