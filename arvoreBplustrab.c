#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ORDEM 4
#define MAX_CHAVES (ORDEM - 1)
#define MIN_CHAVES ((ORDEM / 2) - 1)
#define TOTAL_REGISTROS 10000

typedef struct {
    char nome[50];
    int cpf[11];
    int nota;
    long long chave;
} Registro;

typedef struct NoBMais {
    int folha;
    int num_chaves;
    long long chaves[MAX_CHAVES];
    long long posicoes[MAX_CHAVES];
    struct NoBMais* filhos[ORDEM];
    struct NoBMais* proximo;
} NoBMais;

NoBMais* raiz = NULL;

long long converte_cpf_para_chave(int cpf[11]) {
    long long c = 0, m = 1;
    for (int i = 8; i >= 0; i--) {
        c += cpf[i] * m;
        m *= 10;
    }
    return c;
}

void gerar_nome(char* nome) {
    for (int i = 0; i < 49; i++)
        nome[i] = 'a' + rand() % 26;
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
    printf("==================");
    printf("\nRegistro\n");
    printf("Chave: %lld\n", reg->chave);
    printf("Nome: %s\n", reg->nome);
    printf("Nota: %d\n", reg->nota);
    printf("CPF: ");
    for (int i = 0; i < 11; i++)
        printf("%d", reg->cpf[i]);
    printf("\n===============\n");
}

NoBMais* cria_no(int folha) {
    NoBMais* novo = (NoBMais*)malloc(sizeof(NoBMais));
    novo->folha = folha;
    novo->num_chaves = 0;
    novo->proximo = NULL;
    for (int i = 0; i < ORDEM; i++)
        novo->filhos[i] = NULL;
    return novo;
}

void divide_no(NoBMais* pai, int idx, NoBMais* cheio) {
    NoBMais* novo = cria_no(cheio->folha);
    novo->num_chaves = MIN_CHAVES;
    for (int j = 0; j < MIN_CHAVES; j++) {
        novo->chaves[j] = cheio->chaves[j + MIN_CHAVES + 1];
        novo->posicoes[j] = cheio->posicoes[j + MIN_CHAVES + 1];
    }
    if (!cheio->folha) {
        for (int j = 0; j <= MIN_CHAVES; j++)
            novo->filhos[j] = cheio->filhos[j + MIN_CHAVES + 1];
    }
    cheio->num_chaves = MIN_CHAVES;
    for (int j = pai->num_chaves; j >= idx + 1; j--)
        pai->filhos[j + 1] = pai->filhos[j];
    pai->filhos[idx + 1] = novo;
    for (int j = pai->num_chaves - 1; j >= idx; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
        pai->posicoes[j + 1] = pai->posicoes[j];
    }
    pai->chaves[idx] = cheio->chaves[MIN_CHAVES];
    pai->posicoes[idx] = cheio->posicoes[MIN_CHAVES];
    pai->num_chaves++;
    if (cheio->folha) {
        novo->proximo = cheio->proximo;
        cheio->proximo = novo;
    }
}

void insere_nao_cheio(NoBMais* no, long long chave, long long pos) {
    int i = no->num_chaves - 1;
    if (no->folha) {
        while (i >= 0 && chave < no->chaves[i]) {
            no->chaves[i + 1] = no->chaves[i];
            no->posicoes[i + 1] = no->posicoes[i];
            i--;
        }
        no->chaves[i + 1] = chave;
        no->posicoes[i + 1] = pos;
        no->num_chaves++;
    } else {
        while (i >= 0 && chave < no->chaves[i])
            i--;
        if (no->filhos[i + 1]->num_chaves == MAX_CHAVES) {
            divide_no(no, i + 1, no->filhos[i + 1]);
            if (chave > no->chaves[i + 1])
                i++;
        }
        insere_nao_cheio(no->filhos[i + 1], chave, pos);
    }
}

void insere_bmais(long long chave, long long pos) {
    if (!raiz) {
        raiz = cria_no(1);
        raiz->chaves[0] = chave;
        raiz->posicoes[0] = pos;
        raiz->num_chaves = 1;
    } else {
        if (raiz->num_chaves == MAX_CHAVES) {
            NoBMais* nova_raiz = cria_no(0);
            nova_raiz->filhos[0] = raiz;
            divide_no(nova_raiz, 0, raiz);
            int i = (nova_raiz->chaves[0] < chave) ? 1 : 0;
            insere_nao_cheio(nova_raiz->filhos[i], chave, pos);
            raiz = nova_raiz;
        } else {
            insere_nao_cheio(raiz, chave, pos);
        }
    }
}

