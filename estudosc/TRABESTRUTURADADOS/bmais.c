#include "bmais.h"

NoBMais* raiz = NULL;
MemoriaRegistros memoria;

void inicializar_bmais() {
    raiz = NULL;
    inicializar_memoria();
}

void inicializar_memoria() {
    memoria.capacidade = TOTAL_REGISTROS + 1000; // Extra para inserções
    memoria.registros = malloc(sizeof(Registro) * memoria.capacidade);
    memoria.count = 0;
}

void adicionar_registro_memoria(Registro* reg) {
    if (memoria.count >= memoria.capacidade) {
        memoria.capacidade *= 2;
        memoria.registros = realloc(memoria.registros, sizeof(Registro) * memoria.capacidade);
    }
    memoria.registros[memoria.count] = *reg;
    memoria.count++;
}

Registro* buscar_registro_memoria(long long chave) {
    for (int i = 0; i < memoria.count; i++) {
        if (memoria.registros[i].chave == chave) {
            return &memoria.registros[i];
        }
    }
    return NULL;
}

int remover_registro_memoria(long long chave) {
    for (int i = 0; i < memoria.count; i++) {
        if (memoria.registros[i].chave == chave) {
            for (int j = i; j < memoria.count - 1; j++) {
                memoria.registros[j] = memoria.registros[j + 1];
            }
            memoria.count--;
            return 1;
        }
    }
    return 0;
}

void liberar_memoria() {
    free(memoria.registros);
    memoria.count = 0;
}

void carregar_arquivo_geral_bmais() {
    FILE* arq = fopen("registros_geral.txt", "r");
    if (!arq) {
        printf("Erro ao abrir arquivo geral\n");
        return;
    }

    printf("Carregando registros para B+...\n");

    long long pos = 0;
    while (!feof(arq)) {
        Registro reg;
        pos = ftell(arq);

        if (fscanf(arq, "%lld %s %d ", &reg.chave, reg.nome, &reg.nota) != 3) {
            break;
        }

        for (int i = 0; i < 11; i++) {
            if (fscanf(arq, "%1d", &reg.cpf[i]) != 1) {
                break;
            }
        }

        fscanf(arq, "\n");

        insere_bmais(reg.chave, pos);
        adicionar_registro_memoria(&reg);
    }

    fclose(arq);
    printf("Dados carregados na B+ com sucesso!\n");
}

void salvar_arquivo_bmais() {
    FILE* arq = fopen("registros_bmais.txt", "w");
    if (!arq) {
        printf("Erro ao criar arquivo da B+\n");
        return;
    }

    for (int i = 0; i < memoria.count; i++) {
        escreve_registro_arquivo(arq, &memoria.registros[i]);
    }

    fclose(arq);
    printf("Arquivo B+ salvo com sucesso!\n");
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

Registro* consulta_bmais(long long chave) {
    return buscar_registro_memoria(chave);
}

int remove_chave(NoBMais* no, long long chave) {
    if (!no) return 0;

    int i = 0;
    while (i < no->num_chaves && chave > no->chaves[i])
        i++;

    if (no->folha) {
        if (i < no->num_chaves && no->chaves[i] == chave) {
            for (int j = i; j < no->num_chaves - 1; j++) {
                no->chaves[j] = no->chaves[j + 1];
                no->posicoes[j] = no->posicoes[j + 1];
            }
            no->num_chaves--;
            return 1;
        }
        return 0;
    } else {
        return remove_chave(no->filhos[i], chave);
    }
}

void listar_registros_bmais() {
    printf("=== LISTAGEM DOS REGISTROS B+ ===\n");
    for (int i = 0; i < memoria.count; i++) {
        imprime_registro(&memoria.registros[i]);
    }
    printf("Total de registros: %d\n", memoria.count);
}

void menu_bmais() {
    int resp;
    long long chave;

    do {
        printf("\n=== MENU B+ ===\n");
        printf("1. Consultar registro\n");
        printf("2. Inserir registro aleatorio\n");
        printf("3. Remover registro\n");
        printf("4. Listar todos os registros\n");
        printf("5. Salvar arquivo B+\n");
        printf("6. Voltar ao menu principal\n");
        printf("Opcao: ");
        scanf("%d", &resp);

        switch(resp) {
            case 1:
                printf("Chave para consulta: ");
                scanf("%lld", &chave);
                imprime_registro(consulta_bmais(chave));
                break;

            case 2: {
                Registro novo_reg;
                gerar_nome(novo_reg.nome);
                gerar_cpf(novo_reg.cpf);
                novo_reg.nota = gerar_nota();
                novo_reg.chave = converte_cpf_para_chave(novo_reg.cpf);

                insere_bmais(novo_reg.chave, 0);
                adicionar_registro_memoria(&novo_reg);

                printf("Registro inserido:\n");
                imprime_registro(&novo_reg);
                break;
            }

            case 3:
                printf("Chave para remover: ");
                scanf("%lld", &chave);
                if (remove_chave(raiz, chave) && remover_registro_memoria(chave)) {
                    printf("Registro removido com sucesso!\n");
                } else {
                    printf("Registro nao encontrado!\n");
                }
                break;

            case 4:
                listar_registros_bmais();
                break;

            case 5:
                salvar_arquivo_bmais();
                break;

            case 6:
                printf("Voltando ao menu principal...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                break;
        }
    } while (resp != 6);
}
