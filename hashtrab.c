#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_REGISTROS 10000
#define TAMANHO_TABELA 100000

typedef struct {
    char nome[50];
    int cpf[11];
    int nota;
    long long chave;
} Registro;

typedef struct {
    long long chave;
    long long posicao;
    int ocupado;       // 1 é ocupado e -1 significa q foi removido
} EntradaHash;
EntradaHash tabela[TAMANHO_TABELA];
long long converte_cpf_para_chave(int cpf[11]) {
    long long c = 0, m = 1;
    for (int i = 8; i >= 0; i--) {
        c += cpf[i] * m;
        m *= 10;
    }
    return c;
}
// Gera nome aleatório e abaixo cpf e abaixo a nota
void gerar_nome(char* nome) {
    for (int i = 0; i < 50; i++)
        nome[i] = 'a' + rand() % 26;
    nome[50] = '\0';
}
void gerar_cpf(int* cpf) {
    for (int i = 0; i < 11; i++)
        cpf[i] = rand() % 10;
}
int gerar_nota() {
    return rand() % 101;
}
int funcao_hash(long long chave) {
    srand((unsigned int)chave);
    return rand() % TAMANHO_TABELA;
}
void inicializa_tabela() {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabela[i].ocupado = 0;
        tabela[i].chave = 0;
        tabela[i].posicao = -1;
    }
}
void inserir_hash(Registro* reg, FILE* arq) {
    fseek(arq, 0, SEEK_END);
    long long pos = ftell(arq);
    fprintf(arq, "%lld %s %d ", reg->chave, reg->nome, reg->nota);
    for (int i = 0; i < 11; i++)
        fprintf(arq, "%d", reg->cpf[i]);
    fprintf(arq, "\n");
    int indc = funcao_hash(reg->chave);
    int inicio = indc;
    while (tabela[indc].ocupado == 1) {
        if (tabela[indc].chave == reg->chave) {
            // Atualiza a posição caso a chave seja repetida
            tabela[indc].posicao = pos;
            return;
        }
        indc = (indc + 1) % TAMANHO_TABELA;
        if (indc == inicio) {
            printf("Tabela cheia\n");
            return;
        }
    }
    tabela[indc].chave = reg->chave;
    tabela[indc].posicao = pos;
    tabela[indc].ocupado = 1;
}
Registro* consultar_hash(long long chave, FILE* arq) {
    int indc = funcao_hash(chave);
    int inicio = indc;
    while (tabela[indc].ocupado != 0) {
        if (tabela[indc].ocupado == 1 && tabela[indc].chave == chave) {
            Registro* reg = (Registro*)malloc(sizeof(Registro));
            fseek(arq, tabela[indc].posicao, SEEK_SET);
            fscanf(arq, "%lld %s %d ", &reg->chave, reg->nome, &reg->nota);
            for (int i = 0; i < 11; i++)
                fscanf(arq, "%1d", &reg->cpf[i]);
            return reg;
        }
        indc = (indc + 1) % TAMANHO_TABELA;
        if (indc == inicio)
            break;
    }
    return NULL;
}
int remover_hash(long long chave) {
    int indc = funcao_hash(chave);
    int inicio = indc;
    while (tabela[indc].ocupado != 0) {
        if (tabela[indc].ocupado == 1 && tabela[indc].chave == chave) {
            tabela[indc].ocupado = -1; // marcado como removido
            return 1;
        }
        indc = (indc + 1) % TAMANHO_TABELA;
        if (indc == inicio)
            break;
    }
    return 0;
}

// Listar todos os registros
void listar_todos(FILE* arq) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        if (tabela[i].ocupado == 1) {
            Registro* reg = (Registro*)malloc(sizeof(Registro));
            fseek(arq, tabela[i].posicao, SEEK_SET);
            fscanf(arq, "%lld %s %d ", &reg->chave, reg->nome, &reg->nota);
            for (int j = 0; j < 11; j++)
                fscanf(arq, "%1d", &reg->cpf[j]);
            imprime_registro(reg);
            free(reg);
        }
    }
}
void imprime_registro(Registro* reg) {
    if (!reg) {
        printf("Registro nao encontrado.\n");
        return;
    }
    printf("================");
    printf("\nRegistro\n");
    printf("Chave: %lld\n", reg->chave);
    printf("Nome: %s\n", reg->nome);
    printf("Nota: %d\n", reg->nota);
    printf("CPF: ");
    for (int i = 0; i < 11; i++)
        printf("%d", reg->cpf[i]);
    printf("\n===============\n");
}
//aqwui é onde os registros são gerados e inseridos na tabela
void gerar_registros_automaticos() {
    FILE* arq = fopen("registros.txt", "w");
    if (!arq) {
        printf("Erro\n");
        exit(1);
    }
    inicializa_tabela();

    for (int i = 0; i < TOTAL_REGISTROS; i++) {
        Registro reg;
        gerar_nome(reg.nome);
        gerar_cpf(reg.cpf);
        reg.nota = gerar_nota();
        reg.chave = converte_cpf_para_chave(reg.cpf);
        inserir_hash(&reg, arq);
    }
    fclose(arq);
    printf("Arquivo criado e tabela hash inicializada com %d registros.\n", TOTAL_REGISTROS);
}

int main() {
    srand(time(NULL));
    gerar_registros_automaticos();
    FILE* arq = fopen("registros.txt", "r+");
    if (!arq) {
        printf("Erro\n");
        return 1;
    }

    int opcao;
    long long chave;
    Registro temp;
    do {
        printf("\nMenu\n");
        printf("1. Consultar\n");
        printf("2. Inserir aleatorio\n");
        printf("3. Remover\n");
        printf("4. Listar todos\n");
        printf("5. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Chave pra consulta: ");
                scanf("%lld", &chave);
                {
                    Registro* r = consultar_hash(chave, arq);
                    imprime_registro(r);
                    if (r) free(r);
                }
                break;

            case 2:
                gerar_nome(temp.nome);
                gerar_cpf(temp.cpf);
                temp.nota = gerar_nota();
                temp.chave = converte_cpf_para_chave(temp.cpf);
                inserir_hash(&temp, arq);
                printf("Registro inserido:\n");
                imprime_registro(&temp);
                break;

            case 3:
                printf("Chave para tirar: ");
                scanf("%lld", &chave);
                if (remover_hash(chave))
                    printf("removida\n");
                else
                    printf("Chave não encontrada\n");
                break;

            case 4:
                listar_todos(arq);
                break;

            case 5:
                break;

            default:
                break;
        }
    } while (opcao != 5);

    fclose(arq);
    return 0;
}
