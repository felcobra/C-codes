#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ORDEM 4
#define MAX (ORDEM - 1)
#define MIN ((ORDEM / 2) - 1)
#define TOTAL 10000
typedef struct {
    char nome[50];
    int cpf[11];
    int nota;
    long long chave;
    int ativo;
} Registro;
typedef struct NoBMais {
    int folha;
    int num_chaves;
    long long chaves[MAX];
    long long posicoes[MAX];
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
    printf("Registro\n");
    printf("Chave: %lld\n", reg->chave);
    printf("Nome: %s\n", reg->nome);
    printf("Nota: %d\n", reg->nota);
    printf("CPF: ");
    for (int i = 0; i < 11; i++)
        printf("%d", reg->cpf[i]);
    printf("\n==================\n");
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
    novo->num_chaves = MIN;
    for (int j = 0; j < MIN; j++) {
        novo->chaves[j] = cheio->chaves[j + MIN + 1];
        novo->posicoes[j] = cheio->posicoes[j + MIN+ 1];
    }
    if (!cheio->folha) {
        for (int j = 0; j <= MIN; j++)
            novo->filhos[j] = cheio->filhos[j + MIN + 1];
    }
    cheio->num_chaves = MIN;
    for (int j = pai->num_chaves; j >= idx + 1; j--)
        pai->filhos[j + 1] = pai->filhos[j];
    pai->filhos[idx + 1] = novo;
    for (int j = pai->num_chaves - 1; j >= idx; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
        pai->posicoes[j + 1] = pai->posicoes[j];
    }
    pai->chaves[idx] = cheio->chaves[MIN];
    pai->posicoes[idx] = cheio->posicoes[MIN];
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
        if (no->filhos[i + 1]->num_chaves == MAX) {
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
        if (raiz->num_chaves == MAX) {
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
    if (!no) return -1;
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
    int ativo;
    if (fscanf(f, "%d %lld %s %d ", &ativo, &p->chave, p->nome, &p->nota) != 4) {
        free(p);
        fclose(f);
        return NULL;
    }
    if (ativo == 0) {
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
    p->ativo = ativo;
    fclose(f);
    return p;
}
int marcar_registro_removido(const char* nome_arquivo, long long chave) {
    long long pos = busca_posicao(raiz, chave);
    if (pos == -1) return 0;
    FILE* f = fopen(nome_arquivo, "r+");
    if (!f) return 0;
    fseek(f, pos, SEEK_SET);
    fprintf(f, "0");
    fclose(f);
    return 1;
}
int remove_chave(NoBMais* no, long long chave) {
    if (!no) return 0;
    int i = 0;
    while (i < no->num_chaves && chave > no->chaves[i])
        i++;
    if (no->folha) {
        if (i < no->num_chaves && no->chaves[i] == chave) {
            if (marcar_registro_removido("registros.txt", chave)) {
                for (int j = i; j < no->num_chaves - 1; j++) {
                    no->chaves[j] = no->chaves[j + 1];
                    no->posicoes[j] = no->posicoes[j + 1];
                }
                no->num_chaves--;
                return 1;
            }
            return 0;
        }
        return 0;
    } else {
        return remove_chave(no->filhos[i], chave);
    }
}
void listar_registros() {
    if (!raiz) {
        return;
    }
    NoBMais* no = raiz;
    while (no && !no->folha)
        no = no->filhos[0];
    int count = 0;
    while (no) {
        for (int i = 0; i < no->num_chaves; i++) {
            Registro* r = consulta_bmais("registros.txt", no->chaves[i]);
            if (r) {
                imprime_registro(r);
                free(r);
                count++;
            }
        }
        no = no->proximo;
    }
    if (count == 0) {
        printf("Nenhum registro encontrado.\n");
    } else {
        printf("Total de registros: %d\n", count);
    }
}

void recriar_arquivo_limpo() {
    FILE* temp = fopen("temp_registros.txt", "w");
    FILE* original = fopen("registros.txt", "r");
    if (!temp || !original) {
        printf("Erro ao criar arquivo temporário.\n");
        return;
    }
    raiz = NULL;
    char linha[200];
    while (fgets(linha, sizeof(linha), original)) {
        int ativo;
        long long chave;
        char nome[50];
        int nota;
        int cpf[11];
        sscanf(linha, "%d %lld %s %d", &ativo, &chave, nome, &nota);
        if (ativo == 1) {
            char* cpf_str = linha;
            for (int i = 0; i < 3; i++) {
                cpf_str = strchr(cpf_str, ' ') + 1;
            }
            for (int i = 0; i < 11; i++) {
                cpf[i] = cpf_str[i] - '0';
            }
            long long pos = ftell(temp);
            fprintf(temp, "1 %lld %s %d ", chave, nome, nota);
            for (int i = 0; i < 11; i++)
                fprintf(temp, "%d", cpf[i]);
            fprintf(temp, "\n");
            insere_bmais(chave, pos);
        }
    }
    fclose(original);
    fclose(temp);
    remove("registros.txt");
    rename("temp_registros.txt", "registros.txt");
}
int arquivo_existe(const char* nome_arquivo) {
    FILE* f = fopen(nome_arquivo, "r");
    if (f) {
        fclose(f);
        return 1;
    }
    return 0;
}
void carregar_registros_existentes() {
    FILE* f = fopen("registros.txt", "r");
    if (!f) {
        return;
    }
    printf("Carregando registros\n");
    fseek(f, 0, SEEK_END);
    fseek(f, 0, SEEK_SET);
    char linha[200];
    int count = 0;
    long long pos = 0;
    while (fgets(linha, sizeof(linha), f)) {
        int ativo;
        long long chave;
        char nome[50];
        int nota;
        pos = ftell(f) - strlen(linha);
        sscanf(linha, "%d %lld %s %d", &ativo, &chave, nome, &nota);
        if (ativo == 1) {
            insere_bmais(chave, pos);
            count++;
        }
    }
    fclose(f);
    printf("Carregados %d registros\n", count);
}
void gerar_arquivito() {
    FILE* arq = fopen("registros.txt", "w");
    if (!arq) {
        printf("Erro\n");
        exit(1);
    }
    printf("Gerando arquivo com %d registros...\n", TOTAL);
    for (int i = 0; i < TOTAL; i++) {
        Registro p;
        gerar_nome(p.nome);
        gerar_cpf(p.cpf);
        p.nota = gerar_nota();
        p.chave = converte_cpf_para_chave(p.cpf);
        p.ativo = 1;
        long long pos = ftell(arq);
        fprintf(arq, "%d %lld %s %d ", p.ativo, p.chave, p.nome, p.nota);
        for (int k = 0; k < 11; k++)
            fprintf(arq, "%d", p.cpf[k]);
        fprintf(arq, "\n");
        insere_bmais(p.chave, pos);
        if ((i + 1) % 1000 == 0) {
            printf("Gerados %d registros\n", i + 1);
        }
    }
    fclose(arq);
    printf("Arquivo gerado com %d registros\n", TOTAL);
}
int main() {
    srand(time(NULL));
    if (arquivo_existe("registros.txt")) {
        printf("1. Carregar registros\n");
        printf("2. Gerar novo arquivo (sobrepoe o que ja existe)\n");
        printf("Opcao: ");
        int opcao;
        scanf("%d", &opcao);
        if (opcao == 1) {
            carregar_registros_existentes();
        } else if (opcao == 2) {
            gerar_arquivito();
        } else {
            carregar_registros_existentes();
        }
    } else {
        gerar_arquivito();
    }
    int resp;
    long long chave;
    do {
        printf("\n MENU \n");
        printf("1. Consulta\n");
        printf("2. Inserir registro aleatorio\n");
        printf("3. Remover registro\n");
        printf("4. Listar todos registros\n");
        printf("5. Reorganizar arquivo\n");
        printf("6. Gerar novo arquivo\n");
        printf("7. Sair\n");
        printf("Opcao: ");
        scanf("%d", &resp);
        switch(resp) {
            case 1:
                printf("Chave para consulta: ");
                scanf("%lld", &chave);
                {
                    Registro* r = consulta_bmais("registros.txt", chave);
                    imprime_registro(r);
                    if (r) free(r);
                }
                break;
            case 2: {
                FILE* f = fopen("registros.txt", "a");
                if (!f) {
                    printf("Erro\n");
                    break;
                }
                Registro p;
                gerar_nome(p.nome);
                gerar_cpf(p.cpf);
                p.nota = gerar_nota();
                p.chave = converte_cpf_para_chave(p.cpf);
                p.ativo = 1;
                fseek(f, 0, SEEK_END);
                long long pos = ftell(f);
                fprintf(f, "%d %lld %s %d ", p.ativo, p.chave, p.nome, p.nota);
                for (int i = 0; i < 11; i++)
                    fprintf(f, "%d", p.cpf[i]);
                fprintf(f, "\n");
                fclose(f);

                insere_bmais(p.chave, pos);
                printf("Registro inserido:\n");
                imprime_registro(&p);
                break;
            }
            case 3:
                printf("Chave para tirar: ");
                scanf("%lld", &chave);
                if (remove_chave(raiz, chave))
                    printf("Removido\n");
                else
                    printf("não encontrado\n");
                break;
            case 4:
                printf("Lista dos registros\n");
                listar_registros();
                break;
            case 5:
                recriar_arquivo_limpo();
                break;
            case 6:
                printf("Confirma? (1-Sim, 0-Nao): ");
                int confirma;
                scanf("%d", &confirma);
                if (confirma == 1) {
                    raiz = NULL;
                    gerar_arquivito();
                }
                break;

            case 7:
                break;
            default:
                break;
        }
    } while (resp != 7);
    return 0;
}