long long busca_posicao(NoBMais* no, long long chave) {
    int i = 0;
    while (i < no->num_chaves && chave > no->chaves[i])
        i++;
    if (no->folha) {
        if (i < no->num_chaves && no->chaves[i] == chave)
            return no->posicoes[i];
        return -1;
    } else {
        return busca_posicao(no->filhos[i], chave);
    }
}

Registro* consulta_bmais(const char* nome_arquivo, long long chave) {
    long long pos = busca_posicao(raiz, chave);
    if (pos == -1) return NULL;
    FILE* f = fopen(nome_arquivo, "r");
    if (!f) return NULL;
    fseek(f, pos, SEEK_SET);
    Registro* p = malloc(sizeof(Registro));
    fscanf(f, "%lld %s %d ", &p->chave, p->nome, &p->nota);
    for (int i = 0; i < 11; i++)
        fscanf(f, "%1d", &p->cpf[i]);
    fclose(f);
    return p;
}

int remove_chave(NoBMais* no, long long chave) {
    if (!no) return 0;

    int i = 0;
    while (i < no->num_chaves && chave > no->chaves[i])
        i++;

    if (no->folha) {
        if (i < no->num_chaves && no->chaves[i] == chave) {
            // Achou a chave ai remove
            for (int j = i; j < no->num_chaves - 1; j++) {
                no->chaves[j] = no->chaves[j + 1];
                no->posicoes[j] = no->posicoes[j + 1];
            }
            no->num_chaves--;
            return 1;
        }
        return 0;  // Não encontrou na folha
    } else {
        // Navega pro filho onde tem
        return remove_chave(no->filhos[i], chave);
    }
}


void listar_registros() {
    NoBMais* no = raiz;
    while (no && !no->folha)
        no = no->filhos[0];
    while (no) {
        for (int i = 0; i < no->num_chaves; i++) {
            Registro* r = consulta_bmais("registros.txt", no->chaves[i]);
            imprime_registro(r);
            free(r);
        }
        no = no->proximo;
    }
}

void gerar_arquivito() {
    FILE* arq = fopen("registros.txt", "w");
    if (!arq) {
        printf("Erro\n");
        exit(1);
    }
    for (int i = 0; i < TOTAL_REGISTROS; i++) {
        Registro p;
        gerar_nome(p.nome);
        gerar_cpf(p.cpf);
        p.nota = gerar_nota();
        p.chave = converte_cpf_para_chave(p.cpf);
        long long pos = ftell(arq);
        fprintf(arq, "%lld %s %d ", p.chave, p.nome, p.nota);
        for (int k = 0; k < 11; k++)
            fprintf(arq, "%d", p.cpf[k]);
        fprintf(arq, "\n");
        insere_bmais(p.chave, pos);
    }
    fclose(arq);
    printf("Arquivo gerado\n");
}

int main() {
    srand(time(NULL));
    gerar_arquivito();
    int resp;
    long long chave;
    do {
        printf("\nMENU\n");
        printf("1. Consultar\n2. Inserir um registro aleatorio\n3. Remover\n4. Listar Todos\n5. Sair\nOpcao: ");
        scanf("%d", &resp);
        switch(resp) {
            case 1:
                printf("Chave pra consulta: ");
                scanf("%lld", &chave);
                imprime_registro(consulta_bmais("registros.txt", chave));
                break;
            case 2: {
                FILE* f = fopen("registros.txt", "a");
                if (!f) { printf("Erro\n"); break; }
                Registro p;
                gerar_nome(p.nome);
                gerar_cpf(p.cpf);
                p.nota = gerar_nota();
                p.chave = converte_cpf_para_chave(p.cpf);
                fseek(f, 0, SEEK_END);
                long long pos = ftell(f);
                fprintf(f, "%lld %s %d ", p.chave, p.nome, p.nota);
                for (int i = 0; i < 11; i++)
                    fprintf(f, "%d", p.cpf[i]);
                fprintf(f, "\n");
                fclose(f);
                insere_bmais(p.chave, pos);
                imprime_registro(&p);
                break;
            }
            case 3:
                printf("Chave para tirar: ");
                scanf("%lld", &chave);
                if (remove_chave(raiz, chave))
                    printf("removida\n");
                else
                    printf("a chave nao foi encontrad\n");
                break;
            case 4:
                listar_registros();
                break;
            case 5:
                break;
            default:
                break;
        }
    } while (resp != 5);
    return 0;
}
