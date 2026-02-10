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
    int ativo; // Campo para marcar se o registro está ativo
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
    fscanf(f, "%d %lld %s %d ", &ativo, &p->chave, p->nome, &p->nota);

    // Se o registro está marcado como removido, não retorna
    if (ativo == 0) {
        free(p);
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < 11; i++)
        fscanf(f, "%1d", &p->cpf[i]);

    p->ativo = ativo;
    fclose(f);
    return p;
}

// Função para marcar registro como removido no arquivo
int marcar_registro_removido(const char* nome_arquivo, long long chave) {
    long long pos = busca_posicao(raiz, chave);
    if (pos == -1) return 0;

    FILE* f = fopen(nome_arquivo, "r+");
    if (!f) return 0;

    fseek(f, pos, SEEK_SET);
    fprintf(f, "0"); // Marca como removido (ativo = 0)
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
            // Marca o registro como removido no arquivo
            if (marcar_registro_removido("registros.txt", chave)) {
                // Remove da estrutura em memória
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
        printf("Nenhum registro encontrado.\n");
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
        printf("Nenhum registro ativo encontrado.\n");
    } else {
        printf("Total de registros ativos: %d\n", count);
    }
}

void recriar_arquivo_limpo() {
    // Cria um arquivo temporário com apenas os registros ativos
    FILE* temp = fopen("temp_registros.txt", "w");
    FILE* original = fopen("registros.txt", "r");

    if (!temp || !original) {
        printf("Erro ao criar arquivo temporário.\n");
        return;
    }

    // Libera a árvore atual
    // (Aqui você pode adicionar uma função para liberar a memória da árvore)
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
            // Extrai CPF da linha
            char* cpf_str = linha;
            // Encontra a posição do CPF na linha
            for (int i = 0; i < 3; i++) {
                cpf_str = strchr(cpf_str, ' ') + 1;
            }
            // Lê os 11 dígitos do CPF
            for (int i = 0; i < 11; i++) {
                cpf[i] = cpf_str[i] - '0';
            }

            long long pos = ftell(temp);
            fprintf(temp, "1 %lld %s %d ", chave, nome, nota);
            for (int i = 0; i < 11; i++)
                fprintf(temp, "%d", cpf[i]);
            fprintf(temp, "\n");

            // Reconstrói a árvore
            insere_bmais(chave, pos);
        }
    }

    fclose(original);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove("registros.txt");
    rename("temp_registros.txt", "registros.txt");

    printf("Arquivo reorganizado com sucesso.\n");
}

void gerar_arquivito() {
    FILE* arq = fopen("registros.txt", "w");
    if (!arq) {
        printf("Erro ao criar arquivo\n");
        exit(1);
    }

    for (int i = 0; i < TOTAL_REGISTROS; i++) {
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
    }

    fclose(arq);
    printf("Arquivo gerado com %d registros\n", TOTAL_REGISTROS);
}

int main() {
    srand(time(NULL));
    gerar_arquivito();

    int resp;
    long long chave;

    do {
        printf("\n=== MENU ===\n");
        printf("1. Consultar registro\n");
        printf("2. Inserir registro aleatorio\n");
        printf("3. Remover registro\n");
        printf("4. Listar todos os registros\n");
        printf("5. Reorganizar arquivo (limpar removidos)\n");
        printf("6. Sair\n");
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
                    printf("Erro ao abrir arquivo\n");
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
                printf("Chave para remover: ");
                scanf("%lld", &chave);
                if (remove_chave(raiz, chave))
                    printf("Registro removido com sucesso\n");
                else
                    printf("Registro nao encontrado\n");
                break;

            case 4:
                printf("=== LISTAGEM DE REGISTROS ===\n");
                listar_registros();
                break;

            case 5:
                recriar_arquivo_limpo();
                break;

            case 6:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida\n");
                break;
        }
    } while (resp != 6);

    return 0;
}
