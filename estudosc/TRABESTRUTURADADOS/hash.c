#include "hash.h"

TabelaHash tabela_hash;

void inicializar_hash() {
    tabela_hash.tabela = malloc(sizeof(SlotHash) * TAMANHO_HASH);
    tabela_hash.count = 0;

    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabela_hash.tabela[i].ocupado = 0;
        tabela_hash.tabela[i].removido = 0;
    }
}

void carregar_arquivo_geral_hash() {
    FILE* arq = fopen("registros_geral.txt", "r");
    if (!arq) {
        printf("Erro ao abrir arquivo geral\n");
        return;
    }

    printf("Carregando registros para Hash...\n");

    while (!feof(arq)) {
        Registro reg;

        if (fscanf(arq, "%lld %s %d ", &reg.chave, reg.nome, &reg.nota) != 3) {
            break;
        }

        for (int i = 0; i < 11; i++) {
            if (fscanf(arq, "%1d", &reg.cpf[i]) != 1) {
                break;
            }
        }

        fscanf(arq, "\n");

        inserir_hash(&reg);
    }

    fclose(arq);
    printf("Dados carregados na Hash com sucesso!\n");
}

void salvar_arquivo_hash() {
    FILE* arq = fopen("registros_hash.txt", "w");
    if (!arq) {
        printf("Erro ao criar arquivo da Hash\n");
        return;
    }

    for (int i = 0; i < TAMANHO_HASH; i++) {
        if (tabela_hash.tabela[i].ocupado && !tabela_hash.tabela[i].removido) {
            escreve_registro_arquivo(arq, &tabela_hash.tabela[i].reg);
        }
    }

    fclose(arq);
    printf("Arquivo Hash salvo com sucesso!\n");
}

int funcao_hash(long long chave) {
    srand(chave);
    return rand() % TAMANHO_HASH;
}

int inserir_hash(Registro* reg) {
    int pos = funcao_hash(reg->chave);
    int pos_original = pos;

    // Endereçamento aberto - sondagem linear
    while (tabela_hash.tabela[pos].ocupado && !tabela_hash.tabela[pos].removido) {
        if (tabela_hash.tabela[pos].reg.chave == reg->chave) {
            return 0; // Chave já existe
        }
        pos = (pos + 1) % TAMANHO_HASH;
        if (pos == pos_original) {
            return 0; // Tabela cheia
        }
    }

    tabela_hash.tabela[pos].reg = *reg;
    tabela_hash.tabela[pos].ocupado = 1;
    tabela_hash.tabela[pos].removido = 0;
    tabela_hash.count++;

    return 1;
}

Registro* consultar_hash(long long chave) {
    int pos = funcao_hash(chave);
    int pos_original = pos;

    while (tabela_hash.tabela[pos].ocupado) {
        if (!tabela_hash.tabela[pos].removido &&
            tabela_hash.tabela[pos].reg.chave == chave) {
            return &tabela_hash.tabela[pos].reg;
        }
        pos = (pos + 1) % TAMANHO_HASH;
        if (pos == pos_original) {
            break;
        }
    }

    return NULL;
}

int remover_hash(long long chave) {
    int pos = funcao_hash(chave);
    int pos_original = pos;

    while (tabela_hash.tabela[pos].ocupado) {
        if (!tabela_hash.tabela[pos].removido &&
            tabela_hash.tabela[pos].reg.chave == chave) {
            tabela_hash.tabela[pos].removido = 1;
            tabela_hash.count--;
            return 1;
        }
        pos = (pos + 1) % TAMANHO_HASH;
        if (pos == pos_original) {
            break;
        }
    }

    return 0;
}

void listar_registros_hash() {
    printf("=== LISTAGEM DOS REGISTROS HASH ===\n");
    int total = 0;

    for (int i = 0; i < TAMANHO_HASH; i++) {
        if (tabela_hash.tabela[i].ocupado && !tabela_hash.tabela[i].removido) {
            imprime_registro(&tabela_hash.tabela[i].reg);
            total++;
        }
    }

    printf("Total de registros: %d\n", total);
}

void liberar_hash() {
    free(tabela_hash.tabela);
    tabela_hash.count = 0;
}

void menu_hash() {
    int resp;
    long long chave;

    do {
        printf("\n=== MENU HASH ===\n");
        printf("1. Consultar registro\n");
        printf("2. Inserir registro aleatorio\n");
        printf("3. Remover registro\n");
        printf("4. Listar todos os registros\n");
        printf("5. Salvar arquivo Hash\n");
        printf("6. Voltar ao menu principal\n");
        printf("Opcao: ");
        scanf("%d", &resp);

        switch(resp) {
            case 1:
                printf("Chave para consulta: ");
                scanf("%lld", &chave);
                imprime_registro(consultar_hash(chave));
                break;

            case 2: {
                Registro novo_reg;
                gerar_nome(novo_reg.nome);
                gerar_cpf(novo_reg.cpf);
                novo_reg.nota = gerar_nota();
                novo_reg.chave = converte_cpf_para_chave(novo_reg.cpf);

                if (inserir_hash(&novo_reg)) {
                    printf("Registro inserido:\n");
                    imprime_registro(&novo_reg);
                } else {
                    printf("Erro ao inserir registro!\n");
                }
                break;
            }

            case 3:
                printf("Chave para remover: ");
                scanf("%lld", &chave);
                if (remover_hash(chave)) {
                    printf("Registro removido com sucesso!\n");
                } else {
                    printf("Registro nao encontrado!\n");
                }
                break;

            case 4:
                listar_registros_hash();
                break;

            case 5:
                salvar_arquivo_hash();
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
