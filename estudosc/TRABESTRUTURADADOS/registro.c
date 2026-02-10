#include "registro.h"

#include <stdio.h>
#include <stdlib.h>

long long converte_cpf_para_chave(int cpf[11]) {
    long long c = 0, m = 1;
    for (int i = 8; i >= 0; i--) {
        c += cpf[i] * m;
        m *= 10;
    }
    return c;
}

void gerar_nome(char *nome) {
    char a[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 49; i++) {
        nome[i] = a[rand() % 26];
    }
    nome[49] = '\0';
}

void gerar_cpf(int* cpf) {
    for (int i = 0; i < 11; i++)
        cpf[i] = rand() % 10;
}

int gerar_nota() {
    return rand() % 101;
}

void imprime_registro(Registro* reg) {
    if (!reg) {
        printf("Registro nao encontrado.\n");
        return;
    }
    printf("==================\n");
    printf("Registro\n");
    printf("Chave: %lld\n", reg->chave);
    printf("Nome: %s\n", reg->nome);
    printf("Nota: %d\n", reg->nota);
    printf("CPF: ");
    for (int i = 0; i < 11; i++)
        printf("%d", reg->cpf[i]);
    printf("\n==================\n");
}

void gerar_arquivo_geral() {
    FILE* arq = fopen("registros_geral.txt", "w");
    if (!arq) {
        printf("Erro ao criar arquivo geral\n");
        exit(1);
    }
    
    printf("Gerando arquivo geral com %d registros...\n", TOTAL_REGISTROS);
    
    for (int i = 0; i < TOTAL_REGISTROS; i++) {
        Registro p;
        gerar_nome(p.nome);
        gerar_cpf(p.cpf);
        p.nota = gerar_nota();
        p.chave = converte_cpf_para_chave(p.cpf);
        
        escreve_registro_arquivo(arq, &p);
        
        if (i % 1000 == 0) {
            printf("Progresso: %d/%d\n", i, TOTAL_REGISTROS);
        }
    }
    
    fclose(arq);
    printf("Arquivo geral gerado com sucesso!\n");
}

Registro* le_registro_arquivo(const char* nome_arquivo, long long posicao) {
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) return NULL;
    
    fseek(f, posicao, SEEK_SET);
    Registro* p = malloc(sizeof(Registro));
    
    if (fscanf(f, "%lld %s %d ", &p->chave, p->nome, &p->nota) != 3) {
        free(p);
        fclose(f);
        return NULL;
    }
    
    for (int i = 0; i < 11; i++) {
        if (fscanf(f, "%1d", &p->cpf[i]) != 1) {
            free(p);
            fclose(f);
            return NULL;
        }
    }
    
    fclose(f);
    return p;
}

void escreve_registro_arquivo(FILE* arq, Registro* reg) {
    fprintf(arq, "%lld %s %d ", reg->chave, reg->nome, reg->nota);
    for (int i = 0; i < 11; i++)
        fprintf(arq, "%d", reg->cpf[i]);
    fprintf(arq, "\n");
}
