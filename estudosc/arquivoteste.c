#include <stdio.h>
#include <stdlib.h>

typedef struct Funcionario {
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
}Tfunc;
//salva no arquivo out, na posição atual do cursor
void salva(Tfunc *func,FILE *out) { //quando é numero bota & antes da declarar o que vai ser escrito
    fwrite(&func->cod, sizeof(int), 1, out);
    fwrite(func->nome, sizeof(char),sizeof(func->nome),out);
    fwrite(func->cpf, sizeof(char),sizeof(func->cpf),out);
    fwrite(func->data_nascimento, sizeof(char),sizeof(func->data_nascimento),out);
    fwrite(&func->salario, sizeof(double), 1, out);
}
//Le do arquivo in na posição atual do cursor
//retorna um ponteiro para funcionário lido do arquivo
Tfunc *le(FILE *in) {
    Tfunc *func = (Tfunc*)malloc(sizeof(Tfunc));
    if (0>=fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char),sizeof(func->nome), in);
    fread(func->cpf, sizeof(char),sizeof(func->cpf), in);
    fread(func->data_nascimento, sizeof(char),sizeof(func->data_nascimento), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}
Tfunc *busca_binaria(int cod, FILE *arq, int tam) {
    int inicio = 0;
    int fim = tam - 1;
    int meio;
    Tfunc *func = (Tfunc*)malloc(sizeof(Tfunc));
    if (func == NULL) {
        printf("Erro ao alocar memoria");
        return NULL;
    }
    while (inicio<=fim) {
        meio = (inicio+fim)/2;
        //posiciona cursor no registro do meio
        fseek(arq, meio*sizeof(Tfunc),SEEK_SET);
        //le registro do meio
        int qtd_lidos = fread(func, sizeof(Tfunc),1,arq);
        if (qtd_lidos!=1) {
            printf("Erro ao ler arquivo");
            free(func);
            return NULL;
        }
        if (func->cod == cod) {
            return func;//encontrou o funcionario
        }
        else if (func->cod < cod) {
            inicio = meio+1; // busca na metade maior
        }
        else if (func->cod > cod) {
            fim = meio-1; // busca na metade menor
        }
    }
    free(func);
    return NULL;
}